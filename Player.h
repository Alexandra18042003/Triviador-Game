#pragma once
#include <stdint.h>
#include <string>
#include "Region.h"
#include <vector>
class Player
{

public:
	enum class Color : uint8_t
	{
		Red,
		Yellow,
		Blue,
		Green
	};
	std::string colorToString(Color color);

public:
	Player();
	Player(std::string name, Color color, bool isReady);

public:
	std::vector <bool> GetAdvantages();
	std::string GetName();
	std::vector<Region>& GetRegions();
	int GetScore();
	Color GetColor();
	bool GetReady();
	void SetAdvantageToUsed(const int& indexAdvantage);
	void SetReady();

public:
	int AddUpTotalScore();
	void DecrementScore();
	static Color SelectColor(int index);
	Region& MoveRegion(int row, int column, std::string color);
	void AddRegions(Region& region);
	
private:
	std::string m_nume;
	std::vector <Region> m_regions;
	int m_score;
	Color m_color;
	int m_numericAnswerOrIndex;
	bool m_isReady;
	std::vector <bool> m_avantages{ false,false,false }; 
	int counterAdvantage = 0;
};