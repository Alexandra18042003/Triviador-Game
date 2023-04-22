#pragma once
#include <utility>
#include <chrono>
#include <vector>
#include <algorithm>
#ifdef EXECUTIONTIME_EXPORTS
#define EXECUTIONTIME_API _declspec(dllexport)
#else
#define EXECUTIONTIME_API _declspec(dllimport)
#endif 

class EXECUTIONTIME_API ExecutionTime
{
public:
	ExecutionTime();

public:
	void ClearTimer();
	void SetStartTime();
	void AddStopTime(const uint8_t& id);
	std::chrono::microseconds GetDurationTime(const std::chrono::high_resolution_clock::time_point& start, const  std::chrono::high_resolution_clock::time_point& stop);
	std::vector<int> GetResponseOrder();

private:
	std::chrono::high_resolution_clock::time_point m_startTime;
	std::vector <std::pair<uint8_t, std::chrono::high_resolution_clock::time_point>> m_stopTimes;

};