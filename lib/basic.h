#pragma once


//======================================
//   �}�E�X�֘A
//======================================

class Mouse {
public:
	int x;
	int y;
	int setx = 0;
	int sety = 0;
	int wheel;
	unsigned int button[8];

	//�}�E�X��Ԃ̎擾
	int update() {
		if (GetMousePoint(&x, &y) == -1) return -1;
		int MouseInput = GetMouseInput();
		for (int i = 0; i < 8; i++) {
			if (MouseInput & (1 << i)) {
				button[i]++;
			}
			else {
				button[i] = 0;
			}
		}
		wheel = GetMouseWheelRotVol();
		return 0;
	}

	void set() {
		setx = x;
		sety = y;
	}

	//��`�̏�Ƀ}�E�X�J�[�\�������邩����
	int onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return x > LeftUpx && x < RightDownx && y > LeftUpy && y < RightDowny;
	}
};



//======================================
//   �J�����֘A
//======================================


class Camera {
public:
	VECTOR pos;

	void initialize() {
		pos.x = 320.0;
		pos.y = 240.0;
		pos.z = -300.0;
	}

	void move(double dx, double dy, VECTOR Origin) {
		VECTOR tmp1 = VSub(pos, Origin);
		VECTOR tmp2 = VNorm(tmp1);
		pos.y += 2.0 * dy / cosh((double)tmp2.y);
		pos.x = Origin.x + tmp1.x * cos(dx / 100.0) + tmp1.z * sin(dx / 100.0);
		pos.z = Origin.z - tmp1.x * sin(dx / 100.0) + tmp1.z * cos(dx / 100.0);
	}
};



//======================================
//   �R�����g�`��֘A
//======================================

void DrawRoundBox(int x, int y, int r, int w, int h, int Color)
{
	unsigned int Black = GetColor(0, 0, 0);
	/* �l���̉~ */
	DrawCircle(x, y, r, Color, TRUE);
	DrawCircle(x + w, y, r, Color, TRUE);
	DrawCircle(x, y + h, r, Color, TRUE);
	DrawCircle(x + w, y + h, r, Color, TRUE);
	DrawCircle(x, y, r, Black, FALSE);
	DrawCircle(x + w, y, r, Black, FALSE);
	DrawCircle(x, y + h, r, Black, FALSE);
	DrawCircle(x + w, y + h, r, Black, FALSE);

	/* �~�ɔ��Ȃ������` */
	DrawBox(x - r, y, x + w + r + 1, y + h, Color, TRUE);
	DrawBox(x, y - r, x + w, y + h + r + 1, Color, TRUE);
	DrawLine(x, y - r, x + w + 1, y - r, Black, FALSE);
	DrawLine(x, y + h + r + 1, x + w + 1, y + h + r + 1, Black, FALSE);
	DrawLine(x - r, y, x - r, y + h + 1, Black, FALSE);
	DrawLine(x + w + r + 1, y, x + w + r + 1, y + h + 1, Black, FALSE);
}

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor) {
	char TempStr[3];
	int StrLen;
	int i;
	int DrawX;
	int DrawY;
	int CharLen;
	int DrawWidth;
	int BoxWidth;
	int BoxHeight;
	double rate = 1.4;	//�f�t�H���g�̃t�H���g�T�C�Y20�ɑ΂��āA���̓t�H���g�T�C�Y28�ɂ��邽��1.4�{
	//�w�肵���t�H���g�n���h���̃t�H���g�T�C�Y�𔽉f������ɂ͂ǂ�����΂悢�̂�
	int r = 5;	//�l�p�`�̊ۊp�̔��a

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	//��x��Ԃł͂��邪�A��x�����o���̑傫���𑪂��Ă���A�ꕶ�����`�悷��
	DrawX = x;
	BoxWidth = 0;
	BoxHeight = AddY * rate;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX) {
			DrawX = x;
			BoxWidth = RightX - x;
			BoxHeight += AddY * rate;
		}
		// �`����W�����炷
		DrawX += DrawWidth;
		i += CharLen;
	}
	if (DrawX - x + 5 > BoxWidth) { BoxWidth = DrawX - x + 5; }
	DrawRoundBox(x, y, r, BoxWidth, BoxHeight, BoxColor);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX)
		{
			DrawX = x;
			DrawY += AddY * rate;
		}
		// �P�����`��
		DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
		// �`����W�����炷
		DrawX += DrawWidth;
		// �`�悷�镶����i�߂�
		i += CharLen;
	}
}

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor) {
	char TempStr[3];
	int StrLen;
	int i;
	int DrawX;
	int DrawY;
	int CharLen;
	int DrawWidth;
	int BoxWidth;
	int BoxHeight;
	double rate = 1.2;	//�f�t�H���g�̃t�H���g�T�C�Y20�ɑ΂��āA���̓t�H���g�T�C�Y24�ɂ��邽��1.2�{
	int Counter = cnt;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// �w�肳�ꂽ�������ŕ`�����߂�
		if (Counter <= 0) break;

		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
			Counter--;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
			Counter--;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX)
		{
			DrawX = x;
			DrawY += AddY * rate;
		}
		// �P�����`��
		DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
		// �`����W�����炷
		DrawX += DrawWidth;
		// �`�悷�镶����i�߂�
		i += CharLen;
	}
}

int MultiByteLength(const char* String) {
	int StrLen;
	int i;
	int CharLen;
	int Counter = 0;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			CharLen = 2;
			Counter++;
		}
		else {
			CharLen = 1;
			Counter++;
		}
		i += CharLen;
	}
	return Counter;
}