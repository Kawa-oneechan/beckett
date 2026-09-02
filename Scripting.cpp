#include <sol.hpp>
#include "engine/Types.h"
#include "engine/Console.h"
#include "engine/Scripting.h"
#include "engine/Random.h"
#include "engine/VFS.h"
#include "Scriptable.h"

//Example implementation that uses Sol.

namespace Scripting
{
	sol::state* Sol{ nullptr };

	static void CCmdLua(const jsonArray& args);

	void Setup()
	{
		Sol = new sol::state();

		Sol->operator[]("print") = [&](sol::variadic_args va) { console->Print(0, va[0]); };
		console->RegisterCCmd("lua", CCmdLua, true);
	}

	bool Conditional(const std::string& expression)
	{
		return Sol->script(fmt::format("return ({})", expression));
	}

	std::string BJTS(const std::string& code, const std::vector<std::string>& args)
	{
		Sol->operator[]("bjts") = args;
		auto ret = Sol->script(code).get<std::string>();
		Sol->operator[]("bjts") = nullptr;
		return ret;
	}

	static void CCmdLua(const jsonArray& args)
	{
		try
		{
			Sol->script(args[0].as_string());
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



Scriptable::~Scriptable()
{
	//delete currentCoro;
	currentCoro.reset();
	if (!ScriptID.empty())
		Scripting::Sol->operator[](ScriptID) = nullptr;
}

void Scriptable::Bind(const std::string& file)
{
	if (ScriptID.empty())
		ScriptID = fmt::format("SCR_{:x}", Random::GetInt(0x10000, 0x20000));
	Scripting::Sol->operator[](ScriptID) = Scripting::Sol->do_string(VFS::ReadString(file));
}
	
void Scriptable::Execute(const std::string& entryPoint, bool* mutex)
{
	if (ScriptID.empty())
	{
		conprint(4, "Can't run method {} on object without a script ID.", entryPoint);
		return;
	}

	Mutex = mutex;
	currentCoro = std::make_shared<sol::coroutine>(Scripting::Sol->operator[](ScriptID)[entryPoint]);
}

bool Scriptable::CanRun() const
{
	return currentCoro && currentCoro->runnable();
}

void Scriptable::Call()
{
	//auto dlgBox = root.GetChild<DialogueBox>();
	//dlgBox->Mutex = Mutex;
	currentCoro->call();
}

sol::call_status Scriptable::Status()
{
	return currentCoro->status();
}
