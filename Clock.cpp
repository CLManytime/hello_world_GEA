#include "pch.h"
#include <iostream>

using namespace std;

#define U64 unsigned long long
#define F32 float

class Clock {
	U64 m_timeCycles; //ʱ��������
	F32 m_timeScale; // ����
	bool m_isPaused; // ��ͣ
	static F32 s_cyclesPerSecond; // ÿ��������

	static inline U64 secondsToCycle(F32 timeSeconds) { // ת��������
		return timeSeconds * s_cyclesPerSecond;
	}

	static inline U64 cyclesToSeconds(U64 timeCycles) { // ת������
		return (U64)(timeCycles / s_cyclesPerSecond);
	}

public:

	static F32 readHiResTimerFrequency() {};

	static void init() {
		s_cyclesPerSecond = (F32)readHiResTimerFrequency(); // ��ȡÿ��������
	}

	explicit Clock(F32 startTimeSeconds = 0.0f) : // ��ʽ����
		m_timeCycles(secondsToCycle(startTimeSeconds)),
		m_timeScale(1.0f),
		m_isPaused(false) {}

	U64 getTimeCycles() const {
		return m_timeCycles;
	}

	F32 calcDeltaSeconds(const Clock& other) { // ��������һʱ�ӵľ���ʱ���(��)
		U64 dt = m_timeCycles - other.m_timeCycles;
		return cyclesToSeconds(dt);
	}

	void update(F32 dtRealSeconds) { // ÿ֡���У�������ʵ����֡ʱ��
		if (!m_isPaused) {
			U64 dtScaledCycles = secondsToCycle(dtRealSeconds*m_timeScale); // ��ȡ����ʱ��������
			m_timeCycles += dtScaledCycles;
		}
	}

	void setPaused(bool isPaused) {
		m_isPaused = isPaused;
	}

	bool isPaused() const {
		return m_isPaused;
	}

	void setTimeScale(F32 scale) {
		m_timeScale = scale;
	}

	F32 getTimeScale() const {
		return m_timeScale;
	}

	void singleStep() { // ���ڵ��ԣ���������֡ʱ��
		if (m_isPaused) {
			U64 dtScaledCycles = secondsToCycle((1.0f / 30.0f)*m_timeScale);
			m_timeCycles += dtScaledCycles;
		}
	}

};