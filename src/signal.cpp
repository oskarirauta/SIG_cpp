#include <stdexcept>
#include "signal.hpp"

static SIG registered_handler;
static bool sig_registered = false;

static void die_handler(int sig) {

	if ( sig == SIGTERM && registered_handler.TERM ) registered_handler.TERM(sig);
	else if ( sig == SIGALRM && registered_handler.ALRM ) registered_handler.ALRM(sig);
	else if ( sig == SIGHUP && registered_handler.HUP ) registered_handler.HUP(sig);
	else if ( sig == SIGINT && registered_handler.INT ) registered_handler.INT(sig);
	else if ( sig == SIGPIPE && registered_handler.PIPE ) registered_handler.PIPE(sig);
	else if ( sig == SIGQUIT && registered_handler.QUIT ) registered_handler.QUIT(sig);
	else if ( sig == SIGUSR1 && registered_handler.USR1 ) registered_handler.USR1(sig);
	else if ( sig == SIGUSR2 && registered_handler.USR2 ) registered_handler.USR2(sig);
}

SIG& SIG::operator =(const SIG& other) {

	this -> TERM = other.TERM;
	this -> ALRM = other.ALRM;
	this -> HUP = other.HUP;
	this -> INT = other.INT;
	this -> PIPE = other.PIPE;
	this -> QUIT = other.QUIT;
	this -> USR1 = other.USR1;
	this -> USR2 = other.USR2;

	return *this;
}

bool SIG::empty() const {

	return !this -> TERM && !this -> ALRM && !this -> HUP && !this -> INT &&
		!this -> PIPE && !this -> QUIT && !this -> USR1 && !this -> USR2;
}

void SIG::clear() {

	this -> TERM = {};
	this -> ALRM = {};
	this -> HUP = {};
	this -> INT = {};
	this -> PIPE = {};
	this -> QUIT = {};
	this -> USR1 = {};
	this -> USR2 = {};
}

void SIG::install() {

	if ( sig_registered )
		throw std::runtime_error("signal handler cannot be registered, another handler is already registered");

	struct sigaction exit_action, ignore_action;

	exit_action.sa_handler = die_handler;
	sigemptyset(&exit_action.sa_mask);
	exit_action.sa_flags = 0;

	ignore_action.sa_handler = SIG_IGN;
	sigemptyset(&ignore_action.sa_mask);
	ignore_action.sa_flags = 0;

	sig_registered = true;
	registered_handler = *this;

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

	if ( !sig_registered )
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

	sig_registered = false;
	registered_handler.clear();
}

bool SIG::is_registered() {

	return sig_registered;
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
