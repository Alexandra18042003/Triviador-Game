#include "Map.h"
#include <iostream>

Map::Map(){}

Map::Map(uint8_t numberOfPlayers)
{
	if (numberOfPlayers == 2)
	{
		m_width = 3;
		m_height = 3;
	}
	else if (numberOfPlayers == 3)
	{
		m_width = 3;
		m_height = 5;
	}
	else if (numberOfPlayers == 4)
	{
		m_width = 4;
		m_height = 6;
	}
	m_map.resize(m_height);
	for (int i = 0; i < m_height; i++)
	{
		std::vector<Region> firstV;
		firstV.resize(m_width);
		for (int j = 0; j < m_width; j++)
		{
			Region reg(i, j, "____", false, 0);
			firstV[j] = reg;
		}
		m_map[i] = firstV;
	}
}

Region& Map::GetRegion(uint8_t row, uint8_t column)
{
	return m_map[row][column];
}

std::vector<std::vector<Region>>& Map::GetMapRegions()
{
	return m_map;
}

bool Map::VerifyUnoccupiedRegions()
{
	bool ok = true;
	for (std::size_t line = 0; line < m_height; line++)
		if (std::all_of(m_map[line].cbegin(), m_map[line].cend(), [](Region reg) { return reg.GetColor() != "____"; }))
			ok = false;
		else return true;
	return ok;
}

bool Map::VerifyNeighbor(std::string color, uint8_t row, uint8_t column)
{
	if (row != 0 && m_map[row - 1][column].GetColor() == color)
		return true;
	else if (row != m_height - 1 && m_map[row + 1][column].GetColor() == color)
		return true;
	else if (column != 0 && m_map[row][column - 1].GetColor() == color)
		return true;
	else if (column != m_width - 1 && m_map[row][column + 1].GetColor() == color)
		return true;

	return false;
}