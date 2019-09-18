/*�ڴ������ʵ�ִ��ض�����ڴ��ʱ���ȷ���������һ����ڴ棬���ϵ��ڴ��ַ�����롣
����������ڴ��ַ�������Чλȡ�����ٰ������Ķ����ȥ��ֵ����Ϊƫ������
���ǿ��԰�ƫ�������ڶ��Ȿ���ڶ�����ڴ��У��Ա��ͷš�*/

#include "pch.h"
#include <iostream>

using namespace std;

#define U32 unsigned int

void* allocateUnaligned(U32 size_bytes);
void freeUnaligned(void* Address);

void* allocateAligned(U32 size_bytes, U32 alignment) { // ��Ҫ�ڴ����������
	U32 expandedSize_bytes = size_bytes + alignment;
	U32 rawAddress = (U32)allocateUnaligned(expandedSize_bytes); // ����δ�����ڴ棨��ʱ����ʵ�֣����Σ���
	U32 mask = (alignment - 1); // ����Ϊ�����ڴ�-1
	U32 misalignment = (rawAddress&mask); // ��λ��
	U32 adjustment = alignment - misalignment; // ������
	U32 alignedAddress = rawAddress + adjustment; // �����ĵ�ַ
	U32* pAdjustment = (U32*)(alignedAddress - 4); // ��ǰ�յ��ڴ���4���ֽ�ȥ�洢��������4����Ϊ4�ֽ��㹻�ˣ�
	*pAdjustment = adjustment; // �洢�������Ա��ͷ�
	return (void*)alignedAddress;
}

void freeAligned(void* p) { // �ͷŶ���Ŀռ�
	U32 alignedAddress = (U32)p;
	U32* pAdjustment = (U32*)(alignedAddress - 4); // �õ�����ǰ��ռ���ĵ������ĵ�ַ
	U32 adjustment = (U32)*pAdjustment; // ȡ��������
	U32 rawAddress = alignedAddress - adjustment; // δ�����ĵ�ַ
	freeUnaligned((void*)rawAddress); // Ҳ������ʵ�֣�������
}