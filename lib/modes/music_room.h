#pragma once

#include <sstream>
#include <iomanip>
#include "lib/const.h"
#include "lib/mode.h"
#include "lib/utils/music.h"
#include "lib/utils/music_unlocker.h"
#include "lib/components/menu.h"
#include "lib/components/fireflower.h"
#include "lib/components/message.h"
#include "./music_room/bg_character.h"

const int MUSIC_NUM(15);
const int MUSIC_MAX_INDEX(MUSIC_NUM);
const int MUSIC_ROOM_FIRE_FLOWER_NUM(2);

const int MUSIC_LEFT_X(50);
const int MUSIC_LEFT_X2(320);
const int MUSIC_DIV_X(10);
const int MUSIC_TOP_Y(40);
const int MUSIC_DIV_Y(40);
const int MUSIC_SPEAKER_ICON_SIZE(36);
const int MUSIC_MESSAGE_INT_X(30);
const int MUSIC_MESSAGE_END_X(610);

// 音楽室モード
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 215, 0);
	int iconSpeaker = LoadGraph("graph/speaker.png");

	struct MusicInfo {
		int id;
		string ext;
		string name;
		int who;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {};
	const struct MusicInfo infoMaster[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					BG_WHO_NONE,			"オープニング曲。一番最初に作りました。\n冒険に胸躍らせるわくわく感が伝われば嬉しいです♪\nえ、本編で冒険してないって？ 気のせいでしょ。" },
		{ 2,	"ogg",	"Mr.K is talking to you",			BG_WHO_PL_YELLOW,	"Mr.Kのテーマ。チュートリアルで流れます。\n突然語りかけてくるアイツの怪しさを表現しました♪\nちなみに彼は全裸サングラスです。イカしてますね！" },
		{ 3,	"ogg",	"海と樹海と私",						BG_WHO_NONE,			"世界（部屋）を包み込む不穏な曲...樹海！樹海！\n樹海！樹海！樹海！樹海！樹海！樹海！樹海！樹海！\nさて問題！いま何回「樹海」っていったでしょうか？" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			BG_WHO_RED,			"激昂して襲いかかってくる赤いアイツのテーマ。\n赤いアイツはモデルとなる人がいるとかいないとか。\nおっと誰か来たようだ..." },
		{ 5,	"ogg",	"Mr.K is smiling on you",			BG_WHO_GREEN,		"仲間の死を悼む緑のアイツのテーマ。\n緑のアイツは割といいやつなイメージがあります。\n彼は最後にいったい何を悟ったのでしょうか？？" },
		{ 6,	"ogg",	"Mr.K is lying to you",				BG_WHO_BLUE,			"謀略に長けた青いアイツのテーマ。\n野心から仲間を切り捨て王を目指す悲しいアイツ。\n狡猾さと悲しさが表現できていれば幸いです♪" },
		{ 7,	"ogg",	"運命の平衡点",						BG_WHO_NONE,			"真実を悟る時の曲。曲名は脚本のA氏による命名。\n冬に寒い寒いって思いながら作った記憶があります。\n運命にも平衡点はあるんだよなぁ..." },
		{ 8,	"ogg",	"Mr.K is playing with you",			BG_WHO_YELLOW,		"操作キャラだった黄色いアイツと戦う時のテーマ。\nチュートリアル曲のアレンジになっています。\nMr.Kらしい曲を目指しました♪" },
		{ 9,	"ogg",	"鹿は静かに森に帰る",				BG_WHO_NONE,			"表エンディング曲。曲名は脚本のA氏による命名。\nなぜ鹿なのか？本当に鹿なのか？なぜ森に帰るのか？\nそれは鹿のみが知っているのである。" },
		{ 10,	"ogg",	"Waterly blue mountain",			BG_WHO_NONE,			"プログラム担当のT氏との合作。ボーナストラック。\n曲名の由来は製作者の一人の名前という噂が...\nこれ以上書くと怒られちゃいそうなのでこの辺で。" },
		{ 11,	"ogg",	"Dear The Deer",					BG_WHO_DEER,			"裏ルート突入！！謎の鹿のテーマ曲。\n戦い前の静かだけどバチバチとした雰囲気を\n感じられる曲になっていれば嬉しいです♪" },
		{ 12,	"ogg",	"The biggest deer",					BG_WHO_DEER,			"裏ラスボス戦の曲。直訳すると最も大きな鹿。\n空間を操る能力があるとか。\n得体の知れない感じが表現できていれば嬉しいです♪" },
		{ 13,	"ogg",	"行き場を失った博士たちのブルース",	BG_WHO_NONE,			"真エンディング。ここまで遊んでくれてありがとう。\nあなたこそが本当のMr.Kなのかもしれない。\nブルースって多分こんな感じだよね、たぶん..." },
		{ 14,	"ogg",	"Fading memories",					BG_WHO_NONE,			"けっこう壮大な雰囲気の曲になりました。\n作ったはいいけど使う場面がなくて...\n...いわゆるボーナストラックというやつですね！" },
		{ 15,	"ogg",	"釣り船",							BG_WHO_NONE,			"ゲームオーバー時に流れます。\n一回も聞かずにクリアできたあなたはスゴイ！！\n侘び寂びを表現することを目指して作りました。" },
	};
	const struct MusicInfo lockedMusicInfo = { 0, "wav", "??????????", BG_WHO_NONE, "この曲はまだ聞いたことがないよ" };
	string defaultMsgStr = "ここは音楽室\nいつもの部屋じゃないどこかだよ";

	Menu menu;
	Button buttons[MUSIC_NUM + 1]; // 末尾の要素は「タイトルへ」
	Message msg;
	FireFlower tama[MUSIC_ROOM_FIRE_FLOWER_NUM];
	BgCharacter bg;
	int choice = -1;
	bool isLoading = false;

public:

	MusicRoom() {
		initialize();
	}

	void initialize() {
		initializeMusicInfo();
		initializeMessage();
		initializeButtons();
		initializeFireFlower();
		initializeBg();
		choice = -1;
		isLoading = false;
	}

	int show() {
		SetBackgroundColor(0, 0, 0);
		bg.draw();
		showFireFlower();
		msg.draw();
		drawSpeakerIcon();
	
		int currentChoice = menu.choose(strColorMenu);

		// タイトルへ戻る
		if (currentChoice == MUSIC_MAX_INDEX) return MODE_TITLE;

		// 別の曲を選択
		if (currentChoice != -1) {
			choice = currentChoice;
			Music::stop();
			setBgCharacter();
			showMusicDesc();
		}

		if (currentChoice != -1 || isLoading) {
			if (play()) {
				isLoading = false;
			}
			else {
				isLoading = true;
				Music::drawLoadMsg();
			}
		}
		return MODE_MUSIC_ROOM;
	}

	void route(Mode& mode, int res) {
		if (res == MODE_TITLE) {
			initialize();
			Music::unloadAll();
			mode.goTitle();
		}
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(365, 65, strColor, "musicChoice: %d", choice);
	}

private:

	int play() {
		if (choice == -1) return 1;

		MusicInfo info = infoList[choice];
		string filename = getMusicFileName(info);
		if (Music::getMusicName(0) == filename) {
			return (Music::stop() && Music::play(MUSIC_START_FROM_TOP));
		}
		else if (Music::getMusicName(1) == filename) {
			return (Music::swapWithoutPlay() && Music::play(MUSIC_START_FROM_TOP));
		}
		else {
			Music::unload(1); // 必要?
			Music::load(filename.c_str());
			return 0;
		}
	}

	string getMusicFileName(MusicInfo& info) {
		ostringstream ss;
		ss << "./sound/bgm";
		ss << setfill('0') << setw(2) << info.id;
		ss << "." << info.ext;
		return ss.str();
	}

	void setBgCharacter() {
		if (choice == -1) return;

		MusicInfo info = infoList[choice];
		bg.set(info.who);
	}

	/*===========================*/
	//    描画系private関数
	/*===========================*/
	void showMusicDesc() {
		MusicInfo info = infoList[choice];
		msg.setWithoutNext(info.desc, MESSAGE_WHO_DESC);
	}

	void showFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}

	void drawSpeakerIcon() {
		if (choice == -1) return;

		int x, y;
		if (choice < (MUSIC_NUM + 1) / 2) {
			x = choice * MUSIC_DIV_X + MUSIC_LEFT_X - MUSIC_SPEAKER_ICON_SIZE - 5;
			y = choice * MUSIC_DIV_Y + MUSIC_TOP_Y - MUSIC_SPEAKER_ICON_SIZE / 3;
		}
		else if (choice < MUSIC_NUM) {
			x = (choice - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2 - MUSIC_SPEAKER_ICON_SIZE - 5;
			y = (choice - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y - MUSIC_SPEAKER_ICON_SIZE / 3;
		}
		DrawExtendGraph(x, y, x + MUSIC_SPEAKER_ICON_SIZE, y + MUSIC_SPEAKER_ICON_SIZE, iconSpeaker, TRUE);
	}

	/*===========================*/
	//    Initialization
	/*===========================*/
	void initializeMusicInfo() {
		for (int i = 0; i < MUSIC_NUM; ++i) {
			struct MusicInfo info = infoMaster[i];
			if (MusicUnlocker::isUnlocked(info.id)) {
				infoList[i] = info;
			}
			else {
				infoList[i] = lockedMusicInfo;
			}
		}
	}

	void initializeMessage() {
		msg.initialize(MUSIC_MESSAGE_INT_X, MUSIC_MESSAGE_END_X);
		msg.setWithoutNext(defaultMsgStr, MESSAGE_WHO_DESC);
	}

	void initializeButtons() {
		for (int i = 0; i < (MUSIC_NUM + 1) / 2; ++i) {
			int x = i * MUSIC_DIV_X + MUSIC_LEFT_X;
			int y = i * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		for (int i = (MUSIC_NUM + 1) / 2; i < MUSIC_NUM; ++i) {
			int x = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2;
			int y = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		buttons[MUSIC_MAX_INDEX].initializeUsingLabelLen(520, 340, "タイトルへ");
		menu.set(buttons, MUSIC_NUM + 1);
		menu.setStep((MUSIC_NUM + 1) / 2);
	}

	void initializeFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize(60.0, 600.0, 520.0, 600.0);
		}
	}

	void initializeBg() {
		bg.initialize();
	}
};
