#include "AuxiliarUtility.h"

void AuxiliarUtility::ShowMenu(std::string& name)
{
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
    std::string password;
    std::string state;
    std::cout << "Triviador\n";
    auto users = crow::json::load(response.text);
    bool quit = false;
    bool ok = true;
    while (quit == false)
    {
        std::cout << "Pick an option \n";
        std::cout << "(a) Login \n";
        std::cout << "(b) Sign Up \n";

        char option;
        std::cin >> option;
        switch (option)
        {
        case 'a':
        {
            while (ok)
            {
                std::cout << "Introduce username and password: \n";
                std::cout << "Username: ";
                std::cin >> name;
                std::cout << "Password: ";
                std::cin >> password;

                std::stringstream urlBuilder;
                urlBuilder << "http://localhost:18080/login";

                auto response = cpr::Get(cpr::Url{ urlBuilder.str() },
                    cpr::Parameters{ { "name",std::string(name) } ,{ "password", std::string(password) } });

                if (response.text == "ok")
                {
                    std::cout << "Login succeded!\n";
                    std::chrono::seconds dura(3);
                    std::this_thread::sleep_for(dura);
                    system("CLS");
                    ok = false;
                    bool statsoptions = true;
                    while (statsoptions)
                    {
                        std::cout << "Pick an option \n";
                        std::cout << "(1) Play \n";
                        std::cout << "(2) Stats \n";

                        char loginOption;
                        std::cin >> loginOption;
                        while (loginOption != '1' && loginOption != '2')
                        {
                            std::cout << "Invalid option! Try again!\n";
                            std::cin >> loginOption;
                        }
                        switch (loginOption)
                        {
                        case '1':
                        {
                           
                            std::stringstream urlBuilderIncrement;
                            urlBuilderIncrement << "http://localhost:18080/incrementGamesPlayed";
                            auto incrementResponse = cpr::Get(cpr::Url{ urlBuilderIncrement.str() },
                                cpr::Parameters{ { "name",std::string(name) } });
                            
                            statsoptions = false;
                            std::cout << "Are you ready? Y/N\n";
                            std::string state;
                            std::cin >> state;
                            while (state != "Y" && state != "N")
                            {
                                std::cout << "Invalid option! Try again!\n";
                                std::cin >> state;
                            }
                            if (state == "Y")
                            {
                                state = "ready";
                                std::stringstream urlBuilderState;
                                urlBuilderState << "http://localhost:18080/ready";
                                auto stateResponse = cpr::Get(cpr::Url{ urlBuilderState.str() },
                                    cpr::Parameters{ { "name",std::string(name) },{ "state", std::string(state) } });
                                if (stateResponse.text == "ok")
                                {
                                    std::cout << "Get ready.\n";
                                    quit = true;
                                }
                            }
                            break;
                        }
                        case '2':
                        {

                            for (const auto& user : users)
                            {
                                if (name == user["name"].s())
                                {
                                    std::cout << "\nNumber Of Games Played:" << user["numberOfGamesPlayed"]
                                        << "\nNumber Of Winnings : " << user["NumberOfWinnings"] << '\n';
                                }
                            }

                            break;
                        }
                        }

                    }
                }
                else if (response.text == "invalidPassword")
                {
                    std::cout << "Invalid password! try again!\n";
                }
            }
            break;
        }
        case 'b':
        {
            bool ok2 = true;
            while (ok2)
            {
                std::cout << "Introduce username and password: \n";
                std::string name;
                std::cout << "Username:  ";
                std::cin >> name;
                std::string password;
                std::cout << "Password: ";
                std::cin >> password;

                std::stringstream urlBuilder;
                urlBuilder << "http://localhost:18080/signup";
                auto response = cpr::Get(cpr::Url{ urlBuilder.str() },
                    cpr::Parameters{ { "name",std::string(name) } ,{ "password", std::string(password) } });
                if (response.text == "ok")
                {
                    std::cout << "Account created!\n";
                    ok2 = false;
                }
                else if (response.text == "taken")
                {
                    std::cout << "Username is already taken!Choose another.\n";

                }
            }
            break;
        }
        case 'q':
        {
            std::cout << "closed menu " << std::endl;
            exit(0);
        }

        default: std::cout << "Please use a,b or q!\n";
        }
    }
}

void AuxiliarUtility::ShowGridsAvantage(const std::string& name)
{
    std::stringstream urlBuilderAvantage;
    urlBuilderAvantage << "http://localhost:18080/availableAvantages";
    auto responseAvantages = cpr::Get(cpr::Url{ urlBuilderAvantage.str() },
        cpr::Parameters{ { "name",std::string(name) } });
    std::string code = responseAvantages.text;
    if (code.at(0) == '0')
    {
        std::cout << "Do you want to use an avantage? Y/N\n";
        std::string answer;
        std::cin >> answer;
        while (answer != "Y" && answer != "N")
        {
            std::cout << "Invalid option! Try again!\n";
            std::cin >> answer;
        }
        if (answer == "Y")
        {
            auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                cpr::Parameters{ { "name",std::string(name) },{ "index","0" } });
            auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantage5050" }).text);
            std::cout << "\n" << advantageResponse["correctAnswer"] << "  " << advantageResponse["wrongAnswer"] << "  ";

        }
        else if (answer == "N")
        {
            std::cout << "You chose not to use an avantage!\n";

        }
    }
}

void AuxiliarUtility::ShowNumericAvantages(const std::string& name)
{
    std::stringstream urlBuilderAvantage;
    urlBuilderAvantage << "http://localhost:18080/availableAvantages";
    auto responseAvantages = cpr::Get(cpr::Url{ urlBuilderAvantage.str() },
        cpr::Parameters{ {"name",std::string(name)} });
    std::string code = responseAvantages.text;
    
    if (code.at(1) == '0' && code.at(2) == '0')
    {
        std::cout << "Do you want to use an avantage? Y/N\n";
        std::string answer;
        std::cin >> answer;
        while (answer != "Y" && answer != "N")
        {
            std::cout << "Invalid option! Try again!\n";
            std::cin >> answer;
        }
        if (answer == "Y")
        {
            std::cout << "Choose an avantage: \n";
            std::cout << "(1) choose numeric answer \n"; 
            std::cout << "(2) suggestion \n";

            char avantageOption;
            std::cin >> avantageOption;
            while (avantageOption != '1' && avantageOption != '2')
            {
                std::cout << "Invalid option! Try again!\n";
                std::cin >> avantageOption;
            }
            switch (avantageOption)
            {
            case '1':
            {
                auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                    cpr::Parameters{ {"name",std::string(name)}, {"index","1"} });

                std::cout << "You chose numeric answer!\n";
                std::cout << "Choose an answer: \n";
                auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantageNumeric" }).text);
                std::cout << "\n" << advantageResponse["answer1"] << "  " << advantageResponse["answer2"] << "  "
                    << advantageResponse["answer3"] << "  " << advantageResponse["answer4"];
                break;
            }
            case '2':
            {
                std::cout << "You chose suggestion!\n";
                auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                    cpr::Parameters{ {"name",std::string(name)}, {"index","2"} });
                auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantageSuggestion" }).text);
                std::cout << "Here's one similar answer: \n";
                std::cout << "\n" << advantageResponse["similarAnswer"] << "\n";
                break;
            }
            }
        }
        else if (answer == "N")
        {
            std::cout << "You chose not to use an avantage!\n";
        }
    }
    else if ((code.at(1) == '0' && code.at(2) == '1'))
    {
        std::cout << "Do you want to use the choose numeric answer avantage? Y/N\n";
        std::string answer;
        std::cin >> answer;
        while (answer != "Y" && answer != "N")
        {
            std::cout << "Invalid option! Try again!\n";
            std::cin >> answer;
        }
        if (answer == "Y")
        {
            auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                cpr::Parameters{ {"name",std::string(name)}, {"index","1"} });
            std::cout << "You chose numeric answer!\n";
            std::cout << "Choose an answer: \n";
            auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantageNumeric" }).text);
            std::cout << "\n" << advantageResponse["answer1"] << "  " << advantageResponse["answer2"] << "  "
                << advantageResponse["answer3"] << "  " << advantageResponse["answer4"] << "\n";
        }
        else if (answer == "N")
        {
            std::cout << "You chose not to use an avantage!\n";
        }
    }
    else  if (code.at(1) == '1' && code.at(2) == '0')
    {
        std::cout << "Do you want to use the suggestion avantage? Y/N\n";
        std::string answer;
        std::cin >> answer;
        while (answer != "Y" && answer != "N")
        {
            std::cout << "Invalid option! Try again!\n";
            std::cin >> answer;
        }
        if (answer == "Y")
        {
            auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                cpr::Parameters{ {"name",std::string(name)}, {"index","2"} });
            std::cout << "You chose suggestion!\n";
            auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantageSuggestion" }).text);
            std::cout << "Here's one similar answer: \n";
            std::cout << "\n" << advantageResponse["similarAnswer"] << "\n";
        }
        else if (answer == "N")
        {
            std::cout << "You chose not to use an avantage!\n";
        }
    }

}

void AuxiliarUtility::CheckRowPosition(int& row, crow::json::rvalue& regions)
{

	if (regions.size() == 9)
	{
		while (row > 2 || row < 0)
		{
			std::cout << "Please enter a valid row!\n";
			std::cin >> row;
		}
	}
	else if (regions.size() == 15)
	{
		while (row > 4 || row < 0)
		{
			std::cout << "Please enter a valid row!\n";
			std::cin >> row;
		}
	}
	else if (regions.size() == 24)
	{
		while (row > 5 || row < 0)
		{
			std::cout << "Please enter a valid row!\n";
			std::cin >> row;
		}
	}
}

void AuxiliarUtility::CheckColumnPosition(int& column, crow::json::rvalue& regions)
{
	if (regions.size() == 9 || regions.size() == 15)
	{
		while (column > 2 || column < 0)
		{
			std::cout << "Please enter a valid column!\n";
			std::cin >> column;
		}
	}
	else if (regions.size() == 24)
	{
		while (column > 3 || column < 0)
		{
			std::cout << "Please enter a valid column!\n";
			std::cin >> column;
		}
	}
}

void AuxiliarUtility::PrintJsonMap(crow::json::rvalue& regions)
{
	int column;
	if (regions.size() == 9)
	{
		column = 3;
	}
	else if (regions.size() == 15)
	{
		column = 3;
	}
	else if (regions.size() == 24)
	{
		column = 4;
	}
	int  j = 0;
	for (const auto& reg : regions)
	{
		std::cout << reg["color"] << " ";
		j++;
		if (j == column)
		{
			j = 0;
			std::cout << "\n";
		}
	}
}

void AuxiliarUtility::StartResponse()
{
    auto startGameResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waiting" });
    do
    {
        if (startGameResponse.text == "ok")
            break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        startGameResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waiting" });
    } while (startGameResponse.text == "waiting");
}

void AuxiliarUtility::ChooseBaseStage(const std::string& name)
 {
     auto rsp = cpr::Get(cpr::Url{ "http://localhost:18080/printMap" });
     auto regions = crow::json::load(rsp.text);

     AuxiliarUtility::PrintJsonMap(regions);

     std::string numericAnswer;
     auto questionChooseBase = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/questionChooseBase" }).text); 
     std::cout << "\n" << questionChooseBase["question"] << "\n";

     std::cout << "Your answer : ";
     std::cin >> numericAnswer;
     while (!std::regex_match(numericAnswer, std::regex("[0-9]+")))
     {
         std::cout << "Please enter a number!\n";
         std::cin >> numericAnswer;
     }
     auto answerResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayers/Base" },
         cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(numericAnswer) } }); 

     auto waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingBase" }); 
     do
     {
         if (waitAnswersPlayersResponse.text == "ok")
             break;
         std::this_thread::sleep_for(std::chrono::seconds(1));
         waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingBase" });

     } while (waitAnswersPlayersResponse.text == "waiting");

     auto orderAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayersOrder" },
         cpr::Parameters{ { "name",std::string(name) } }); //ia ordinea playerilor pentru alegerea bazei
     while (orderAnswersPlayersResponse.text == "waiting")
     {
         std::this_thread::sleep_for(std::chrono::seconds(1));
         orderAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayersOrder" },
             cpr::Parameters{ { "name",std::string(name) } });
     }
     if (orderAnswersPlayersResponse.text == "ok")
     {
         rsp = cpr::Get(cpr::Url{ "http://localhost:18080/printMap" });
         regions = crow::json::load(rsp.text);
         AuxiliarUtility::PrintJsonMap(regions);

         int row, column;
         std::cout << "Choose your BASE Region by position: \n" << "Row: \n";
         std::cin >> row;
         while (!std::regex_match(std::to_string(row), std::regex("[0-9]+")))
         {
             std::cout << "Please enter a number!\n";
             std::cin >> row;
         }
         AuxiliarUtility::CheckRowPosition(row, regions);

         std::cout << "Column: \n";
         std::cin >> column;
         while (!std::regex_match(std::to_string(column), std::regex("[0-9]+")))
         {
             std::cout << "Please enter a number!\n";
             std::cin >> column;
         }
         AuxiliarUtility::CheckColumnPosition(column, regions);

         auto clientChoosingBaseResponse = cpr::Get(cpr::Url{ "http://localhost:18080/clientChooseBase" },
             cpr::Parameters{ { "row",std::string(std::to_string(row)) },{ "column", std::string(std::to_string(column)) },{ "name",std::string(name) } });
     }
     waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersToChoose" }); 
     do
     {
         if (waitAnswersPlayersResponse.text == "ok")
             break;
         std::this_thread::sleep_for(std::chrono::seconds(1));
         waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersToChoose" });
     } while (waitAnswersPlayersResponse.text == "waiting");

     std::cout << "\nNow it's time to choose the remaining REGIONS to fill the map!\n";
 }

void AuxiliarUtility::ChooseRegionsStage(const std::string& name)
 {
     std::string numericAnswer;
     auto questionChooseBase = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/questionChooseBase" }).text);
     auto unoccupiedRegionsResponse = cpr::Get(cpr::Url{ "http://localhost:18080/checkUnoccupiedRegions" });
     while (unoccupiedRegionsResponse.text == "Exists unoccupied regions")
     {
         questionChooseBase = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionNumeric" }).text);
         std::cout << "\n" << questionChooseBase["question"] <<  "\n";
         AuxiliarUtility::ShowNumericAvantages(name);
         std::cout << "Your answer : ";
         std::cin >> numericAnswer;

         auto answerResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayers/Regions" },
             cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(numericAnswer) } });

         auto waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingBase" }); //asteapta ca toti sa dea raspuns la intrebare
         do
         {
             if (waitAnswersPlayersResponse.text == "ok")
                 break;
             std::this_thread::sleep_for(std::chrono::seconds(1));
             waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingBase" });
         } while (waitAnswersPlayersResponse.text == "waiting");


         auto orderAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayersOrder" },
             cpr::Parameters{ { "name",std::string(name) } }); //ia ordinea playerilor pentru alegerea bazei
         while (orderAnswersPlayersResponse.text == "waiting")
         {
             std::this_thread::sleep_for(std::chrono::seconds(1));
             orderAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/setPlayersOrder" },
                 cpr::Parameters{ { "name",std::string(name) } });
         }
         if (orderAnswersPlayersResponse.text == "ok") 
         {
             auto numberOfRegionsToChoose = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/returnNumberOfRegToChoose" }).text).operator int();
             if (!numberOfRegionsToChoose)
             {
                 auto eraseQuestionResponse = cpr::Get(cpr::Url{ "http://localhost:18080/eraseQuestion/numeric" });
             }
             while (numberOfRegionsToChoose)
             {
                 std::cout << "\nYou have " << numberOfRegionsToChoose << " remaining REGIONS to choose \n";
                 auto rsp = cpr::Get(cpr::Url{ "http://localhost:18080/printMap" });
                 auto regions = crow::json::load(rsp.text);
                 AuxiliarUtility::PrintJsonMap(regions);
                 int row, column;
                 std::cout << "Choose your REGION by position: \n" << "Row: \n";
                 std::cin >> row;
                 std::cout << "Column: \n";
                 std::cin >> column;
                 AuxiliarUtility::CheckColumnPosition(column, regions);
                 auto clientChoosingBaseResponse = cpr::Get(cpr::Url{ "http://localhost:18080/clientChooseRemainingRegions" },
                     cpr::Parameters{ { "row",std::string(std::to_string(row)) },{ "column", std::string(std::to_string(column)) },{ "name",std::string(name) } });
                 numberOfRegionsToChoose--;
             }
         }
         auto erasePlayerFromOrderMapResp = cpr::Get(cpr::Url{ "http://localhost:18080/erasePlayerFromOrderMap" });
         waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersToChoose" }); //asteapta ca toti sa aleaga regiune
         do
         {
             if (waitAnswersPlayersResponse.text == "ok")
                 break;
             std::this_thread::sleep_for(std::chrono::seconds(1));
             waitAnswersPlayersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersToChoose" });
         } while (waitAnswersPlayersResponse.text == "waiting");
         unoccupiedRegionsResponse = cpr::Get(cpr::Url{ "http://localhost:18080/checkUnoccupiedRegions" });
     }
 }

void AuxiliarUtility::DuelFightStage(const std::string& name)
 {
     auto numberRoundsResponse = cpr::Get(cpr::Url{ "http://localhost:18080/return/numberOfRounds" }).text;
     int numberOfRounds = std::stoi(numberRoundsResponse);
     int round = 1;
     std::string state;
     while (numberOfRounds)
     {
         std::cout << "\nROUND " << round << "\n";
         auto clientsTurnResponse = cpr::Get(cpr::Url{ "http://localhost:18080/clientsTurnToAttack" },
             cpr::Parameters{ { "name",std::string(name) } }).text;
         auto checkEndOfRound = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound" }).text;

         int row, column, gridsAnswer;
         do
         {
             if (clientsTurnResponse == "ok") //cel care ataca
             {
                 auto questionDuel = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionGrid" }).text);
                 std::cout << "\nIt's your turn to attack! \n";
                 auto rsp = cpr::Get(cpr::Url{ "http://localhost:18080/printMap" });
                 auto regions = crow::json::load(rsp.text);
                 AuxiliarUtility::PrintJsonMap(regions);
                 std::cout << "Choose a REGION nearby your REGIONS by position: \n" << "Row: \n";
                 std::cin >> row;
                 std::cout << "Column: \n";
                 std::cin >> column;

                 auto verifyItIsNeigbor = cpr::Get(cpr::Url{ "http://localhost:18080/verifyItIsNeigbor" },
                     cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                     { "column", std::string(std::to_string(column)) } });
                 do
                 {
                     if (verifyItIsNeigbor.text == "Not neighbor")
                     {
                         std::cout << "The REGION you chose is not close to yours! Choose again!\n";
                         std::cout << "Choose a REGION nearby your REGIONS by position: \n" << "Row: \n";
                         std::cin >> row;
                         std::cout << "Column: \n";
                         std::cin >> column;
                         verifyItIsNeigbor = cpr::Get(cpr::Url{ "http://localhost:18080/verifyItIsNeigbor" },
                             cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                             { "column", std::string(std::to_string(column)) } });
                     }
                 } while (verifyItIsNeigbor.text == "Not neighbor");

                 auto attackedPlayerResponse = cpr::Get(cpr::Url{ "http://localhost:18080/searchAttackedPlayer" },
                     cpr::Parameters{ { "row",std::string(std::to_string(row)) },
                     { "column", std::string(std::to_string(column)) } });

                 std::cout << "Are you ready? Y/N\n";
                 std::cin >> state;
                 if (state == "Y")
                     state = "ready";
                 auto stateForDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/prepareDuel" },
                     cpr::Parameters{ { "state",std::string(state) } });
                 auto readyDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/duel" });
                 do
                 {
                     if (readyDuel.text == "ok")
                         break;
                     std::this_thread::sleep_for(std::chrono::seconds(1));
                     readyDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/duel" });
                 } while (readyDuel.text == "wait");

                 std::cout << "\n" << questionDuel["question"] << "\n"; 
                 std::cout << "1." << questionDuel["answerGrids1"] << "\n"
                     << "2." << questionDuel["answerGrids2"] << "\n"
                     << "3." << questionDuel["answerGrids3"] << "\n"
                     << "4." << questionDuel["answerGrids4"] << "\n";
                 AuxiliarUtility::ShowGridsAvantage(name);
                 std::cout << "Choose a number from 1 to 4:\n";
                 std::cin >> gridsAnswer;

                 auto incrementAnswer = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer/increment" },
                     cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(std::to_string(gridsAnswer)) } });

                 while (cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer" }).text == "wait") {}

                 auto checkWinner = cpr::Get(cpr::Url{ "http://localhost:18080/checkWinner" }).text;

                 if (checkWinner == "Same answer")
                 {
                     auto questionDuelSameAnswer = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionNumeric" }).text);
                     std::cout << "\nYou both got the answer right!\n";
                     std::cout << "\nNumeric question incoming!\n";
                     std::cout << "\n" << questionDuelSameAnswer["question"] <<"\n";
                     auto reset = cpr::Get(cpr::Url{ "http://localhost:18080/sameAnswer/reset" });
                     std::cout << "Your answer: ";
                     std::cin >> gridsAnswer;
                     auto incrementAnswer = cpr::Get(cpr::Url{ "http://localhost:18080/sameAnswer/sendResponse" },
                         cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(std::to_string(gridsAnswer)) } });

                     while (cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer" }).text == "wait") {}

                     auto checkWinnerPhase2 = cpr::Get(cpr::Url{ "http://localhost:18080/sameAnswer/checkWinner" }).text;
                     if (checkWinnerPhase2 == name)
                     {
                         std::cout << "\nYou got the answer right! You got your opponent's region!\n";
                         auto winnerOfDuel = cpr::Get(cpr::Url{ "http://localhost:18080/moveRegionToWinner" },
                             cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                             { "column", std::string(std::to_string(column)) } });
                     }
                     else
                         auto decreaseScore = cpr::Get(cpr::Url{ "http://localhost:18080/decreaseScore" },
                             cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                             { "column", std::string(std::to_string(column)) } });
                     auto eraseQuestionResponse = cpr::Get(cpr::Url{ "http://localhost:18080/eraseQuestion/numeric" });
                 }
                 else if (checkWinner == "incorrect") //amandoi au gresit
                 {
                     std::cout << "\nYou both got the answer wrong\n";
                 }
                 else if (checkWinner == name) // atacatorul castiga
                 {
                     std::cout << "\nYou got the answer right! You got your opponent's region!\n";
                     auto winnerOfDuel = cpr::Get(cpr::Url{ "http://localhost:18080/moveRegionToWinner" },
                         cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                         { "column", std::string(std::to_string(column)) } });
                 }
                 else
                 {
                     std::cout << "\nYou got the answer wrong! You lost 100 points!\n";
                     auto decreaseScore = cpr::Get(cpr::Url{ "http://localhost:18080/decreaseScore" }, // atacatorul pierde
                         cpr::Parameters{ { "name",std::string(name) },{ "row",std::string(std::to_string(row)) },
                         { "column", std::string(std::to_string(column)) } });
                 }

                 auto removePlayerFromShuffle = cpr::Get(cpr::Url{ "http://localhost:18080/removePlayerFromShuffle" });
                 auto eraseQuestionGrids = cpr::Get(cpr::Url{ "http://localhost:18080/eraseQuestion/grids" });
                 auto increaseTurn = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound/increase" });
                 auto increment = cpr::Get(cpr::Url{ "http://localhost:18080/increment" });
                 auto verifyIncrement = cpr::Get(cpr::Url{ "http://localhost:18080/Verify/increment" });
                 do
                 {
                     if (verifyIncrement.text == "ok")
                         break;
                     std::this_thread::sleep_for(std::chrono::seconds(1));
                     verifyIncrement = cpr::Get(cpr::Url{ "http://localhost:18080/Verify/increment" });

                 } while (verifyIncrement.text == "not ok");

             }
             else if (cpr::Get(cpr::Url{ "http://localhost:18080/verifyItIsAttacked" }, //cel atacat
                 cpr::Parameters{ { "name",std::string(name) } }).text == "ok")
                 {

                     auto questionDuel = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionGrid" }).text);
                     std::cout << "\nYou are being attacked! Get ready!\n";
                     std::cout << "Are you ready? Y/N\n";
                     std::cin >> state;
                     if (state == "Y")
                         state = "ready";
                     auto stateForDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/prepareDuel" },
                         cpr::Parameters{ { "state",std::string(state) } });
                     auto readyDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/duel" });
                     do
                     {
                         if (readyDuel.text == "ok")
                             break;
                         std::this_thread::sleep_for(std::chrono::seconds(1));
                         readyDuel = cpr::Get(cpr::Url{ "http://localhost:18080/ready/duel" });
                     } while (readyDuel.text == "wait");

                     std::cout << "\n" << questionDuel["question"] << "\n";
                     std::cout << "1." << questionDuel["answerGrids1"] << "\n"
                         << "2." << questionDuel["answerGrids2"] << "\n"
                         << "3." << questionDuel["answerGrids3"] << "\n"
                         << "4." << questionDuel["answerGrids4"] << "\n";
                     std::stringstream urlBuilderAvantage;
                     urlBuilderAvantage << "http://localhost:18080/availableAvantages";
                     auto responseAvantages = cpr::Get(cpr::Url{ urlBuilderAvantage.str() },
                         cpr::Parameters{ { "name",std::string(name) } });
                     std::string code = responseAvantages.text;
                     if (code.at(0) == '0')
                     {
                         std::cout << "Do you want to use an avantage? Y/N\n";
                         std::string answer;
                         std::cin >> answer;
                         while (answer != "Y" && answer != "N")
                         {
                             std::cout << "Invalid option! Try again!\n";
                             std::cin >> answer;
                         }
                         if (answer == "Y")
                         {
                             auto responsetomodify = cpr::Get(cpr::Url{ "http://localhost:18080/boolOfAvantages" },
                                 cpr::Parameters{ { "name",std::string(name) },{ "index","0" } });
                             auto advantageResponse = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/avantage5050" }).text);
                             std::cout << "\n" << advantageResponse["correctAnswer"] << "  " << advantageResponse["wrongAnswer"] << "\n";

                         }
                         else if (answer == "N")
                         {
                             std::cout << "You chose not to use an avantage!\n";

                         }
                     }
                     std::cout << "Choose a number from 1 to 4:\n";
                     std::cin >> gridsAnswer;
                     auto incrementAnswer = cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer/increment" },
                         cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(std::to_string(gridsAnswer)) } });

                     while (cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer" }).text == "wait") {}

                     auto checkWinner = cpr::Get(cpr::Url{ "http://localhost:18080/checkWinner" }).text;
                     if (checkWinner == "Same answer")
                     {
                     
                         auto questionDuelSameAnswer = crow::json::load(cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionNumeric" }).text);
                         std::cout << "\nYou both got the answer right!\n";
                         std::cout << "\nNumeric question incoming!\n";
                         std::cout << "\n" << questionDuelSameAnswer["question"] << "\n";
                         std::cout << "Your answer: ";
                         std::cin >> gridsAnswer;
                         auto incrementAnswer = cpr::Get(cpr::Url{ "http://localhost:18080/sameAnswer/sendResponse" },
                             cpr::Parameters{ { "name",std::string(name) },{ "answer", std::string(std::to_string(gridsAnswer)) } });

                         while (cpr::Get(cpr::Url{ "http://localhost:18080/waitingForPlayersAnswer" }).text == "wait") {}

                         auto checkWinnerPhase2 = cpr::Get(cpr::Url{ "http://localhost:18080/sameAnswer/checkWinner" }).text;
                         if (checkWinnerPhase2 == name)
                         {
                             std::cout << "\nYou got the answer right! You get 100 points on your region!\n";
                         }
                         else
                             std::cout << "\nYou got the answer wrong! You lost your region!\n";
                     }
                     else if (checkWinner == "incorrect") //amandoi au gresit
                     {
                         std::cout << "\nYou both got the answer wrong\n";
                     }
                     else if (checkWinner == name) // atacatul castiga
                     {
                         std::cout << "\nYou got the answer right! You get 100 points on your region!\n";
                     }
                     else
                     {
                         std::cout << "\nYou got the answer wrong! You lost your region!\n";
                     }
                     auto increment = cpr::Get(cpr::Url{ "http://localhost:18080/increment" });
                     auto verifyIncrement = cpr::Get(cpr::Url{ "http://localhost:18080/Verify/increment" });
                     do
                     {
                         if (verifyIncrement.text == "ok")
                             break;
                         std::this_thread::sleep_for(std::chrono::seconds(1));
                         verifyIncrement = cpr::Get(cpr::Url{ "http://localhost:18080/Verify/increment" });

                     } while (verifyIncrement.text == "not ok");
                     auto reset = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound/reset2" });
                     auto resetName = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound/resetAttackedPlayer" });
             }
             std::this_thread::sleep_for(std::chrono::seconds(1));
             clientsTurnResponse = cpr::Get(cpr::Url{ "http://localhost:18080/clientsTurnToAttack" },
                 cpr::Parameters{ { "name",std::string(name) } }).text;
             checkEndOfRound = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound" }).text;

         } while (checkEndOfRound == "not over");
         auto increment = cpr::Get(cpr::Url{ "http://localhost:18080/waitForPlayersEndRound/increment" });
         auto allPrepared = cpr::Get(cpr::Url{ "http://localhost:18080/waitForPlayersEndRound" });
         do
         {
             if (allPrepared.text == "ok")
                 break;
             allPrepared = cpr::Get(cpr::Url{ "http://localhost:18080/waitForPlayersEndRound" });
         } while (allPrepared.text == "not ok");

         auto reset = cpr::Get(cpr::Url{ "http://localhost:18080/checkEndOfRound/reset" });
         auto shufflePlayers = cpr::Get(cpr::Url{ "http://localhost:18080/shufflePlayers" });
         numberOfRounds--;
         round++;
         auto resetSyncEndRound = cpr::Get(cpr::Url{ "http://localhost:18080/waitForPlayersEndRound/reset" });
         system("CLS");
     }
 }
