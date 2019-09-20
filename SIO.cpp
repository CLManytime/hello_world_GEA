#include "pch.h"
#include <iostream>

using namespace std;

#define U32 unsigned int

bool syncReadFile(const char* filePath, U32* buffer, size_t bufferSize, size_t& rBytesRead) { // ͬ���ļ�I/O(�Ҳ���Ϸ��ʼ�ǵ�����ָ�ľ������)
	FILE* handle = fopen(filePath, "rb"); // ֻ���������ļ�
	if (handle) {
		rsize_t bytesRead = fread(buffer, 1, bufferSize, handle); // �ڴ˵ȴ����ݶ���
		int err = ferror(handle); // ȡ�ô����룬�ж��Ƿ����
		if (0 == err) { // �޴���
			rBytesRead = bytesRead;
			return true;
		}
	}
	return false;
}

void main() {
	U32 testBuffer[512];
	size_t bytesRead = 0;
	if (syncReadFile("testfile.bin",testBuffer,sizeof(testBuffer),bytesRead)) { // ���λ��,��ǰĿ¼��
		printf("success: read %u bytes\n", bytesRead);
	}
}