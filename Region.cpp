#include "Region.h"

Region::Region()
{
}

Region::Region(int row, int column, std::string color, bool isBase, int score)
	:m_row(row),
	m_column(column),
	m_color(color),
	m_isBase(isBase),
	m_score(score)
{
}

Region::Region(Region&& region)
{
	*this = std::move(region);
}

Region::Region(const Region& region)
{
	*this = region;
}

void Region::SetAtributes(std::string color, bool isBase, int score) {
	m_color = color;
	m_isBase = isBase;
	m_score= score;
}

void Region::SetScore(int score)
{
	m_score = score;
}

int Region::GetScore()
{
	return m_score;
}

int Region::GetRow()
{
	return m_row;
}

int Region::GetColumn()
{
	return m_column;
}

std::string Region::GetColor()
{
	return m_color;
}

void Region::ChangeColor(std::string color)
{
	m_color = color;
}

void Region::IncreaseScore()
{
	m_score += 100;
}

Region& Region::operator=(Region&& region)
{
	m_color = region.m_color;
	m_score = region.m_score;
	m_isBase = region.m_isBase;
	m_row = region.m_row;
	m_column = region.m_column;
	new (&region) Region;
	return *this;
}

Region& Region::operator=(const Region& region)
{
	m_color = region.m_color;
	m_score = region.m_score;
	m_isBase = region.m_isBase;
	m_row = region.m_row;
	m_column = region.m_column;
	return *this;
}

