#include "pch.h"
#include <iostream>
#include <stdio.h>
#ifndef WIN32_LEAN_AND_MEAN //���ڷ�ֹ����ļ�ʱ������ͻ
#define WIN32_LEAN_AND_MEAN //����windows.h������
#endif // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#define U32 unsigned int

using namespace std;

int  VDebugPrintF(const char* format, va_list argList) { // ���������⣨����������,ָ�������ָ��
	const U32 MAX_CHARS = 1023;
	static char s_buffer[MAX_CHARS + 1];
	int charsWrittrn = vsnprintf(s_buffer, MAX_CHARS, format, argList); // �˺������ڱ�����⣬����s_buffer�У�����ֽ�MAX_CHARS,��ʽformat�����
	s_buffer[MAX_CHARS] = '\0'; // ȷ�����ַ���β
	OutputDebugStringA(s_buffer); // ԭ��ʹ�õ�OutputDebugString���ʽ��ͬ�޷�ʹ��
	return charsWrittrn;
}

int DebugPrintf(const char* format, ...) {
	va_list argList;
	va_start(argList, format); // �ڶ���������...ǰ�����ŵĲ���
	int charsWritten = VDebugPrintF(format, argList);
	va_end(argList);
	return charsWritten;
}