#pragma once

#include <csignal>
#include <functional>

struct SIG {

	public:

		using handler = std::function<void(int)>;

		const handler TERM = {};
		const handler ALRM = {};
		const handler HUP = {};
		const handler INT = {};
		const handler PIPE = {};
		const handler QUIT = {};
		const handler USR1 = {};
		const handler USR2 = {};

		void install();

		static void uninstall();
		static bool is_registered();
		static std::string to_string(int sig);
};
