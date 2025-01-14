#include <stdexcept>
#include "signal.hpp"

static SIG::handler fn_TERM = {};
static SIG::handler fn_ALRM = {};
static SIG::handler fn_HUP = {};
static SIG::handler fn_INT = {};
static SIG::handler fn_PIPE = {};
static SIG::handler fn_QUIT = {};
static SIG::handler fn_USR1 = {};
static SIG::handler fn_USR2 = {};
static bool _is_registered = false;

static void die_handler(int sig) {

	if ( sig == SIGTERM && fn_TERM ) fn_TERM(sig);
	else if ( sig == SIGALRM && fn_ALRM ) fn_ALRM(sig);
	else if ( sig == SIGHUP && fn_HUP ) fn_HUP(sig);
	else if ( sig == SIGINT && fn_INT ) fn_INT(sig);
	else if ( sig == SIGPIPE && fn_PIPE ) fn_PIPE(sig);
	else if ( sig == SIGQUIT && fn_QUIT ) fn_QUIT(sig);
	else if ( sig == SIGUSR1 && fn_USR1 ) fn_USR1(sig);
	else if ( sig == SIGUSR2 && fn_USR2 ) fn_USR2(sig);
}

void fn_assign(SIG* other) {

	fn_TERM = other -> TERM;
	fn_ALRM = other -> ALRM;
	fn_HUP = other -> HUP;
	fn_INT = other -> INT;
	fn_PIPE = other -> PIPE;
	fn_QUIT = other -> QUIT;
	fn_USR1 = other -> USR1;
	fn_USR2 = other -> USR2;
}

void fn_clear() {

	fn_TERM = fn_ALRM = fn_HUP = fn_INT = fn_PIPE = fn_QUIT = fn_USR1 = fn_USR2 = {};
}

void SIG::install() {

	if ( _is_registered )
		throw std::runtime_error("signal handler cannot be registered, another handler is already registered");

	struct sigaction exit_action, ignore_action;

	exit_action.sa_handler = die_handler;
	sigemptyset(&exit_action.sa_mask);
	exit_action.sa_flags = 0;

	ignore_action.sa_handler = SIG_IGN;
	sigemptyset(&ignore_action.sa_mask);
	ignore_action.sa_flags = 0;

	_is_registered = true;
	fn_assign(this);

	if ( sigaction(SIGTERM, this -> TERM ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGTERM");
	if ( sigaction(SIGALRM, this -> ALRM ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGALRM");
	if ( sigaction(SIGHUP, this -> HUP ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGHUP");
	if ( sigaction(SIGINT, this -> INT ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGINT");
	if ( sigaction(SIGPIPE, this -> PIPE ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGPIPE");
	if ( sigaction(SIGQUIT, this -> QUIT ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGQUIT");
	if ( sigaction(SIGUSR1, this -> USR1 ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGUSR1");
	if ( sigaction(SIGUSR2, this -> USR2 ? &exit_action : &ignore_action, NULL) == -1 )
		throw std::runtime_error("failed to register signal handler for SIGUSR2");
}

void SIG::uninstall() {

	if ( !_is_registered )
		throw std::runtime_error("signal handler cannot be unregistered, signal handler was not previously registered");

	struct sigaction def_action;

	def_action.sa_handler = SIG_DFL;
	sigemptyset(&def_action.sa_mask);
	def_action.sa_flags = 0;

	sigaction(SIGTERM, &def_action, NULL);
	sigaction(SIGALRM, &def_action, NULL);
	sigaction(SIGHUP, &def_action, NULL);
	sigaction(SIGINT, &def_action, NULL);
	sigaction(SIGPIPE, &def_action, NULL);
	sigaction(SIGQUIT, &def_action, NULL);
	sigaction(SIGUSR1, &def_action, NULL);
	sigaction(SIGUSR2, &def_action, NULL);

	_is_registered = false;
	fn_clear();
}

bool SIG::is_registered() {

	return _is_registered;
}

std::string SIG::to_string(int sig) {

	switch (sig) {
		case SIGTERM: return "SIGTERM";
		case SIGALRM: return "SIGALRM";
		case SIGHUP: return "SIGHUP";
		case SIGINT: return "SIGINT";
		case SIGPIPE: return "SIGPIPE";
		case SIGQUIT: return "SIGQUIT";
		case SIGUSR1: return "SIGUSR1";
		case SIGUSR2: return "SIGUSR2";
		default: return "(unsupported signal type)";
	}
}
