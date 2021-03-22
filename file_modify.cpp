/**
 * 文件修改
 * 1.文件按字段排序
 * 2.文件字段转换(时间转换为时间戳)
 */

#include "file_modify.h"
#include <iostream>

// 按行读文件
void read_file(std::string &file_name) {
    FILE* fp = nullptr;
    fp = fopen(file_name.c_str(), "rb");
    if (fp == nullptr)
        std::cout << "open read file failed" << std::endl;
    fp = nullptr;
}

