#pragma once
#include "engine/Types.h"
#include "engine/Scripting.h"
#include <sol.hpp>

namespace Scripting
{
	extern sol::state* __sol;

	class SolProxy
	{
	public:
		template <typename T>
		sol::proxy<sol::global_table&, T> operator[](T&& key)
		{
			return __sol->operator[](std::forward<T>(key));
		}

		sol::unsafe_function_result script(const sol::string_view& code, const std::string& chunkname = sol::detail::default_chunk_name(), sol::load_mode mode = sol::load_mode::any)
		{
			return __sol->unsafe_script(code, chunkname, mode);
		}

		sol::protected_function_result do_string(const sol::string_view& code, const std::string& chunkname = sol::detail::default_chunk_name(), sol::load_mode mode = sol::load_mode::any)
		{
			return __sol->do_string(code, chunkname, mode);
		}
	};
}


class Scriptable
{
private:
	std::shared_ptr<sol::coroutine> currentCoro{ nullptr };

public:
	~Scriptable();

	void Bind(const std::string& file);
	void Execute(const std::string& entryPoint, bool* mutex);

	bool CanRun() const;
	void Call();
	sol::call_status Status();

	std::string ScriptID{ "" };
	bool* Mutex{ nullptr };
};
