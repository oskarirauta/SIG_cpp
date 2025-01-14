#include <iostream>
#include <atomic>
#include <unistd.h>

#include "signal.hpp"

std::atomic<bool> running(true);

static void die_handler(int signum) {

	std::cout << SIG::to_string(signum) << " signal received" << std::endl;
	running.store(false, std::memory_order_relaxed);
}

int main(const int argc, const char **argv) {

	std::cout << "signal handler test, program running with pid " << ::getpid() << std::endl;

	SIG handler = {
		.TERM = die_handler,
		.INT = die_handler
	};

	handler.install();
	std::cout << "exit with CTRL-C" << std::endl;

	while (running.load(std::memory_order_relaxed));

	std::cout << "program exits" << std::endl;
	return 0;
}
