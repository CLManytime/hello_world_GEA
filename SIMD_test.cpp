// GEA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <xmmintrin.h>

using namespace std;

#define SHUFFLE_PARAM(x,y,z,w) ((x)|((y)<<2)|((z)<<4)|((w)<<6))
#define _mm_replicate_x_ps(v) _mm_shuffle_ps((v),(v),SHUFFLE_PARAM(0,0,0,0)) //将寄存器里的值全复制为x
#define _mm_replicate_y_ps(v) _mm_shuffle_ps((v),(v),SHUFFLE_PARAM(1,1,1,1))
#define _mm_replicate_z_ps(v) _mm_shuffle_ps((v),(v),SHUFFLE_PARAM(2,2,2,2))
#define _mm_replicate_w_ps(v) _mm_shuffle_ps((v),(v),SHUFFLE_PARAM(3,3,3,3))
#define _mm_madd_ps(a,b,c) _mm_add_ps(_mm_mul_ps((a),(b)),(c)) //乘并加

__m128 mulVectorMatrixFinal(__m128 v, __m128 Mrow1, __m128 Mrow2, __m128 Mrow3, __m128 Mrow4) {
	__m128 result;
	result = _mm_mul_ps(_mm_replicate_x_ps(v), Mrow1); //v[x]*M[1][1],v[x]*M[1][2],v[x]*M[1][3],v[x]*M[1][4]
	result = _mm_madd_ps(_mm_replicate_y_ps(v), Mrow2, result);//v[x]*M[1][1]+v[y]*M[2][1],v[x]*M[1][2]+v[y]*M[2][2],v[x]*M[1][3]+v[y]*M[2][3],v[x]*M[1][4]+v[y]*M[2][4]
	result = _mm_madd_ps(_mm_replicate_z_ps(v), Mrow3, result);
	result = _mm_madd_ps(_mm_replicate_w_ps(v), Mrow4, result);
	return result;
}

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
__declspec(align(16)) float E[] = { 0.0f,0.0f,0.0f,0.0f };
__declspec(align(16)) float V[] = { 2.0f,1.0f,3.0f,4.0f };

int main() {
	__m128 a = _mm_load_ps(&A[0]);//将float载入a
	__m128 b = _mm_load_ps(&B[0]);
	__m128 c = addWithAssembly(a, b);
	__m128 d = addWithIntrinsics(a, b);
	__m128 v = _mm_load_ps(&V[0]);
	__m128 e = mulVectorMatrixFinal(v, a, b, c, d);
	_mm_store_ps(&A[0], a);//存回
	_mm_store_ps(&B[0], b);
	_mm_store_ps(&C[0], c);
	_mm_store_ps(&D[0], d);
	_mm_store_ps(&V[0], v);
	_mm_store_ps(&E[0], e);
	printf("%g %g %g %g\n", A[0], A[1], A[2], A[3]);
	printf("%g %g %g %g\n", B[0], B[1], B[2], B[3]);
	printf("%g %g %g %g\n", C[0], C[1], C[2], C[3]);
	printf("%g %g %g %g\n", D[0], D[1], D[2], D[3]);
	printf("%g %g %g %g\n", V[0], V[1], V[2], V[3]);
	printf("%g %g %g %g\n", E[0], E[1], E[2], E[3]);
	return 0;
}
