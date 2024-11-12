#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>

// ログファイル出力用クラス
class Logger {
private:
	static Logger* _instance; // singleton
	string logfile = "./data/debug.log";
	
	static Logger* getInstance() {
		if (!_instance) {
			_instance = new Logger();
		}
		return _instance;
	}

public:
	static stringstream ss;

	static void log() {
		log(ss.str());

		// 初期化
		ss.str("");
		ss.clear(stringstream::goodbit);
	}

	static void log(string msg) {
		if (!DEBUG) return;

		Logger* logger = getInstance();
		logger->output(msg);
	}
	
	void output(string msg) {
		ofstream ofs;
		ofs.open(logfile, ios::app);
		if (ofs.bad()) exit(1);

		string date = strDatetime();
		ofs << date << msg << endl;

		ofs.close();
	}

	string strDatetime() {
		time_t timer = time(NULL);
		struct tm date;
		localtime_s(&date, &timer);
		stringstream ss;
		ss << "[" << date.tm_year + 1900;
		ss << "/" << setfill('0') << setw(2) << date.tm_mon + 1;
		ss << "/" << setfill('0') << setw(2) << date.tm_mday;
		ss << " " << setfill('0') << setw(2) << date.tm_hour;
		ss << ":" << setfill('0') << setw(2) << date.tm_min;
		ss << ":" << setfill('0') << setw(2) << date.tm_sec;
		ss << "] ";
		return ss.str();
	}
};

Logger* Logger::_instance = NULL;
stringstream Logger::ss;
