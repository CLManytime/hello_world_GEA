// GEA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <xmmintrin.h>

using namespace std;

__m128 addWithAssembly(__m128 a,__m128 b) { //运用内联汇编实现sse运算
	__m128 r;
	__asm {
		movaps xmm0, xmmword ptr [a] //将地址a里的数据读入寄存器中
		movaps xmm1, xmmword ptr [b]
		addps xmm0, xmm1 //相加结果再xmm0中
		movaps xmmword ptr [r], xmm0
	}
	return r;
}

__m128 addWithIntrinsics(__m128 a, __m128 b) { //应用内部函数实现see运算
	__m128 r = _mm_add_ps(a, b);
	return r;
}

__declspec(align(16)) float A[] = { 2.0f,-1.0f,3.0f,4.0f };//强制16字节对其（16*8 = 128）
__declspec(align(16)) float B[] = { -1.0f,3.0f,4.0f,2.0f };
__declspec(align(16)) float C[] = { 0.0f,0.0f,0.0f,0.0f };
__declspec(align(16)) float D[] = { 0.0f,0.0f,0.0f,0.0f };

int main() {
	__m128 a = _mm_load_ps(&A[0]);//将float载入a
	__m128 b = _mm_load_ps(&B[0]);
	__m128 c = addWithAssembly(a, b);
	__m128 d = addWithIntrinsics(a, b);
	_mm_store_ps(&A[0], a);//存回
	_mm_store_ps(&B[0], b);
	_mm_store_ps(&C[0], c);
	_mm_store_ps(&D[0], d);
	printf("%g %g %g %g\n", A[0], A[1], A[2], A[3]);
	printf("%g %g %g %g\n", B[0], B[1], B[2], B[3]);
	printf("%g %g %g %g\n", C[0], C[1], C[2], C[3]);
	printf("%g %g %g %g\n", D[0], D[1], D[2], D[3]);
	return 0;
}