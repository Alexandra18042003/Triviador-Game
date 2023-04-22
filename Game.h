#pragma once
#include <vector>
#include "Player.h"
#include <stdint.h>
#include <iostream>
#include "QuestionManager.h"
#include "Map.h"
#include "Player.h"
#include <map>
#include"Question.h"
#include"Database.h"
class Game
{

public:
	Game();

public:
	uint8_t GetNumberOfRounds();
	std::vector<Player>& GetPlayers();
	Map& GetMap();

public:
	void SetNumberOfPlayers(uint8_t number);
	void SetNumberOfRounds();
	void SetVectorOfPlayers(std::vector<Player> players);
	void SetMap(Map Triviador);
	
public:
	void CreateGame(std::vector<Player>& players);
	std::vector <int> GenerateSimilarAnswers(const int& answer);
	int GenerateOneSimilarAnswer(const int& answer);
	std::vector<Player> OrderOfWinners();
	std::vector<Question> GenerateGameQuestions(int& activePlayers, Database& database, char c);
	void ShufflePlayers(std::vector<Player>& players);

private:
	uint8_t m_numberOfPlayers;
	uint8_t m_numberOfRounds;
	std::vector<Player> m_players;
	QuestionManager m_questionManager;
	Map m_mapTriviador;
};
