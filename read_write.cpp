#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <string.h>
#include<math.h>		// ���ڼ���idȡ��λ
#include "read_write.h"
 
std::unordered_map<int, FILE*> m;
// ȫ������,����Ԫ���ǿ��ָ��
char* block[MAP_SIZE] = { nullptr };
// ȫ������,����Ԫ����ÿ�����ֽ���,д��ǰ���ܳ���5,000,000�ֽ�,Լ5MB
int block_size[MAP_SIZE] = { 0 };

// ���ļ�
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

// �ر��ļ���
void write_file_close() {
	for (int i = MAP_SIZE - 1; i > 0; --i) {
		// �ж��ļ��������Ƿ��
		if (m[i] != nullptr) {
			fclose(m[i]);
			m[i] = nullptr;
		}
	}
}

// �����ݿ�д���ļ�
void fwrite_file(const int key) {
	// �Ȼ�ȡ�ļ�������
	fwrite(block[key], sizeof(char), block_size[key], m[key]);
}

// free�ֶ�������ڴ�
void free_block() {
	for (int i = 0; i < MAP_SIZE; ++i) {
		// freeǰ��ջ�����
		if (block[i] != nullptr) {
			fwrite_file(i);
			block_size[i] = 0;
			free(block[i]);
			block[i] = nullptr;
		}
	}
}

// ����д�����ݿ飬��fread_line����
void fwrite_block(const char* line,const int key) {
	// �жϽ�Ҫд������ݿ��ǲ���NULLָ��
	if (block[key] == nullptr)
		block[key] = (char*)malloc(BLOCK_SIZE * sizeof(char));

	size_t line_size = strlen(line);
	// ���жϼ��ϸ��к��Ƿ񳬳����С
	if (block_size[key] + line_size > BLOCK_SIZE) {
		// ���ļ���д��ÿ�
		fwrite_file(key);
		// �����С��Ϊ0
		block_size[key] = 0;
	}

	// �����д����,������line�����'\0'
	for (int i = 0; i < line_size; ++i) {
		*(block[key] + block_size[key]++) = *(line + i);
	}
}

// ����һ�����ݣ�������'/n'��β
void fread_line(char* line) {
	char* cur_ptr = line;
	char new_line[LINE_SIZE];		// �洢 id��ʱ�䣬���ȣ�γ�ȣ��ٶȣ���λ�ǣ�������Դ���˶�״̬
	char* new_line_ptr = new_line;
	int i = 0;
	int key = 0;
	// ָ��ָ���һλ
	// ��id
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}

	// ȡ��id��kλ,����MAP_SIZE��ȷ��
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

	++cur_ptr;	// ����'\t'
	*new_line_ptr++ = '\t';		// ��'\t'����id��ʱ��

	// ��ʱ�䣬ʱ�乲19���ַ�
	while (i++ < 19) {
		*new_line_ptr++ = *cur_ptr++;
	}
	*new_line_ptr++ = '\t';		// ��'\t'����ʱ���뾭��

	// ��������'\t'
	i = 0;
	while (i < 3) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	// ��ȡ����
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// ��'\t'����������γ��
	//��ȡγ��
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// ��'\t'����γ�����ٶ�
	//��ȡ�ٶ�
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// ��'\t'�����ٶ��뷽λ��
	//��ȡ��λ��
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\t';		// ��'\t'������λ����������Դ

	// ����������
	i = 0;
	while (i < 1) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	//��ȡ������Դ
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}

	++cur_ptr;
	*new_line_ptr++ = '\t';		// ��'\t'������������Դ���˶�״̬

	// �������緽ʽ
	i = 0;
	while (i < 1) {
		if (*cur_ptr++ == '\t')
			++i;
	}

	//��ȡ�˶�״̬
	while (*cur_ptr != '\t') {
		*new_line_ptr++ = *cur_ptr++;
	}
	++cur_ptr;
	*new_line_ptr++ = '\n';
	*new_line_ptr++ = '\0';
	//printf("%s\n",new_line);
	fwrite_block(new_line, key);	//����fwrite_block ,����д���
	// ��ָ���ÿ�
	cur_ptr = nullptr;
	new_line_ptr = nullptr;
}

// �������ݿ飬������Ҫ���˵��ֽ��������ݿ�����һ���ַ�Ϊ'\0'
int fread_block(char* block) {
	// ÿ����һ��'\n',�Ͷ�ȡһ��
	char* cur_ptr = block;
	char line[LINE_SIZE];
	int line_size = 0;
	int line_count = 0;
	//int i = 0;
	while (*cur_ptr != '\0') {
		// ������ĳһ��ĩβʱ
		if (*cur_ptr == '\n') {
			// ����������
			++line_count;
			// line����'\n'
			line[line_size++] = *cur_ptr++;
			// line��'\0'��β
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

// ʹ��fread��ȡ����
void fread_file(std::string& file_name) {

	// ��д���ļ�
	write_file_open();
	// �򿪶����ļ� 'rb'ģʽ��ȡ�Ļ��з���Դ�ļ��б���һ��
	//FILE* fp = nullptr;	
	FILE* fp = fopen(file_name.c_str(), "rb");
	// �ж϶��ļ��Ƿ��ʧ��
	if (fp == nullptr)
		std::cout << "open read file failed" << std::endl;

	//����Ҫ������һ��
	while (getc(fp) != '\n') {
	}

	char block[BLOCK_SIZE];
	size_t block_size = BLOCK_SIZE;

	int backwards = 0;		// ��Ҫ���˵��ֽ���
	int i = 0;
	auto t = clock();

	char temp[1];

	while (!feof(fp)) {
		// ͨ������ֵ�ж϶�������
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
			// �����дָ���Ƿ���ȷ�ػص�����
			fseek(fp, -1, SEEK_CUR);
			fread(temp, sizeof(char), 1, fp);
			if (temp[0] != '\n')
				std::cout << temp << std::endl;
		}

		if (i++ % 1000 == 0)
			std::cout << i / 1000 << std::endl;
	}
	std::cout << clock() - t << std::endl;
	// ���ֶ�����Ŀռ�free��
	free_block();
	// �ر�д�ļ���
	write_file_close();
	// �رն��ļ���
	fclose(fp);
	fp = nullptr;
}
