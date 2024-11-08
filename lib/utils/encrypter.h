#pragma once

#include <iostream>
#include <fstream>
#include "lib/nlohmann/json.hpp"
#include "lib/utils/logger.h"


const int ENCRYPTER_MAX_STR_LENGTH(2000);
const int ENCRYPTER_PREFIX_LENGTH(9);

class Encrypter {
private:
	string data_file = "./data/save.dat";

public:
	int hash_key[8] = { 0 };
	int period = 2;

	Encrypter() {
		generateKeys();
	}

	Encrypter(const string path) {
		generateKeys();
		data_file = path;
	}

	void write(nlohmann::json src) {
		ofstream ofs;
		ofs.open(data_file, ios::out);
		if (ofs.bad()) {
			Logger::ss << "Failed to open file: " << data_file;
			Logger::log();
		}
		
		loggingWrite(src);
		writePrefix(ofs);

		char encrypted[ENCRYPTER_MAX_STR_LENGTH];
		encrypt(src.dump().c_str(), encrypted);
		ofs << encrypted;
		ofs.close();

		Logger::ss << "Successfully saved: " << data_file;
		Logger::log();
	}

	nlohmann::json read() {
		nlohmann::json result;

		ifstream ifs(data_file, ios::in);
		if (ifs.fail()) {
			Logger::ss << "Not exists file: " << data_file;
			Logger::log();
			return result;
		}

		string buf;
		getline(ifs, buf);

		char decrypted[ENCRYPTER_MAX_STR_LENGTH];
		decrypt(buf.c_str(), decrypted);
		loggingRead(decrypted);

		result = nlohmann::json::parse(decrypted);

		Logger::ss << "Successfully loaded: " << data_file;
		Logger::log();

		return result;
	}

private:

	void writePrefix(ofstream& ofs) {
		char hash_key_chr[9];
		for (int i = 0; i < 8; i++) {
			hash_key_chr[i] = charize(hash_key[i]);
		}
		hash_key_chr[8] = '\0';

		ofs << hash_key_chr
			<< charize(period);
	}

	void encrypt(const char src[], char trg[]) {
		int cnt = 0;
		while (src[cnt] != '\0') {
			int turn = (cnt / 8) % period;
			trg[cnt] = src[cnt] + hash_key[cnt % 8] + turn;
			if (++cnt >= ENCRYPTER_MAX_STR_LENGTH - 1) break;
		}
		trg[cnt] = '\0';
	}

	void decrypt(const char src[], char trg[]) {
		getKeys(src);

		int cnt = 0;
		while (src[cnt + ENCRYPTER_PREFIX_LENGTH] != '\0') {
			int steped_cnt = cnt + ENCRYPTER_PREFIX_LENGTH;
			int turn = (cnt / 8) % period;
			trg[cnt] = src[steped_cnt] - hash_key[cnt % 8] - turn;
			if (++cnt >= ENCRYPTER_MAX_STR_LENGTH - 1) break;
		}
		trg[cnt] = '\0';
	}

	void getKeys(const char src[]) {
		for (int i = 0; i < 8; i++) {
			hash_key[i] = intize(src[i]);
		}
		period = intize(src[8]);
	}

	void generateKeys() {
		for (int i = 0; i < 8; i++) {
			hash_key[i] = rand() % 60;
		}
		period = 2 + rand() % 4;
	}

	char charize(int key) {
		return (char)(65 + key);
	}

	int intize(char c) {
		return (int)c - 65;
	}


	/*===========================*/
	//    Logging
	/*===========================*/
	void loggingWrite(nlohmann::json src) {
		Logger::ss << "Save json: " << src.dump();
		Logger::log();

		loggingHashKey();
	}

	void loggingRead(const char src[]) {
		loggingHashKey();

		Logger::ss << "Read json: " << src;
		Logger::log();
	}

	void loggingHashKey() {
		Logger::ss << "Encryption key: ";
		for (int i = 0; i < 8; i++) {
			Logger::ss << hash_key[i] << ",";
		}
		Logger::log();
	}
};
