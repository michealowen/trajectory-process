/**
 * �ļ��޸�
 * 1.�ļ����ֶ�����
 * 2.�ļ��ֶ�ת��(ʱ��ת��Ϊʱ���)
 */

#include "file_modify.h"
#include <iostream>

// ���ж��ļ�
void read_file(std::string &file_name) {
    FILE* fp = nullptr;
    fp = fopen(file_name.c_str(), "rb");
    if (fp == nullptr)
        std::cout << "open read file failed" << std::endl;
    fp = nullptr;
}

