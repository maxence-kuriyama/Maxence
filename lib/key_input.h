
//���S�𓮂���
if (Key[KEY_INPUT_AT] == 1) {
	if (AcRate >= 0.5) { AcRate = 0.0; titleX = 445; titleY = 400; }
	else { AcRate += 1.0; }
}


//�����F�̕ύX
if (Key[KEY_INPUT_I] == 1) {
	if (StringColor == Black) { StringColor = White; }
	else { StringColor = Black; }
}


//�f�[�^�̕ۑ�
if (Key[KEY_INPUT_O] == 1) {
	//save parameters
}


//���y,SE�̗L��
if (Key[KEY_INPUT_P] == 1) {
	//musicFlg = (musicFlg + 1) % 2;
	tama[0].sound = (tama[0].sound + 1) % 2;
}


//�J�b�g�C��������
if (Key[KEY_INPUT_C] == 1) {
	cutinFlg = 1;
}


//COM�̏o�͂�����
if (Key[KEY_INPUT_V] == 1) {
	likeliFlg = (likeliFlg + 1) % 3;
}


//�R�����g������
if (Key[KEY_INPUT_K] == 1) {
	commentFlg = (commentFlg + 1) % 2;
}


//AI�̃E�F�C�g��ύX����
if (Key[KEY_INPUT_M] == 1) {
	waitOnCOM++;
}
else if (Key[KEY_INPUT_N] == 1) {
	waitOnCOM--;
}


//�f�o�b�O���[�h����
//if (Key[KEY_INPUT_D] == 1) {
//	debugFlg = 0;
//}


//�G���f�B���O���[�h
if (Key[KEY_INPUT_MINUS] == 1) {
	if (Gameflg != -4) { Gameflg = -4; }
	else { StopMusic(); Gameflg = 0; }
	end_cnt = 0;
}