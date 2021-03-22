#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <string.h>
#include<math.h>		// 用于计算id取后几位
#include "read_write.h"
 
std::unordered_map<int, FILE*> m;
// 全局数组,数组元素是块的指针
char* block[MAP_SIZE] = { nullptr };
// 全局数组,数组元素是每个块字节数,写入前不能超过5,000,000字节,约5MB
int block_size[MAP_SIZE] = { 0 };

// 打开文件
void write_file_open() {
	char file_name[] = { 'D',':','\\','d','a','t','a','\\','0','0','0','.','t','x','t','\0' };
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i] = fopen(file_name, "w");
		if (m[i] == nullptr)
			std::cout << "open write file failed" << std::endl;
		if (file_name[10] == '9') {
			file_name[10] = '0';
			if (file_name[9] == '9'){
				file_name[9] = '0';
				++file_name[8];
			}
			else {
				++file_name[9];
			}
		}
		else {
			++file_name[10];
		}
	}
}

// 关闭文件流
void write_file_close() {
	for (int i = MAP_SIZE - 1; i > 0; --i) {
		// 判断文件描述符是否打开
		if (m[i] != nullptr) {
			fclose(m[i]);
			m[i] = nullptr;
		}
	}
}

// 将数据块写入文件
void fwrite_file(const int key) {
	// 先获取文件描述符
	fwrite(block[key], sizeof(char), block_size[key], m[key]);
}

// free手动分配的内存
void free_block() {
	for (int i = 0; i < MAP_SIZE; ++i) {
		// free前清空缓冲区
		if (block[i] != nullptr) {
			fwrite_file(i);
			block_size[i] = 0;
			free(block[i]);
			block[i] = nullptr;
		}
	}
}

// 将行写入数据块，被fread_line调用
void fwrite_block(const char* line,const int key) {
	// 判断将要写入的数据块是不是NULL指针
	if (block[key] == nullptr)
		block[key] = (char*)malloc(BLOCK_SIZE * sizeof(char));

	size_t line_size = strlen(line);
	// 先判断加上该行后是否超出块大小
	if (block_size[key] + line_size > BLOCK_SIZE) {
		// 向文件中写入该块
		fwrite_file(key);
		// 将块大小置为0
		block_size[key] = 0;
	}

	// 向块中写数据,不包括line后面的'\0'
	for (int i = 0; i < line_size; ++i) {
		*(block[key] + block_size[key]++) = *(line + i);
	}
}

// 处理一行数据，数据以'/n'结尾
void fread_line(char* line) {
	char* cur_ptr = line;
	char new_line[LINE_SIZE];		// 存储 id，时间，经度，纬度，速度，方位角，数据来源，运动状态
	char* new_line_ptr = new_line;
	int i = 0;
	int key = 0;
	// 指针指向第一位
	// 读id
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}

	// 取出id后k位,根据MAP_SIZE而确定
	int k = (int)ceil(log(MAP_SIZE) / log(10));
	for (i = 0; i < k; ++i) {
		if (cur_ptr - line == i + 1) {
			key += (*(cur_ptr - i - 1) - '0') * (int)pow(10, i);
			break;
		}
		key += (*(cur_ptr - i - 1) - '0') * (int)pow(10, i);
	}
	i = 0;
	key %= MAP_SIZE;

	if (key < 0 || key>MAP_SIZE-1) {
		char* tmp = line;
		while (tmp != cur_ptr) {
			std::cout << *tmp << std::endl;
			++tmp;
		}
	}

	++cur_ptr;	// 跳过'\t'
	*new_line_ptr++ = '\t';		// 以'\t'隔开id与时间

	// 读时间，时间共19个字符
	while (i++ < 19) {
		*new_line_ptr++ = *cur_ptr++;
	}
	*new_line_ptr++ = '\t';		// 以'\t'隔开时间与经度

	// 跳过三个'\t'
	i = 0;
	while (i < 3) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	// 读取经度
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// 以'\t'隔开经度与纬度
	//读取纬度
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// 以'\t'隔开纬度与速度
	//读取速度
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// 以'\t'隔开速度与方位角
	//读取方位角
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// 以'\t'隔开方位角与数据来源

	// 跳过卫星数
	i = 0;
	while (i < 1) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	//读取数据来源
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}

	++cur_ptr;
	*new_line_ptr++ = '\t';		// 以'\t'隔开与数据来源与运动状态

	// 跳过供电方式
	i = 0;
	while (i < 1) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	//读取运动状态
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\n';
	*new_line_ptr++ = '\0';
	//printf("%s\n",new_line);
	fwrite_block(new_line, key);	//调用fwrite_block ,将行写入块
	// 将指针置空
	cur_ptr = nullptr;
	new_line_ptr = nullptr;
}

// 解析数据块，返回需要回退的字节数，数据块的最后一个字符为'\0'
int fread_block(char* block) {
	// 每读到一个'\n',就读取一行
	char* cur_ptr = block;
	char line[LINE_SIZE];
	int line_size = 0;
	int line_count = 0;
	//int i = 0;
	while (*cur_ptr != '\0') {
		// 当读到某一行末尾时
		if (*cur_ptr == '\n') {
			// 行数量自增
			++line_count;
			// line读入'\n'
			line[line_size++] = *cur_ptr++;
			// line以'\0'结尾
			line[line_size] = '\0';
			fread_line(line);
			line_size = 0;
		}
		else {
			line[line_size++] = *cur_ptr++;
		}
	}
	// std::cout << line_size << std::endl;
	cur_ptr = nullptr;
	return line_size;
}

// 使用fread读取数据
void fread_file(std::string& file_name) {

	// 打开写的文件
	write_file_open();
	// 打开读的文件 'rb'模式读取的换行符和源文件中保持一致
	//FILE* fp = nullptr;	
	FILE* fp = fopen(file_name.c_str(), "rb");
	// 判断读文件是否打开失败
	if (fp == nullptr)
		std::cout << "open read file failed" << std::endl;

	//首先要跳过第一行
	while (getc(fp) != '\n') {
	}

	char block[BLOCK_SIZE];
	size_t block_size = BLOCK_SIZE;

	int backwards = 0;		// 需要回退的字节数
	int i = 0;
	auto t = clock();

	char temp[1];

	while (!feof(fp)) {
		// 通过返回值判断读到哪了
		size_t t = fread(block, sizeof(char), block_size - 1, fp);
		//std::cout << ftell(fp) << std::endl;
		//std::cout << t << std::endl;

		if (t < block_size - 1) {
			fseek(fp, t * sizeof(char), SEEK_END);
			fread(block, sizeof(char), t, fp);
			block[t] = '\0';
			fread_block(block);
			break;
		}
		else {
			block[block_size - 1] = '\0';
			backwards = fread_block(block);
			fseek(fp, -backwards * sizeof(char), SEEK_CUR);
			// 检验读写指针是否正确地回到行首
			fseek(fp, -1, SEEK_CUR);
			fread(temp, sizeof(char), 1, fp);
			if (temp[0] != '\n')
				std::cout << temp << std::endl;
		}

		if (i++ % 1000 == 0)
			std::cout << i / 1000 << std::endl;
	}
	std::cout << clock() - t << std::endl;
	// 把手动分配的空间free掉
	free_block();
	// 关闭写文件流
	write_file_close();
	// 关闭读文件流
	fclose(fp);
	fp = nullptr;
}
