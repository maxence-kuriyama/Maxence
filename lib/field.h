#pragma once


// �������̔Ֆʕێ��A����������s���N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
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
	int stone1;
	int stone2;
	int stone1_t;
	int stone2_t;

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
				//if(temp != 0) cout << "��" << k << endl;
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
				//if(temp != 0) cout << "�c" << k << endl;
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
			//if (temp != 0) cout << "���΂�" << endl;
			if (vict == 0) {
				vict = temp;
			}
			else if (vict != temp && temp != 0) {
				vict = 100;
			}
		}
		temp = state[2][0];
		if (temp == state[1][1] && temp == state[0][2]) {
			//if (temp != 0) cout << "�E�΂�" << endl;
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

	void draw(double baseX, double baseY, double width) {
		for (int k = 0; k < 3; ++k) {
			for (int l = 0; l < 3; ++l) {
				if (state[k][l] == 1) {
					DrawExtendGraph(baseX + width * k - 15, baseY + width * l - 15, baseX + width * k + 15, baseY + width * l + 15, stone1, TRUE);
					//DrawCircleAA(baseX + width * k, baseY + width * l, 13, 12, GetColor(50, 50, 50), TRUE);
				}
				else if (state[k][l] == -1) {
					DrawExtendGraph(baseX + width * k - 15, baseY + width * l - 15, baseX + width * k + 15, baseY + width * l + 15, stone2, TRUE);
					//DrawCircleAA(baseX + width * k, baseY + width * l, 13, 12, GetColor(255, 200, 100), TRUE);
				}
			}
		}
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

