#include "Game.h"


Game::Game()
{
}

uint8_t Game::GetNumberOfRounds()
{
	return m_numberOfRounds;
}

std::vector<Player>& Game::GetPlayers()
{
	return m_players;
}

Map& Game::GetMap()
{
	return m_mapTriviador;
}

void Game::SetNumberOfPlayers(uint8_t number)
{
	m_numberOfPlayers = number;
}

void Game::SetNumberOfRounds()
{
	if (m_numberOfPlayers == 2 || m_numberOfPlayers == 4)
		m_numberOfRounds = 5;
	else m_numberOfRounds = 4;
}

void Game::SetVectorOfPlayers(std::vector<Player> players)
{
	m_players = players;
}

void Game::SetMap(Map Triviador)
{
	m_mapTriviador = Triviador;
}

void Game::CreateGame(std::vector<Player>& players)
{
	this->SetNumberOfPlayers(players.size());
	this->SetNumberOfRounds();
	this->SetVectorOfPlayers(players);
	this->SetMap(Map(players.size()));
}

int Game::GenerateOneSimilarAnswer(const int& answer)
{
	int low = answer - 10;
	int high = answer + 10;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

std::vector<Player> Game::OrderOfWinners()
{
	auto comp = [&](Player a, Player b)-> bool {

		return a.GetScore() > b.GetScore();
	};
	std::sort(m_players.begin(), m_players.end(), comp);
	return m_players;

}
		
std::vector<Question> Game::GenerateGameQuestions(int& activePlayers, Database& database, char c)
{
	int numberOfQuestions;
	std::vector<Question> questions;

	if (activePlayers == 2)
		numberOfQuestions = 17;
	else if (activePlayers == 3) numberOfQuestions = 16;
	else if (activePlayers == 4) numberOfQuestions = 24;

	for (int i = 0; i < numberOfQuestions; i++)
		questions.push_back(database.GenerateRandomQuestion(c));
	database.UpdateDataBase();

	return questions;
}

void Game::ShufflePlayers(std::vector<Player>& players)
{
	auto rng = std::default_random_engine{ std::random_device{}() };
	std::shuffle(std::begin(players), std::end(players), rng);
}

std::vector<int> Game::GenerateSimilarAnswers(const int& answer)
{
	std::vector<int> numbers;
	auto random = [&](int low, int high)->int
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(low, high);
		return dist(gen);
	};

	for (int i = 0; i < 3; i++) {
		numbers.push_back(random(answer - 50, answer + 50));
	}
	return numbers;

}

