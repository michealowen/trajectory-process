#ifndef _READ_WRITE_ 
#define _READ_WRITE_

#define LINE_SIZE 200			// �������С
//#define BLOCK_SIZE 5000000		// ��д�Ŀ��С 5,000,000�ֽ� Լ5MB
#define BLOCK_SIZE 500000
#define MAP_SIZE 500			// ӳ���Ĵ�С(Ͱ����)

// ʹ��fread��ȡ����
void fread_file(std::string& file_name);

#endif // !_READ_WRITE_ 

