#include "ExecutionTime.h"

ExecutionTime::ExecutionTime() {}

void ExecutionTime::SetStartTime()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

void ExecutionTime::AddStopTime(const uint8_t& id)
{
	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
	m_stopTimes.push_back(std::make_pair(id, stop));

}

std::chrono::microseconds ExecutionTime::GetDurationTime(const std::chrono::high_resolution_clock::time_point& start, const std::chrono::high_resolution_clock::time_point& stop)
{
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	return duration;
}

void ExecutionTime::ClearTimer()
{
	m_stopTimes.clear();
}

std::vector<int> ExecutionTime::GetResponseOrder()
{
	std::vector<std::pair< uint8_t, std::chrono::microseconds>> timeVector;
	for (int i = 0; i < m_stopTimes.size(); i++)
	{
		timeVector.push_back(std::make_pair(m_stopTimes[i].first, GetDurationTime(m_startTime, m_stopTimes[i].second)));
	}
	auto Comp = [&](std::pair< uint8_t, std::chrono::microseconds> a, std::pair< uint8_t, std::chrono::microseconds>b)-> bool {

		return a.second > b.second;
	};
	std::sort(timeVector.begin(), timeVector.end(), Comp);
	std::vector<int> result;
	for (int i = 0; i < timeVector.size(); i++)
	{
		result.push_back(timeVector[i].first);
	}

	return result;
}