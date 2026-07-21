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

	bool Conditional(const std::string& snippet)
	{
		return Sol.script(fmt::format("return ({})", snippet));
	}

	std::string BJTS(const std::string& func, const std::vector<std::string> bjts)
	{
		Sol["bjts"] = bjts;
		auto ret = Sol.script(func).get<std::string>();
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
