#include "pch.h"
#include <iostream>
#include <stdio.h>
#ifndef WIN32_LEAN_AND_MEAN //用于防止多跟文件时声明冲突
#define WIN32_LEAN_AND_MEAN //加速windows.h的生成
#endif // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#define U32 unsigned int

using namespace std;

int  VDebugPrintF(const char* format, va_list argList) { // 解决变参问题（参数个数）,指向参数的指针
	const U32 MAX_CHARS = 1023;
	static char s_buffer[MAX_CHARS + 1];
	int charsWrittrn = vsnprintf(s_buffer, MAX_CHARS, format, argList); // 此函数用于变参问题，存在s_buffer中，最大字节MAX_CHARS,格式format，变参
	s_buffer[MAX_CHARS] = '\0'; // 确保空字符结尾
	OutputDebugStringA(s_buffer); // 原书使用的OutputDebugString因格式不同无法使用
	return charsWrittrn;
}

int DebugPrintf(const char* format, ...) {
	va_list argList;
	va_start(argList, format); // 第二个参数是...前紧接着的参数
	int charsWritten = VDebugPrintF(format, argList);
	va_end(argList);
	return charsWritten;
}