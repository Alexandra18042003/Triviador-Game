#pragma once
#include <iostream>
#include <cpr/cpr.h>
#include <crow.h>
#include <regex>
class AuxiliarUtility
{
public:
    static void ShowMenu(std::string& name);
	static void ShowGridsAvantage(const std::string& name);
	static void ShowNumericAvantages(const std::string& name);
	static void CheckRowPosition(int& row, crow::json::rvalue& regions);
	static void CheckColumnPosition(int& column, crow::json::rvalue& regions);
	static void PrintJsonMap(crow::json::rvalue& regions);
	static void StartResponse();
	static void ChooseBaseStage(const std::string& name);
	static void ChooseRegionsStage(const std::string& name);
	static void DuelFightStage(const std::string& name);
};

