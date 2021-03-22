#ifndef _READ_WRITE_ 
#define _READ_WRITE_

#define LINE_SIZE 200			// 行数组大小
//#define BLOCK_SIZE 5000000		// 读写的块大小 5,000,000字节 约5MB
#define BLOCK_SIZE 500000
#define MAP_SIZE 500			// 映射表的大小(桶数量)

// 使用fread读取数据
void fread_file(std::string& file_name);

#endif // !_READ_WRITE_ 

