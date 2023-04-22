//#pragma once
//#include <fstream>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <crow.h>
//#include <sqlite_orm/sqlite_orm.h>
//#include "QuestionManager.h"
//#include <random>
//namespace sql = sqlite_orm;
//
//inline auto createStorage1(const std::string& filename)
//{
//	return sql::make_storage(
//		filename,
//		sql::make_table(
//			"Questions",
//			sql::make_column("id", &Question::m_id, sql::autoincrement(), sql::primary_key()),
//			//verificam daca id-ul este <50 -> numeric, altfel grile
//			sql::make_column("question", &Question::m_question),
//			sql::make_column("answerOrindexCorrectGrid", &Question::m_answerOrIndex)
//		)
//	);
//}
//
//using Storage1 = decltype(createStorage1(""));//IULIAN
//
//class DataBaseQuestions
//{
//public:
//	void populateStorage();
//	Question GenerateRandomQuestion(char c);
//	//stergere intrebare din db
//
//	Storage1 m_db = createStorage1("questions.sqlite");
//	std::vector<Question> m_allQuestions;
//
//};
//
