#include "pch.h"
#include <iostream>

using namespace std;

#define F32 float
#define U32 unsigned int

F32 lowPassFilter(F32 unfilteredInput,F32 lastFramesFilteredInput,F32 rc,F32 dt) { // f(t) = (1 - a)f(t-t')+au(t)��ɢ��ͨ�˲���
	F32 a = dt / (rc + dt);
	return (1 - a)*lastFramesFilteredInput + a * unfilteredInput;
}

template< typename TYPE, int SIZE> // �ƶ�ƽ��
class MovingAverage {
	TYPE m_samples[SIZE];
	TYPE m_sum;
	U32 m_curSample;
	U32 m_sampleCount;

public:
	MovingAverage() :
		m_sum(static_cast<TYPE>(0)),
		m_curSample(0), // ��ǰ�±꣬��n��0
		m_sampleCount(0) {} // ��Χ0-SIZE

	void addSample(TYPE data) {
		if (m_sampleCount == SIZE) { // ����ѭ������һ�����ظ�
			m_sum -= m_samples[m_curSample];
		}
		else {
			++m_sampleCount;
		}
		m_samples[m_curSample] = data;
		m_sum += data;
		++m_curSample;
		if (m_curSample >= SIZE) {
			m_curSample = 0;
		}
	}

	F32 getCurrentAverage() const {
		if (m_sampleCount != 0) {
			return static_cast<F32>(m_sum) / static_cast<F32>(m_sampleCount);
		}
		return 0.0f;
	}

};
