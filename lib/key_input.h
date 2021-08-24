
//ロゴを動かす
if (Key[KEY_INPUT_AT] == 1) {
	if (AcRate >= 0.5) { AcRate = 0.0; titleX = 445; titleY = 400; }
	else { AcRate += 1.0; }
}


//文字色の変更
if (Key[KEY_INPUT_I] == 1) {
	if (StringColor == Black) { StringColor = White; }
	else { StringColor = Black; }
}


//データの保存
if (Key[KEY_INPUT_O] == 1) {
	//save parameters
}


//音楽,SEの有無
if (Key[KEY_INPUT_P] == 1) {
	//musicFlg = (musicFlg + 1) % 2;
	tama[0].sound = (tama[0].sound + 1) % 2;
}


//カットインを入れる
if (Key[KEY_INPUT_C] == 1) {
	cutinFlg = 1;
}


//COMの出力を見る
if (Key[KEY_INPUT_V] == 1) {
	likeliFlg = (likeliFlg + 1) % 3;
}


//コメントを消す
if (Key[KEY_INPUT_K] == 1) {
	commentFlg = (commentFlg + 1) % 2;
}


//AIのウェイトを変更する
if (Key[KEY_INPUT_M] == 1) {
	waitOnCOM++;
}
else if (Key[KEY_INPUT_N] == 1) {
	waitOnCOM--;
}


//デバッグモード解除
//if (Key[KEY_INPUT_D] == 1) {
//	debugFlg = 0;
//}


//エンディングモード
if (Key[KEY_INPUT_MINUS] == 1) {
	if (Gameflg != -4) { Gameflg = -4; }
	else { StopMusic(); Gameflg = 0; }
	end_cnt = 0;
}