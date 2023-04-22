#include <iostream>
#include <sstream>
#include <string>
#include <cpr/cpr.h>
#include <crow.h>
#include "../TriviadorGame/Player.h"
#include "../TriviadorGame/Game.h"
#include "AuxiliarUtility.h"

int main()
{
    std::string name;
   
    AuxiliarUtility::ShowMenu(name);
    AuxiliarUtility::StartResponse();
    AuxiliarUtility::ChooseBaseStage(name);
    AuxiliarUtility::ChooseRegionsStage(name);
    AuxiliarUtility::DuelFightStage(name);

}