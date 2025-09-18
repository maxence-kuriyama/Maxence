#pragma once

#include "lib/nlohmann/json.hpp"
#include "lib/utils/coordinate.h"

// �������̎w����̗�����ێ�����N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
class History {
public:
	Coordinate history[2];
	int cancelCnt = 0; // ����O�܂ł̏���ێ����Ă��邩 (<= 2)

	void initialize() {
		for (int i = 0; i < 2; i++) {
			history[i].clear();
		}
		cancelCnt = 0;
	}

	// �����ɒǉ�
	void add(int global_x, int global_y, int local_x, int local_y, int nextField) {
		Coordinate c = { global_x, global_y, local_x, local_y, nextField };
		history[1] = history[0];
		history[0] = c;
		if (cancelCnt < 2) cancelCnt++;
	}

	// �L�����Z���\���ǂ�������
	bool canCancel() {
		return (cancelCnt > 0);
	}

	// �L�����Z���ł�����1�A�ł��Ȃ�������0��Ԃ�
	int goBack() {
		if (canCancel()) {
			history[0] = history[1];
			history[1].clear();
			cancelCnt--;
			return 1;
		}
		return 0;
	}

	Coordinate last() {
		return history[0];
	}

	nlohmann::json json() {
		nlohmann::json result = {
			{"history",
				{
					{
						history[0].global_x,
						history[0].global_y,
						history[0].x,
						history[0].y,
						history[0].last_field,
					},
					{
						history[1].global_x,
						history[1].global_y,
						history[1].x,
						history[1].y,
						history[1].last_field,
					},
				}
			},
			{"cancelCnt", cancelCnt},
		};
		return result;
	}

	void restore(nlohmann::json data) {
		initialize();
		int cnt = data["cancelCnt"];
		for (int i = cnt - 1; i >= 0; i--) {
			nlohmann::json hist_data = data["history"][i];
			add(hist_data[0], hist_data[1], hist_data[2], hist_data[3], hist_data[4]);
		}
	}
};