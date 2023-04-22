#include "QuestionManager.h"
#include <fstream>
#include <random>

void QuestionManager::ReadGridsFromFile()
{
	std::ifstream f("Grids.txt");
	std::string question;
	std::vector<std::string> m_grid;
	int indexGrid;
	std::string grid;

	while (getline(f, question))
	{
		f >> indexGrid;
		for (int i = 0; i < 5; i++)
		{
			getline(f, grid);
			m_grid.emplace_back(grid);
		}
		m_g.emplace_back(Question{ id, question, indexGrid, m_grid });
		m_grid.clear();
		id++;
	}
	f.close();

}

void QuestionManager::ReadNumericFromFile()
{
	std::ifstream g("Numeric.txt");
	std::string question;
	int number;

	while (getline(g, question))
	{
		g >> number;
		g.ignore();
		m_n.emplace_back(Question{ id, question, number });
		id++;
	}
	g.close();
}
