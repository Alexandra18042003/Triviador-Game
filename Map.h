#pragma once
#include "Region.h"
#include <vector>
#include <algorithm>

class Map
{

public:
	Map();
	Map(uint8_t numberOfPlayers);

public:
	Region& GetRegion(uint8_t row, uint8_t column);
	std::vector<std::vector<Region>>& GetMapRegions();

public:
	bool VerifyNeighbor(std::string color, uint8_t row, uint8_t column);
	bool VerifyUnoccupiedRegions();

private:
	std::size_t m_width;
	std::size_t m_height;
	std::vector<std::vector<Region>> m_map;
};

