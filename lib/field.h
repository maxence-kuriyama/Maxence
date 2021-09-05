
class Field {
private:
	int filled() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (state[i][j] == 0) return 0;
			}
		}
		return 1;
	}

public:
	int state[3][3];	// 0:None, 1:Black, -1:White

	Field() {
		initialize();
	}

	void initialize() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				state[i][j] = 0;
			}
		}
	}

	int victory() {
		int vict = 0;
		int temp = 0;

		for (int k = 0; k < 3; ++k) {
			temp = state[0][k];
			if (temp == state[1][k] && temp == state[2][k]) {
				//if(temp != 0) cout << "‰¡" << k << endl;
				if (vict == 0) {
					vict = temp;
				}
				else if (vict != temp && temp != 0) {
					vict = 100;
					break;
				}
			}
			temp = state[k][0];
			if (temp == state[k][1] && temp == state[k][2]) {
				//if(temp != 0) cout << "c" << k << endl;
				if (vict == 0) {
					vict = temp;
				}
				else if (vict != temp && temp != 0) {
					vict = 100;
					break;
				}
			}
		}
		temp = state[0][0];
		if (temp == state[1][1] && temp == state[2][2]) {
			//if (temp != 0) cout << "¶ŽÎ‚ß" << endl;
			if (vict == 0) {
				vict = temp;
			}
			else if (vict != temp && temp != 0) {
				vict = 100;
			}
		}
		temp = state[2][0];
		if (temp == state[1][1] && temp == state[0][2]) {
			//if (temp != 0) cout << "‰EŽÎ‚ß" << endl;
			if (vict == 0) {
				vict = temp;
			}
			else if (vict != temp && temp != 0) {
				vict = 100;
			}
		}
		if (filled() == 1 && vict == 0) vict = 10;
		return vict;
	} // 0:on-game, 1:Black, -1:White, 10: Draw, 100:Error

	int update(int i, int j, int side) {
		if (victory() != 0) return -1;
		if (state[i][j] == 0) {
			state[i][j] = side;
			return 0;
		}
		return -1;
	}

	Field operator = (const Field& src) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				state[i][j] = src.state[i][j];
			}
		}
		return src;
	}
};

