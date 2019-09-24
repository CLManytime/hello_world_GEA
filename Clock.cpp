#include "pch.h"
#include <iostream>

using namespace std;

#define U64 unsigned long long
#define F32 float

class Clock {
	U64 m_timeCycles; //时钟周期数
	F32 m_timeScale; // 缩放
	bool m_isPaused; // 暂停
	static F32 s_cyclesPerSecond; // 每秒周期数

	static inline U64 secondsToCycle(F32 timeSeconds) { // 转换至周期
		return timeSeconds * s_cyclesPerSecond;
	}

	static inline U64 cyclesToSeconds(U64 timeCycles) { // 转换至秒
		return (U64)(timeCycles / s_cyclesPerSecond);
	}

public:

	static F32 readHiResTimerFrequency() {};

	static void init() {
		s_cyclesPerSecond = (F32)readHiResTimerFrequency(); // 读取每秒周期数
	}

	explicit Clock(F32 startTimeSeconds = 0.0f) : // 显式构造
		m_timeCycles(secondsToCycle(startTimeSeconds)),
		m_timeScale(1.0f),
		m_isPaused(false) {}

	U64 getTimeCycles() const {
		return m_timeCycles;
	}

	F32 calcDeltaSeconds(const Clock& other) { // 计算与另一时钟的绝对时间差(秒)
		U64 dt = m_timeCycles - other.m_timeCycles;
		return cyclesToSeconds(dt);
	}

	void update(F32 dtRealSeconds) { // 每帧运行，给予真实量度帧时间
		if (!m_isPaused) {
			U64 dtScaledCycles = secondsToCycle(dtRealSeconds*m_timeScale); // 获取运行时钟周期数
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

	void singleStep() { // 用于调试，加上理想帧时间
		if (m_isPaused) {
			U64 dtScaledCycles = secondsToCycle((1.0f / 30.0f)*m_timeScale);
			m_timeCycles += dtScaledCycles;
		}
	}

};