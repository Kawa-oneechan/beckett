#include <sol.hpp>
#include "engine/Types.h"
#include "engine/Console.h"
#include "engine/Scripting.h"

//Example implementation that uses Sol.

namespace Scripting
{
	sol::state Sol;

	static void CCmdLua(const jsonArray& args);

	void Setup()
	{
		Sol["print"] = [&](sol::variadic_args va) { console->Print(0, va[0]); };
		console->RegisterCCmd("lua", CCmdLua, true);
	}

	bool Conditional(const std::string& expression)
	{
		return Sol.script(fmt::format("return ({})", expression));
	}

	std::string BJTS(const std::string& code, const std::vector<std::string>& args)
	{
		Sol["bjts"] = args;
		auto ret = Sol.script(code).get<std::string>();
		Sol["bjts"] = nullptr;
		return ret;
	}

	static void CCmdLua(const jsonArray& args)
	{
		try
		{
			Sol.script(args[0].as_string());
		}
		catch (sol::error& e)
		{
			std::string what = e.what();
			if (what.find("attempt to yield from outside a coroutine") != std::string::npos)
				return; //Accept this silently.
			else
				conprint(1, "Error: {}", what);
		}
	}
}
