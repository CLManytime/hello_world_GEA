#include "pch.h"
#include <iostream>

using namespace std;

#define U32 unsigned int

bool syncReadFile(const char* filePath, U32* buffer, size_t bufferSize, size_t& rBytesRead) { // 同步文件I/O(我猜游戏开始是的载入指的就是这个)
	FILE* handle = fopen(filePath, "rb"); // 只读二进制文件
	if (handle) {
		rsize_t bytesRead = fread(buffer, 1, bufferSize, handle); // 在此等待数据读完
		int err = ferror(handle); // 取得错误码，判断是否出错
		if (0 == err) { // 无错误
			rBytesRead = bytesRead;
			return true;
		}
	}
	return false;
}

void main() {
	U32 testBuffer[512];
	size_t bytesRead = 0;
	if (syncReadFile("testfile.bin",testBuffer,sizeof(testBuffer),bytesRead)) { // 相对位置,当前目录下
		printf("success: read %u bytes\n", bytesRead);
	}
}