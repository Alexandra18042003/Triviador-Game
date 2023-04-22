#include "Player.h"

Player::Player()
{
}

Player::Player(std::string name, Color color , bool isReady) 
	:m_nume(name),
	m_color(color),
	m_isReady(isReady),
	m_score(0)
{
}

std::string Player::colorToString(Color color)
{
	std::string colorString;
	colorString = (static_cast<int>(color) == 0 ? "Red" : static_cast<int>(color) == 2 ? "Blue" : static_cast<int>(color) == 1 ? "Yellow" : "Green");
	return colorString;
}

Player::Color Player::SelectColor(int index)
{
	switch (index)
	{
	case 0:
		return Color::Red;
		break;
	case 1:
		return Color::Blue;
		break;
	case 2:
		return Color::Yellow;
		break;
	case 3:
		return Color::Green;
		break;
	}
}

std::string Player::GetName()
{
	return m_nume;
}

std::vector <bool> Player::GetAdvantages()
{
	return m_avantages;
}

void Player::SetAdvantageToUsed(const int& indexAdvantage)
{
	m_avantages[indexAdvantage] = 1;
}

std::vector<Region>& Player::GetRegions()
{
	return m_regions;
}

Region& Player::MoveRegion(int row, int column, std::string color)
{
	for (int i = 0; i < m_regions.size(); i++)
		if (m_regions[i].GetRow() == row && m_regions[i].GetColumn() == column)
		{
			m_regions[i].ChangeColor(color);
			m_regions[i].SetScore(100);
			auto& moveRegion = m_regions[i];
			m_regions.erase(m_regions.begin() + i);
			return moveRegion;
		}
}

int Player::GetScore()
{
	return m_score;
}

Player::Color Player::GetColor()
{
	return m_color;
}

bool Player::GetReady()
{
	return m_isReady;
}

void Player::SetReady()
{
	m_isReady = true;
}

void Player::AddRegions(Region& region)
{
	m_regions.push_back(region);
	m_score += region.GetScore();
}

void Player::DecrementScore() { m_score -= 100; }

int Player::AddUpTotalScore()
{
	int suma = 0;
	for (int i = 0; i < m_regions.size(); i++)
		suma += m_regions[i].GetScore();
	return suma;
}