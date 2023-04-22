#include "Question.h"

std::string Question::GetQuestion() const
{
	return m_question;
}

std::vector<std::string>& Question::GetGrids()
{
	return m_grids;
}

int Question::GetNumericAnswer() const
{
	return m_answerOrIndex;
}