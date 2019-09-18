/*内存分配器实现传回对齐的内存块时，先分配比请求多一点的内存，再上调内存地址至对齐。
先用掩码把内存地址的最低有效位取出，再把期望的对齐减去此值，就为偏移量。
我们可以把偏移量存在额外本用于对齐的内存中，以便释放。*/

#include "pch.h"
#include <iostream>

using namespace std;

#define U32 unsigned int

void* allocateUnaligned(U32 size_bytes);
void freeUnaligned(void* Address);

void* allocateAligned(U32 size_bytes, U32 alignment) { // 需要内存与额外分配的
	U32 expandedSize_bytes = size_bytes + alignment;
	U32 rawAddress = (U32)allocateUnaligned(expandedSize_bytes); // 分配未对齐内存（暂时不会实现（尴尬））
	U32 mask = (alignment - 1); // 掩码为额外内存-1
	U32 misalignment = (rawAddress&mask); // 错位量
	U32 adjustment = alignment - misalignment; // 调整量
	U32 alignedAddress = rawAddress + adjustment; // 对齐后的地址
	U32* pAdjustment = (U32*)(alignedAddress - 4); // 向前空的内存跳4个字节去存储调整量（4是因为4字节足够了）
	*pAdjustment = adjustment; // 存储调整量以便释放
	return (void*)alignedAddress;
}

void freeAligned(void* p) { // 释放对齐的空间
	U32 alignedAddress = (U32)p;
	U32* pAdjustment = (U32*)(alignedAddress - 4); // 得到存在前面空间里的调整量的地址
	U32 adjustment = (U32)*pAdjustment; // 取出调整量
	U32 rawAddress = alignedAddress - adjustment; // 未调整的地址
	freeUnaligned((void*)rawAddress); // 也并不会实现（哭泣）
}