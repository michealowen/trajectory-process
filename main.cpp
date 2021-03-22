/**
* 主程序
*/

#include <iostream>
#include <string.h>
#include <math.h>
#include "read_write.h"

int main() {

	//std::string file_name("tenMillion_0.txt");
	//std::string file_name("new.txt");
	std::string file_name("E:\\郑州电动车数据\\trackdata\\trackdata.txt");
	//std::string file_name("E:\\郑州电动车数据\\trackdata\\billion\\billion_10.txt");
	fread_file(file_name);
	//std::cout << (int)ceil(log(500) / log(10)) << std::endl;
}