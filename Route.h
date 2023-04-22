#pragma once
#include <filesystem>
#include <iostream>
#include <memory>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;
#include "Database.h"
#include "Game.h"
#include "Player.h"
#include "DataBaseQuestions.h"
#include "../ExecutionTime/ExecutionTime.h"
#include"Region.h"

class Route
{
public:
	Route();

public:
	crow::SimpleApp& GetApp();
	void Login(Database& database, int& indexColorPlayer, std::vector<Player>& players);
	void Avantages(Database& database, Game& Triviador, std::vector<Question>& gameQuestionsGrids, std::vector<Question>& gameQuestionsNumeric);
	void SendingQuestions(Database& database, Question& q, ExecutionTime& timer, std::vector<Question>& gameQuestionsGrids, std::vector<Question>& gameQuestionsNumeric, std::multimap<int, Player>& playersOrder );;
	void SetPlayers(Question& q, std::multimap<int, Player>& playersOrder, int& activePlayers, std::vector<Question>& gameQuestionsNumeric, Game& Triviador, ExecutionTime& timer, Database& database, int& count, std::vector<std::pair<std::string, int>>& playersNameandAnswer);
	void RegionRelated(Game& Triviador, std::multimap<int, Player>& playersOrder);
	void WaitingAndReady(std::multimap<int, Player>& playersOrder, std::vector<Player>& players, int& activePlayers, Game& Triviador, std::vector<Question>& gameQuestionsNumeric, std::vector<Question>& gameQuestionsGrids, Database& database);
	void DuelRelated(std::vector<Player>& players, Game& Triviador, std::string& nameOfAttackedPlayer, std::string& copyNameOfAttackedPlayer, int& duelPlayers, int& waitingForPlayersAnswer, std::multimap<int, Player>& playersOrder, std::vector<Question>& gameQuestionsGrids, int& endOfRound1, int& inc, std::string& colorMove, int& syncEndRound, std::vector<Question>& gameQuestionsNumeric);

private:
	crow::SimpleApp m_app;
};

