#ifndef _READ_WRITE_ 
#define _READ_WRITE_

#define LINE_SIZE 200			// 行数组大小
//#define BLOCK_SIZE 5000000		// 读写的块大小 5,000,000字节 约5MB
#define BLOCK_SIZE 800000
#define MAP_SIZE 500			// 映射表的大小(桶数量)

/*
 * 打开写的文件
 */
void write_file_open();

/**
 * 关闭文件流
 */
void write_file_close();


/**
 * 使用fread读取数据
 */
void fread_file(std::string& file_name);

/**
 * 释放手动分配的内存
 */
void free_block();

/**
 * 将行写入数据块，被fread_line调用
 */
void fwrite_block(const char* line, const int key);

/**
 * 处理一行数据，数据以'/n'结尾
 */
void fread_line(char* line);

/**
 * 判断是否位于指定区域
 */
bool check_location();

/**
 * 解析数据块, 返回需要回退的字节数, 数据块的最后一个字符为'\0'
 */
int fread_block(char* block);

/**
 * 使用fread读取文件
 */
void fread_file(std::string& file_name);

#endif // !_READ_WRITE_ 
