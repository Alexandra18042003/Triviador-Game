#include "Route.h"

crow::SimpleApp& Route::GetApp() { return m_app; }

Route::Route() {}

void Route::Login(Database& database, int& indexColorPlayer, std::vector<Player>& players)
{
	CROW_ROUTE(m_app, "/users")([&database]()
	{
		std::vector<crow::json::wvalue> users_json;

		for (const auto& user : database.GetStorage().iterate<User>())
		{
			users_json.push_back(crow::json::wvalue{
				{ "id", user.id },
				{ "name", user.name },
				{ "password", user.password },
				{ "numberOfGamesPlayed", user.numberOfGamesPlayed },
				{ "NumberOfWinnings", user.NumberOfWinnings }
				});

		}
		return crow::json::wvalue{ users_json };
	});
	CROW_ROUTE(m_app, "/login")([&database, &indexColorPlayer, &players](const crow::request& req)
	{
		std::string username = req.url_params.get("name");
		std::string password = req.url_params.get("password");
		if (database.IsAlreadyTaken(username))

			if (database.IsPasswordValid(username, password))
			{
				Player player(username, Player::SelectColor(indexColorPlayer), false);
				players.push_back(player);
				indexColorPlayer++;

				return crow::response("ok");
			}
			else
			{
				return crow::response("invalidPassword");
			}

		return crow::response(200);
	});
	CROW_ROUTE(m_app, "/signup")([&database](const crow::request& req)
	{
		std::string name = req.url_params.get("name");
		std::string password = req.url_params.get("password");
		if (database.IsAlreadyTaken(name))
			return crow::response("taken");
		else
		{
			User user(-1, name, password, 0, 0);
			database.InsertIntoDatabase(user);
			return crow::response("ok");
		}
		return crow::response(200);
	});

	CROW_ROUTE(m_app, "/incrementGamesPlayed")([&database](const crow::request& req)
	{
		std::string name = req.url_params.get("name");
		database.IncrementGamesPlayed(name);
		return crow::response(200);
	});
}

void Route::Avantages(Database& database, Game& Triviador, std::vector<Question>& gameQuestionsGrids, std::vector<Question>& gameQuestionsNumeric)
 {
	 CROW_ROUTE(m_app, "/boolOfAvantages")([&Triviador](const crow::request& req)
	 {
		 std::string name = req.url_params.get("name");
		 int indexInt = std::stoi(req.url_params.get("index"));

		 for (int i = 0; i < Triviador.GetPlayers().size(); i++)
		 {
			 if (Triviador.GetPlayers()[i].GetName() == name)
			 {
				 Triviador.GetPlayers()[i].SetAdvantageToUsed(indexInt);
			 }
		 }
		 return crow::response(200);
	 });

	 CROW_ROUTE(m_app, "/availableAvantages")([&Triviador](const crow::request& req)
	 {
		 std::string name = req.url_params.get("name");
		 for (int i = 0; i < Triviador.GetPlayers().size(); i++)
		 {
			 if (Triviador.GetPlayers()[i].GetName() == name)
			 {
				 std::vector <bool> advantages = Triviador.GetPlayers()[i].GetAdvantages();
				 std::string cod = std::to_string(advantages[0]) + std::to_string(advantages[1]) + std::to_string(advantages[2]);
				 return crow::response(cod);
			 }
		 }
		 return crow::response(200);
	 });
	 CROW_ROUTE(m_app, "/avantage5050")([&gameQuestionsGrids, &Triviador, &database]()
	 {

		 int correctAnswer = gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_answerOrIndex;

		 std::vector <std::string> v = database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids();

		 v.erase(std::remove(v.begin(), v.end(), v[correctAnswer]), v.end());

		 std::random_device rd;
		 std::mt19937 g(rd());
		 std::shuffle(v.begin(), v.end(), g);
		 std::string wrongAnswer = v[0];
		 return crow::json::wvalue{
			 { "correctAnswer", database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids()[correctAnswer] },
			 { "wrongAnswer", wrongAnswer }
		 };
	 }
	 );
	 CROW_ROUTE(m_app, "/avantageNumeric")([&gameQuestionsNumeric, &Triviador]()
	 {
		 int answer = gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].m_answerOrIndex;
		 std::vector<int> answers = Triviador.GenerateSimilarAnswers(answer);
		 answers.push_back(answer);
		 auto rd = std::random_device{};
		 auto rng = std::default_random_engine{ rd() };
		 std::shuffle(std::begin(answers), std::end(answers), rng);
		 return crow::json::wvalue{
			 { "answer1",answers[0] },
			 { "answer2",answers[1] },
			 { "answer3",answers[2] },
			 { "answer4",answers[3] }
		 };

	 });

	 CROW_ROUTE(m_app, "/avantageSuggestion")([&gameQuestionsNumeric, &Triviador]()
	 {
		 int answer = gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].m_answerOrIndex;
		 int similarAnswer = Triviador.GenerateOneSimilarAnswer(answer);
		 return crow::json::wvalue{ { "similarAnswer",similarAnswer }
		 };
	 });
 }

void Route::SendingQuestions(Database& database, Question& q, ExecutionTime& timer, std::vector<Question>& gameQuestionsGrids, std::vector<Question>& gameQuestionsNumeric, std::multimap<int, Player>& playersOrder )
  {
	  CROW_ROUTE(m_app, "/questionChooseBase")([&q, &timer]()
	  {
		  timer.SetStartTime();
		  return crow::json::wvalue{
			  { "id", q.m_id },
			  { "question", q.m_question },
			  { "answerOrIndex", q.m_answerOrIndex }
		  };
	  });
	  CROW_ROUTE(m_app, "/getQuestionGrid")([&gameQuestionsGrids, &playersOrder, &database]()
	  {
		  return crow::json::wvalue
		  {
			  { "id", gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id },
			  { "question", gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_question },
			  { "answerOrIndex", gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_answerOrIndex },
			  { "answerGrids1", database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids()[1] },
			  { "answerGrids2", database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids()[2] },
			  { "answerGrids3", database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids()[3] },
			  { "answerGrids4", database.GetAllQuestionsVector()[gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_id - 1].GetGrids()[4] }
		  };
	  });

	  CROW_ROUTE(m_app, "/getQuestionNumeric")([&gameQuestionsNumeric, &playersOrder, &timer]()
	  {
		  timer.SetStartTime();
		  return crow::json::wvalue{
			  { "id", gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].m_id },
			  { "question", gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].m_question },
			  { "answerOrIndex", gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].m_answerOrIndex }
		  };
	  });
	  CROW_ROUTE(m_app, "/eraseQuestion/numeric")([&gameQuestionsNumeric]() { 
		  gameQuestionsNumeric.pop_back();
		  return crow::response(200);
	  });

	  CROW_ROUTE(m_app, "/eraseQuestion/grids")([&gameQuestionsGrids]() { 
		  gameQuestionsGrids.pop_back();
		  return crow::response(200);
	  });

  }

void Route::SetPlayers(Question& q, std::multimap<int, Player>& playersOrder, int& activePlayers, std::vector<Question>& gameQuestionsNumeric, Game& Triviador, ExecutionTime& timer, Database& database, int& count, std::vector<std::pair<std::string, int>>& playersNameandAnswer)
   {
	   CROW_ROUTE(m_app, "/setPlayersOrder")([&playersOrder](const crow::request& req) { 
		   std::string name = req.url_params.get("name");
		   if (playersOrder.begin()->second.GetName() == name)
			   return crow::response("ok");
		   else return crow::response("waiting");
		   return crow::response(200);
	   });
	   CROW_ROUTE(m_app, "/setPlayers/Regions")([&playersOrder, &activePlayers, &gameQuestionsNumeric, &Triviador, &timer, &database, &count, &playersNameandAnswer](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string answer = req.url_params.get("answer");
		   timer.AddStopTime(database.GetUserIdforPlayer(name));
		   int num = std::stoi(answer);
		   count++;

		   playersNameandAnswer.push_back(std::make_pair(name, abs(gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].GetNumericAnswer() - num)));

		   if (count == activePlayers)
		   {

			   for (int i = 0; i < playersNameandAnswer.size(); i++)
			   {
				   for (int j = i + 1; j < playersNameandAnswer.size(); j++)
				   {
					   if (playersNameandAnswer[i].second > playersNameandAnswer[j].second)
					   {
						   std::swap(playersNameandAnswer[i], playersNameandAnswer[j]);
					   }
					   else if (playersNameandAnswer[i].second == playersNameandAnswer[j].second)
					   {
						   for (int k = 0; k < timer.GetResponseOrder().size(); k++)
						   {
							   if (timer.GetResponseOrder()[k] == database.GetUserIdforPlayer(playersNameandAnswer[i].first))
							   {
								   std::swap(playersNameandAnswer[i], playersNameandAnswer[j]);
								   break;
							   }
							   else if (timer.GetResponseOrder()[k] == database.GetUserIdforPlayer(playersNameandAnswer[j].first))
							   {
								   break;
							   }
						   }

					   }

				   }
			   }

			   count = 0;
			   playersOrder.clear();
			   timer.ClearTimer();
			   for (int i = 0; i < playersNameandAnswer.size(); i++)
			   {
				   for (int j = 0; j < Triviador.GetPlayers().size(); j++)
				   {
					   if (Triviador.GetPlayers()[j].GetName() == playersNameandAnswer[i].first)
					   {
						  
						   playersOrder.insert(std::pair<int, Player>(i, Triviador.GetPlayers()[j]));
						   break;
					   }
				   }

			   }
			   playersNameandAnswer.clear();
		   }


		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/setPlayers/Base")([&playersOrder, &q, &activePlayers, &Triviador, &timer, &database, &count, &playersNameandAnswer](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string answer = req.url_params.get("answer");
		   int num = std::stoi(answer);
		   timer.AddStopTime(database.GetUserIdforPlayer(name));
		   count++;
		   playersNameandAnswer.push_back(std::make_pair(name, abs(q.GetNumericAnswer() - num)));

		   if (count == activePlayers)
		   {

			   for (int i = 0; i < playersNameandAnswer.size(); i++)
			   {
				   for (int j = i + 1; j < playersNameandAnswer.size(); j++)
				   {
					   if (playersNameandAnswer[i].second > playersNameandAnswer[j].second)
					   {
						   std::swap(playersNameandAnswer[i], playersNameandAnswer[j]);
					   }
					   else if (playersNameandAnswer[i].second == playersNameandAnswer[j].second)
					   {
						   for (int k = 0; k < timer.GetResponseOrder().size(); k++)
						   {
							   if (timer.GetResponseOrder()[k] == database.GetUserIdforPlayer(playersNameandAnswer[i].first))
							   {
								   std::swap(playersNameandAnswer[i], playersNameandAnswer[j]);
								   break;
							   }
							   else if (timer.GetResponseOrder()[k] == database.GetUserIdforPlayer(playersNameandAnswer[j].first))
							   {
								   break;
							   }
						   }

					   }

				   }
			   }
			   playersOrder.clear();
			   count = 0;
			   timer.ClearTimer();
			   for (int i = 0; i < playersNameandAnswer.size(); i++)
			   {
				   for (int j = 0; j < Triviador.GetPlayers().size(); j++)
				   {
					   if (Triviador.GetPlayers()[j].GetName() == playersNameandAnswer[i].first)
					   {
						   playersOrder.insert(std::pair<int, Player>(i, Triviador.GetPlayers()[j]));
						   break;
					   }
				   }
			   }
			   playersNameandAnswer.clear();
		   }


		   return crow::response(200);
	   });
   }

void Route::RegionRelated(Game& Triviador, std::multimap<int, Player>& playersOrder) {
	   CROW_ROUTE(m_app, "/printMap")([&Triviador]()
	   {
		   std::vector<crow::json::wvalue> regionsJson;
		   std::vector<std::vector<Region>> regions = Triviador.GetMap().GetMapRegions();

		   for (int i = 0; i < regions.size(); i++)
			   for (int j = 0; j < regions[i].size(); j++)
			   {
				   regionsJson.push_back(crow::json::wvalue{
					   { "color",regions[i][j].GetColor() }
					   });
			   }
		   return crow::json::wvalue{ regionsJson };
	   });


	   CROW_ROUTE(m_app, "/returnNumberOfRegToChoose")([&playersOrder](const crow::request& req) { 
		   if (!playersOrder.empty())
			   return crow::response(std::to_string(playersOrder.size() - 1));
		   else return crow::response(std::to_string(0));
		   return crow::response(200);
	   });


	   CROW_ROUTE(m_app, "/clientChooseBase")([&playersOrder, &Triviador](const crow::request& req) {
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");
		   std::string name = req.url_params.get("name");
		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
		   {
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   Triviador.GetMap().GetRegion(row, column).SetAtributes(Triviador.GetPlayers()[i].colorToString(Triviador.GetPlayers()[i].GetColor()), true, 300);
				   Triviador.GetPlayers()[i].AddRegions(Triviador.GetMap().GetRegion(row, column));
				   std::multimap<int, Player>::iterator firstPlayer = playersOrder.begin();
				   playersOrder.erase(firstPlayer);
				   break;
			   }
		   }
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/clientChooseRemainingRegions")([&Triviador](const crow::request& req) { 
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");
		   std::string name = req.url_params.get("name");
		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
		   {
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   Triviador.GetMap().GetRegion(row, column).SetAtributes(Triviador.GetPlayers()[i].colorToString(Triviador.GetPlayers()[i].GetColor()), false, 100);
				   Triviador.GetPlayers()[i].AddRegions(Triviador.GetMap().GetRegion(row, column));
				   break;
			   }
		   }
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkUnoccupiedRegions")([&Triviador]() {
		   if (Triviador.GetMap().VerifyUnoccupiedRegions())
			   return crow::response("Exists unoccupied regions");
		   else return crow::response("ok");
		   return crow::response(200);
	   });
   }

void Route::WaitingAndReady(std::multimap<int, Player>& playersOrder, std::vector<Player>& players, int& activePlayers, Game& Triviador, std::vector<Question>& gameQuestionsNumeric, std::vector<Question>& gameQuestionsGrids, Database& database) {
	   CROW_ROUTE(m_app, "/waiting")([&players]() {
		   for (int i = 0; i < players.size(); i++)
		   {
			   if (players[i].GetReady() == false)
				   return crow::response("waiting");
		   }
		   return crow::response("ok");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/ready")([&players, &activePlayers, &Triviador, &gameQuestionsNumeric, &gameQuestionsGrids, &database](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string state = req.url_params.get("state");

		   if (state == "ready")
		   {
			   for (int i = 0; i < players.size(); i++)
				   if (players[i].GetName() == name
					   && players[i].GetReady() == false)
				   {
					   players[i].SetReady();
					   activePlayers++;

					   if (activePlayers == players.size())
					   {
						   Triviador.CreateGame(players);
						   gameQuestionsNumeric = Triviador.GenerateGameQuestions(activePlayers, database, 'N');
						   gameQuestionsGrids = Triviador.GenerateGameQuestions(activePlayers, database, 'G');
						   
					   }

					   return crow::response("ok");
				   }
		   }
		   return crow::response(200);
	   });



	   CROW_ROUTE(m_app, "/waitingBase")([&activePlayers, &playersOrder]() { 
		   if (activePlayers == playersOrder.size())
			   return crow::response("ok");
		   return crow::response("waiting");
		   return crow::response(200);
	   });



	   CROW_ROUTE(m_app, "/waitingForPlayersToChoose")([&playersOrder]() { 
		   if (playersOrder.empty())
			   return crow::response("ok");
		   else return crow::response("waiting");
		   return crow::response(200);
	   });


	   CROW_ROUTE(m_app, "/erasePlayerFromOrderMap")([&playersOrder]() { 
		   if (!playersOrder.empty())
		   {
			   std::multimap<int, Player>::iterator firstPlayer = playersOrder.begin();
			   playersOrder.erase(firstPlayer);
		   }
		   return crow::response(200);
	   });





	   CROW_ROUTE(m_app, "/proba")([&Triviador](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   return crow::response(std::to_string(Triviador.GetPlayers()[i].AddUpTotalScore()));
			   }
		   return crow::response(200);
	   });//de sters?

	   CROW_ROUTE(m_app, "/return/numberOfRounds")([&Triviador]() {
		   return crow::response(std::to_string(Triviador.GetNumberOfRounds()));
	   });

   }

void Route::DuelRelated(std::vector<Player>& players, Game& Triviador, std::string& nameOfAttackedPlayer, std::string& copyNameOfAttackedPlayer, int& duelPlayers, int& waitingForPlayersAnswer, std::multimap<int, Player>& playersOrder, std::vector<Question>& gameQuestionsGrids, int& endOfRound1, int& inc, std::string& colorMove, int& syncEndRound, std::vector<Question>& gameQuestionsNumeric)
   {
	   CROW_ROUTE(m_app, "/shufflePlayers")([&players, &Triviador]()
	   {
		   if (players.empty())
		   {
			   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
				   players.push_back(Triviador.GetPlayers()[i]);
			   Triviador.ShufflePlayers(players);
		   }
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/clientsTurnToAttack")([&players](const crow::request& req) {
		   std::string name = req.url_params.get("name");

		   if (!players.empty() && players[players.size() - 1].GetName() == name)
			   return crow::response("ok");
		   else return crow::response("wait");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/verifyItIsNeigbor")([&players, &Triviador](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");
		   std::string color;
		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   color = Triviador.GetPlayers()[i].colorToString(Triviador.GetPlayers()[i].GetColor());
				   break;
			   }
		   if (Triviador.GetMap().VerifyNeighbor(color, row, column))
			   return crow::response("ok");
		   else return crow::response("Not neighbor");

		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/searchAttackedPlayer")([&Triviador, &nameOfAttackedPlayer, &copyNameOfAttackedPlayer](const crow::request& req) {
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");
		   std::string color;
		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   color = Triviador.GetMap().GetRegion(row, column).GetColor();
		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].colorToString(Triviador.GetPlayers()[i].GetColor()) == color)
			   {
				   nameOfAttackedPlayer = Triviador.GetPlayers()[i].GetName();
				   break;
			   }
		   if (!copyNameOfAttackedPlayer.empty())
			   copyNameOfAttackedPlayer.clear();
		   copyNameOfAttackedPlayer = nameOfAttackedPlayer;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/verifyItIsAttacked")([&nameOfAttackedPlayer](const crow::request& req) {
		   std::string name = req.url_params.get("name");

		   if (name == nameOfAttackedPlayer)
			   return crow::response("ok");
		   else return crow::response("wait");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/ready/prepareDuel")([&duelPlayers](const crow::request& req) {
		   std::string state = req.url_params.get("state");
		   if (state == "ready")
			   duelPlayers++;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/ready/duel")([&duelPlayers]() {
		   if (duelPlayers == 2)
		   {
			   return crow::response("ok");
		   }
		   else return crow::response("wait");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/removePlayerFromShuffle")([&players, &duelPlayers]() {
		   players.pop_back();
		   duelPlayers = 0;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/waitingForPlayersAnswer/increment")([&waitingForPlayersAnswer, &playersOrder, &Triviador, &gameQuestionsGrids](const crow::request& req) {
		   waitingForPlayersAnswer++;
		   std::string name = req.url_params.get("name");
		   std::string answer = req.url_params.get("answer");
		   int num = std::stoi(answer);
		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   if (gameQuestionsGrids[gameQuestionsGrids.size() - 1].m_answerOrIndex + 1 == num)
					   playersOrder.insert(std::pair<int, Player>(num, Triviador.GetPlayers()[i]));
				   break;
			   }
		   return crow::response(200);

	   });

	   CROW_ROUTE(m_app, "/waitingForPlayersAnswer")([&waitingForPlayersAnswer]() {
		   if (waitingForPlayersAnswer == 2)
		   {
			   return crow::response("ok");
		   }
		   else return crow::response("wait");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkWinner")([&playersOrder]() {
		   if (playersOrder.size() == 2)
			   return crow::response("Same answer");
		   else if (playersOrder.size() == 0) 
			   return crow::response("incorrect");
		   else if (playersOrder.size() == 1)
			   return crow::response(playersOrder.begin()->second.GetName());
	   });

	   CROW_ROUTE(m_app, "/decreaseScore")([&Triviador, &nameOfAttackedPlayer, &playersOrder](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");
		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   Triviador.GetPlayers()[i].DecrementScore();
			   }
			   else if (Triviador.GetPlayers()[i].GetName() == nameOfAttackedPlayer)
			   {
				   Triviador.GetMap().GetRegion(row, column).IncreaseScore();
			   }
		   playersOrder.clear();
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkEndOfRound")([&endOfRound1, &Triviador, &players]() {
		   if (endOfRound1 == Triviador.GetPlayers().size())
			   //SI AICI
			   return crow::response("ok");
		   else return crow::response("not over");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkEndOfRound/reset2")([&waitingForPlayersAnswer]() {
		   waitingForPlayersAnswer = 0;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkEndOfRound/reset")([&endOfRound1, &copyNameOfAttackedPlayer]() {
		   endOfRound1 = 0;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkEndOfRound/increase")([&endOfRound1]() {
		   endOfRound1++;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/checkEndOfRound/resetAttackedPlayer")([&inc, &playersOrder, &nameOfAttackedPlayer]() {
		   inc = 0;
		   nameOfAttackedPlayer.clear();
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/verifyLastAttack")([&players]() {
		   if (players.size() == 1)
			   return crow::response("ok");
		   else return crow::response("not ok");
		   return crow::response(200);
	   }); 

	   CROW_ROUTE(m_app, "/moveRegionToWinner")([&Triviador, &copyNameOfAttackedPlayer, &playersOrder, &colorMove](const crow::request& req) {
		   std::string name = req.url_params.get("name");
		   std::string inputRow = req.url_params.get("row");
		   std::string inputColumn = req.url_params.get("column");

		   int row = std::stoi(inputRow);
		   int column = std::stoi(inputColumn);

		   int attackerPos, attackedPos;
		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
				   attackerPos = i;
			   else if (Triviador.GetPlayers()[i].GetName() == copyNameOfAttackedPlayer)
				   attackedPos = i;
		   colorMove = Triviador.GetPlayers()[attackerPos].colorToString(Triviador.GetPlayers()[attackerPos].GetColor());

		   auto& stolenRegion = Triviador.GetPlayers()[attackedPos].MoveRegion(row, column, colorMove);
		   Triviador.GetPlayers()[attackerPos].GetRegions().push_back(std::move(stolenRegion));
		   Triviador.GetMap().GetRegion(row, column).ChangeColor(colorMove);
		   playersOrder.clear();
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/increment")([&inc]() {
		   inc++;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/Verify/increment")([&inc]() {
		   if (inc == 2)
			   return crow::response("ok");
		   else return crow::response("not ok");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/waitForPlayersEndRound/increment")([&syncEndRound]() {
		   syncEndRound++;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/waitForPlayersEndRound")([&syncEndRound, &Triviador]() {
		   if (syncEndRound == Triviador.GetPlayers().size())
			   return crow::response("ok");
		   else return crow::response("not ok");
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/waitForPlayersEndRound/reset")([&syncEndRound]() {
		   syncEndRound = 0;
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/sameAnswer/reset")([&playersOrder, &waitingForPlayersAnswer]() {
		   waitingForPlayersAnswer = 0;
		   playersOrder.clear();
		   return crow::response(200);
	   });

	   CROW_ROUTE(m_app, "/sameAnswer/sendResponse")([&waitingForPlayersAnswer, &playersOrder, &Triviador, &gameQuestionsNumeric](const crow::request& req) {
		   waitingForPlayersAnswer++;
		   std::string name = req.url_params.get("name");
		   std::string answer = req.url_params.get("answer");
		   int num = std::stoi(answer);
		   for (int i = 0; i < Triviador.GetPlayers().size(); i++)
			   if (Triviador.GetPlayers()[i].GetName() == name)
			   {
				   playersOrder.insert(std::pair<int, Player>(abs(gameQuestionsNumeric[gameQuestionsNumeric.size() - 1].GetNumericAnswer() - num), Triviador.GetPlayers()[i]));
				   break;
			   }
		   return crow::response(200);

	   });

	   CROW_ROUTE(m_app, "/sameAnswer/checkWinner")([&playersOrder]() {
		   return crow::response(playersOrder.begin()->second.GetName());
	   });
   }