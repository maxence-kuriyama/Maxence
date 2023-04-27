#pragma once

#include <iostream>
#include <fstream>
#include <time.h>
#include "lib/vect.h"
#include "lib/logger.h"

using namespace Eigen;

const float ROTATE_INTERVAL(3600.0); // 1 hour
const float BACKUP_INTERVAL(300.0);  // 5 minutes

// 学習機械のパラメータをファイルに読み書きするクラス
class Loader {
private:
	string input_file = "./data/parameter.dat";

public:

	Loader() {}

	~Loader() {}

	void read(int machine_size, int* layer_size, ::Affine** layers) {
		ifstream ifs(input_file, ios::in);

		if (ifs.fail()) {
			Logger::ss << "Not exists file: " << input_file;
			Logger::log();
			return;
		}
		if (!compare_signature(ifs, machine_size, layer_size)) {
			Logger::ss << "Signature is different";
			Logger::log();
			exit(1);
		}

		string line;
		for (int l = 0; l < machine_size; l++) {
			::Affine* layer = layers[l];
			for (int r = 0; r < layer_size[l]; r++) {
				getline(ifs, line);
				stringstream ss{ line };
				for (int c = 0; c < layer_size[l + 1]; c++) {
					layer->param(r, c) = get_single_value(ss);
				}
			}

			getline(ifs, line);
			stringstream ss{ line };
			for (int i = 0; i < layer_size[l + 1]; i++) {
				layer->bias(i) = get_single_value(ss);
			}
		}
		ifs.close();

		Logger::ss << "Successfully loaded: " << input_file;
		Logger::log();
	}

private:

	double get_single_value(stringstream& ss) {
		string value_str;
		getline(ss, value_str, ',');
		return stod(value_str);
	}

	bool compare_signature(ifstream& ifs, int machine_size, int* layer_size) {
		string line;
		getline(ifs, line);
		stringstream ss;
		make_signature(ss, machine_size, layer_size);
		return (line == ss.str());
	}

	void make_signature(stringstream& ss, int machine_size, int* layer_size) {
		ss << "machine_size:" << machine_size << ",";
		ss << "layer_size:";
		for (int l = 0; l <= machine_size; l++) {
			ss << layer_size[l] << ",";
		}
	}
};
