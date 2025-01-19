#pragma once
#include "dbOperations.h"



int status_to_int(std::string status) {
	if (status == std::string("Inactive")) return Inactive;
	else if (status == std::string("Active")) return Active;
	else if (status == std::string("Clarify")) return Clarify;
	else if (status == std::string("Deleted")) return Deleted;
	else return -1;
}


struct Session {

	//internal data
	db base;
	bool isLive = false;

	// user data
	int id = 0;
	int rid = 0;
	std::string username;
	std::string login;
	std::string mail;
	std::string hashPassword;
	std::string status;


	Session(const char* filepath) {
		base.start_session(filepath);
	}

	~Session() {
		if (base.open) {
			base.end_session();
		}
	}

	bool log_in(std::string log, std::string password) {

		std::string hash = std::to_string(hashing(password));
		std::vector<std::vector<std::string>> logins = base.request_names();

		bool goodLogin = false;
		for (std::vector<std::string> vec : logins) {
			if ((vec[1] == log)) {
				goodLogin = true;
			}
		}

		if (!goodLogin) {
			return false;
		}


		if (base.get_password_by_login(log) == hash) {

			std::vector<std::string> info = base.request_login(log);

			id = std::stoi(info[0]);
			username = info[1];
			this->login = log;
			mail = info[3];
			hashPassword = info[4];
			status = info[5];
			rid = std::stoi(info[6]);


			isLive = true;
			return true;
		}
		else {
			return false;
		}

	}

	void end() {
		if (base.open) {
			base.end_session();
		}
	}
};