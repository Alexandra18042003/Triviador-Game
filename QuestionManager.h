#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Question.h"

struct QuestionManager
{
public:
	QuestionManager() = default;

public:
	void ReadGridsFromFile();
	void ReadNumericFromFile();

public:
	int id = 1;
	std::vector<Question> m_g;
	std::vector<Question> m_n;
};