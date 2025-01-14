#pragma once

#include <csignal>
#include <functional>

struct SIG {

	public:

		using handler = std::function<void(int)>;

		handler TERM = {};
		handler ALRM = {};
		handler HUP = {};
		handler INT = {};
		handler PIPE = {};
		handler QUIT = {};
		handler USR1 = {};
		handler USR2 = {};

		SIG& operator =(const SIG& other);

		bool empty() const;
		void clear();

		void install();

		static void uninstall();
		static bool is_registered();
		static std::string to_string(int sig);
};
