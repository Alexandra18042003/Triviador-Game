#pragma once
#include <string>
#include<iostream>

class Region
{
public:
	Region();
	Region(int row, int column, std::string color, bool isBase, int score);
	Region(Region&& region);
	Region(const Region& region);

public:
	void SetAtributes(std::string color, bool isBase, int score);
	void SetScore(int score);

public:
	int GetScore();
	int GetRow();
	int GetColumn();
	std::string GetColor();

public:
	void ChangeColor(std::string color);
	void IncreaseScore();
	
public:
	Region& operator=(Region&& region);
	Region& operator=(const Region& region);

private:
	int m_score;
	std::string m_color;
	bool m_isBase;
	int m_row, m_column;
};

