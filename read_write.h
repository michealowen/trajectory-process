#ifndef _READ_WRITE_ 
#define _READ_WRITE_

#define LINE_SIZE 200			// 行数组大小
//#define BLOCK_SIZE 5000000		// 读写的块大小 5,000,000字节 约5MB
#define BLOCK_SIZE 800000
#define MAP_SIZE 500			// 映射表的大小(桶数量)

#include <stdio.h>
#include <iostream>
#include <unordered_map> 
#include <chrono>
#include <string.h>
#include <math.h>		// 用于计算id取后几位
#include <sstream>		// 用于字符串转数字

// 文件序号与文件标识符的映射, 如key:000, value:000.txt
extern std::unordered_map<int, FILE*> m;
// 全局数组,数组元素是块的指针
extern char* block[MAP_SIZE];
// 全局数组,数组元素是每个块字节数,写入前不能超过5,000,000字节,约5MB
extern int block_size[MAP_SIZE];
// 经度范围, 纬度范围
extern float max_lon, min_lon, max_lat, min_lat;
// id 字段取出的位数
extern int id_digits;

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
inline void free_block();

/**
 * 将行写入数据块，被fread_line调用
 */
void fwrite_block(const char* line, const int key);

/**
 * 处理一行数据，数据以'/n'结尾
 */
inline void fread_line(char* line);

/**
 * 判断是否位于指定区域
 * lon_begin_ptr: 经度起始的指针
 * lon_num: 经度所占的位数
 */
inline bool check_lon(char* lon_begin_ptr, int lon_num);

/**
 * 判断是否位于指定区域
 * lat_begin_ptr: 纬度起始的指针
 * lat_num: 纬度所占的位数
 */
inline bool check_lat(char* lat_begin_ptr, int lat_num);

/**
 * 检查数据来源, 跳过来源为'2'的数据
 */
bool check_data_source(char data_source);

/**
 * 解析数据块, 返回需要回退的字节数, 数据块的最后一个字符为'\0'
 */
int fread_block(char* block);

/**
 * 使用fread读取文件
 */
void fread_file(std::string& file_name);

#endif // !_READ_WRITE_ 
