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
#include "Route.h"

int main()
{
	Route route;
	Database database;
	database.PopulateStorage();
	database.PopulateQuestionsStorage();
	crow::SimpleApp& app = route.GetApp();
	Game Triviador;
	ExecutionTime timer;
	std::multimap<int, Player> playersOrder;
	std::vector<std::pair<std::string, int>>playersNameandAnswer;
	std::vector<Question> gameQuestionsNumeric;
	std::vector<Question> gameQuestionsGrids;
	std::vector<Player> players;
	Question q = database.GenerateRandomQuestion('N');
	std::string nameOfAttackedPlayer, copyNameOfAttackedPlayer, colorMove;
	int activePlayers = 0;
	int inc = 0;
	int count = 0;
	int indexColorPlayer = 0;
	int duelPlayers = 0, waitingForPlayersAnswer = 0, endOfRound1 = 0, syncEndRound = 0;

	route.Login(std::ref(database),std::ref(indexColorPlayer),std::ref(players));
	route.Avantages(std::ref(database), std::ref(Triviador), std::ref(gameQuestionsGrids), std::ref(gameQuestionsNumeric));
	route.SendingQuestions(std::ref(database), std::ref(q), std::ref(timer), std::ref(gameQuestionsGrids), std::ref(gameQuestionsNumeric), std::ref(playersOrder));
	route.SetPlayers(std::ref(q), std::ref(playersOrder), std::ref(activePlayers), std::ref(gameQuestionsNumeric), std::ref(Triviador), std::ref(timer), std::ref(database),std::ref(count), std::ref(playersNameandAnswer));
	route.RegionRelated(std::ref(Triviador), std::ref(playersOrder));
	route.WaitingAndReady(std::ref(playersOrder), std::ref(players), std::ref(activePlayers), std::ref(Triviador), std::ref(gameQuestionsNumeric), std::ref(gameQuestionsGrids), std::ref(database));
	route.DuelRelated(std::ref(players), std::ref(Triviador), std::ref(nameOfAttackedPlayer), std::ref(copyNameOfAttackedPlayer), std::ref(duelPlayers), std::ref(waitingForPlayersAnswer), std::ref(playersOrder), std::ref(gameQuestionsGrids), std::ref(endOfRound1), std::ref(inc), std::ref(colorMove), std::ref(syncEndRound), std::ref(gameQuestionsNumeric));
	
	
	app.port(18080).multithreaded().run();
}