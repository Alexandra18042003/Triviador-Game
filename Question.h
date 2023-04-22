#pragma once
#include <iostream>
#include <string>
#include <vector>


struct Question
{
public:
	std::string GetQuestion() const;
	std::vector<std::string>& GetGrids();
	int GetNumericAnswer() const;

public:
	int m_id;
	std::string m_question;
	int m_answerOrIndex;
	std::vector<std::string> m_grids;
};

