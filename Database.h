#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
#include "QuestionManager.h"
#include <random>
#include"Question.h"
namespace sql = sqlite_orm;

struct User
{
public:
    int id;
    std::string name;
    std::string password;
    int numberOfGamesPlayed;
    int NumberOfWinnings;

public:
    User() {};
    User(int id, std::string name, std::string password, int numberOfGamesPlayed, int NumberOfWinnings) :
        id(id), name(name), password(password), numberOfGamesPlayed(numberOfGamesPlayed), NumberOfWinnings(NumberOfWinnings)
    {};

};
inline auto CreateStorage(const std::string& filename)
{
    return sql::make_storage(
        filename,
        sql::make_table(
            "User",
            sql::make_column("id", &User::id, sql::autoincrement(), sql::primary_key()),
            sql::make_column("name", &User::name),
            sql::make_column("password", &User::password),
            sql::make_column("numberOfGamesPlayed", &User::numberOfGamesPlayed),
            sql::make_column("NumberOfWinnings", &User::NumberOfWinnings)
        ),
        
        sql::make_table(
            "Questions",
            sql::make_column("id", &Question::m_id, sql::primary_key()),
            sql::make_column("question", &Question::m_question),
            sql::make_column("answerOrindexCorrectGrid", &Question::m_answerOrIndex)
        )
    );
}
using Storage = decltype(CreateStorage(""));
class Database
{
public:
    void PopulateQuestionsStorage();
    Question GenerateRandomQuestion(char c);
    void RemoveQuestionFromDb(Question q);
    void UpdateDataBase();
    std::vector<Question>& GetAllQuestionsVector();
    Storage& GetStorage() { return m_db; }

public:
    int GetUserIdforPlayer(std::string name);
    void PopulateStorage();
    void InsertIntoDatabase(const User& user);
    bool IsAlreadyTaken(const std::string& username);
    bool IsPasswordValid(const std::string& name,const  std::string& password);
    void IncrementGamesPlayed(const std::string& name);

private:
    Storage m_db = CreateStorage("db1.sqllite");
    std::vector <User> m_users;
    std::vector<Question> m_allQuestions;
    std::vector<Question> m_allQuestionsRemoved;
};