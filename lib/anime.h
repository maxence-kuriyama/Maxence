#pragma once

// 試合中のアニメーションクラス
// 短いアニメーションを必要とするクラスのメンバ変数として使う
class Anime {
private:
	string mode = "CUTIN";
	int cnt = 0;

public:
	int flg = 0;
	int image0;
	int image1;

	void setMode(string m) {
		mode = m;
	}

	void update() {
		if (mode == "CUTIN") {
			if (flg == 1) {
				SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
				DrawGraph(pow(cnt - 15, 2.0) * 640 / pow(15.0, 2.0), 0, image1, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
				DrawGraph(pow(cnt - 15, 2.0) * 640 / pow(15.0, 2.0), 0, image0, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (cnt > 15) {
					flg = 2;
				}
				cnt++;
			}
			else if (flg == 2) {
				SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
				DrawGraph(0, 0, image1, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
				DrawGraph(0, 0, image0, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (cnt > 105) {
					flg = 3;
				}
				cnt++;
			}
			else if (flg == 3) {
				SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
				DrawGraph(-pow(cnt - 105, 2.0) * 640 / pow(15.0, 2.0), 0, image1, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
				DrawGraph(-pow(cnt - 105, 2.0) * 640 / pow(15.0, 2.0), 0, image0, FALSE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (cnt > 120) {
					flg = 0;
					cnt = 0;
				}
				cnt++;
			}
		}
	}
};