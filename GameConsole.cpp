#include "engine/Console.h"
#include "Game.h"

std::string testString;
glm::vec2 testVector;
glm::vec4 testColor;

bool ConsoleInterpret(const std::string& str)
{
	conprint(1, "What does that mean?");
	return true;
}

void ConsoleRegister(Console* console)
{
#define RV console->RegisterCVar

	//RV("ai_disable", CVar::Type::Bool, &);
	//RV("cl_showpos", CVar::Type::Bool, &);
	//RV("noclip", CVar::Type::Bool, &, true);
	//RV("r_acredistance", CVar::Type::Int, &, false, 1, 6);
	//RV("r_drawgui", CVar::Type::Bool, &, true);
	//RV("r_drum", CVar::Type::Bool, &);
	//RV("r_drumexp", CVar::Type::Float, &);
	//RV("r_farz", CVar::Type::float, &, yes);

	RV("teststr", CVar::Type::String, &testString);
	RV("testvec", CVar::Type::Vec2, &testVector);
	RV("testcol", CVar::Type::Color, &testColor);

#undef RV

	//console->RegisterCCmd("reshade", CCmdReshade);
}
