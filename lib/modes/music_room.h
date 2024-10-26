#pragma once

#include <sstream>
#include <iomanip>
#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"
#include "lib/modes/common/fireflower.h"

const int MUSIC_NUM(15);
const int MUSIC_MAX_INDEX(MUSIC_NUM);
const int MUSIC_ROOM_FIRE_FLOWER_NUM(2);

const int MUSIC_LEFT_X(30);
const int MUSIC_LEFT_X2(300);
const int MUSIC_DIV_X(10);
const int MUSIC_TOP_Y(40);
const int MUSIC_DIV_Y(40);

// 音楽室モード
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 215, 0);

	struct MusicInfo {
		int id;
		string ext;
		string name;
		int who;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					MESSAGE_WHO_YELLOW,	"オープニング曲。一番最初に作りました。\n冒険に胸躍らせるわくわく感が伝われば嬉しいです♪\nえ、本編で冒険してないって？ 気のせいでしょ。" },
		{ 2,	"ogg",	"Mr.K is talking to you",			MESSAGE_WHO_YELLOW,	"Mr.Kのテーマ。チュートリアルで流れます。\n突然語りかけてくるアイツの怪しさを表現しました♪\nちなみに彼は全裸サングラスです。イカしてますね！" },
		{ 3,	"ogg",	"海と樹海と私",						MESSAGE_WHO_YELLOW,	"世界（部屋）を包み込む不穏な曲...樹海！樹海！\n樹海！樹海！樹海！樹海！樹海！樹海！樹海！樹海！\nさて問題！いま何回「樹海」っていったでしょうか？" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			MESSAGE_WHO_RED,	"激昂して襲いかかってくる赤いアイツのテーマ。\n赤いアイツはモデルとなる人がいるとかいないとか。\nおっと誰か来たようだ..." },
		{ 5,	"ogg",	"Mr.K is smiling on you",			MESSAGE_WHO_GREEN,	"仲間の死を悼む緑のアイツのテーマ。\n緑のアイツは割といいやつなイメージがあります。\n彼は最後にいったい何を悟ったのでしょうか？？" },
		{ 6,	"ogg",	"Mr.K is lying to you",				MESSAGE_WHO_BLUE,	"謀略に長けた青いアイツのテーマ。\n野心から仲間を切り捨て王を目指す悲しいアイツ。\n狡猾さと悲しさが表現できていれば幸いです♪" },
		{ 7,	"ogg",	"運命の平衡点",						MESSAGE_WHO_YELLOW,	"真実を悟る時の曲。曲名は脚本のA氏による命名。\n冬に寒い寒いって思いながら作った記憶があります。\n運命にも平衡点はあるんだよなぁ..." },
		{ 8,	"ogg",	"Mr.K is playing with you",			MESSAGE_WHO_YELLOW,	"操作キャラだった黄色いアイツと戦う時のテーマ。\nチュートリアル曲のアレンジになっています。\nMr.Kらしい曲を目指しました♪" },
		{ 9,	"ogg",	"鹿は静かに森に帰る",				MESSAGE_WHO_YELLOW,	"表エンディング曲。曲名は脚本のA氏による命名。\nなぜ鹿なのか？本当に鹿なのか？なぜ森に帰るのか？\nそれは鹿のみが知っているのである。" },
		{ 10,	"ogg",	"Waterly blue mountain",			MESSAGE_WHO_YELLOW,	"プログラム担当のT氏との合作。ボーナストラック。\n曲名の由来は製作者の一人の名前という噂が...\nこれ以上書くと怒られちゃいそうなのでこの辺で。" },
		{ 11,	"ogg",	"Dear The Deer",					MESSAGE_WHO_YELLOW,	"裏ルート突入！！謎の鹿のテーマ曲。\n戦い前の静かだけどバチバチとした雰囲気を\n感じられる曲になっていれば嬉しいです♪" },
		{ 12,	"ogg",	"The biggest deer",					MESSAGE_WHO_DEER,	"裏ラスボス戦の曲。直訳すると最も大きな鹿。\n空間を操る能力があるとか。\n得体の知れない感じが表現できていれば嬉しいです♪" },
		{ 13,	"ogg",	"行き場を失った博士たちのブルース",	MESSAGE_WHO_PLAYER,	"真エンディング。ここまで遊んでくれてありがとう。\nあなたこそが本当のMr.Kなのかもしれない。\nブルースって多分こんな感じだよね、たぶん..." },
		{ 14,	"ogg",	"Fading memories",					MESSAGE_WHO_YELLOW,	"けっこう壮大な雰囲気の曲になりました。\n作ったはいいけど使う場面がなくて...\n...いわゆるボーナストラックというやつですね！" },
		{ 15,	"ogg",	"釣り船",							MESSAGE_WHO_YELLOW,	"ゲームオーバー時に流れます。\n一回も聞かずにクリアできたあなたはスゴイ！！\n侘び寂びを表現することを目指して作りました。" },
	};
	string defaultMsgStr = "ここは音楽室\nいつもの部屋じゃないどこかだよ";

	Menu menu;
	Button buttons[MUSIC_NUM + 1]; // 末尾の要素は「タイトルへ」
	Message msg;
	fireflower tama[MUSIC_ROOM_FIRE_FLOWER_NUM];
	int choice = -1;
	bool isLoading = false;

public:

	MusicRoom() {
		initialize();
	}

	void initialize() {
		msg.initialize();
		msg.setWithoutNext(defaultMsgStr, MESSAGE_WHO_YELLOW);
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
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize(60.0, 600.0, 520.0, 600.0);
		}
		choice = -1;
		isLoading = false;
	}

	int show(UserInput& ui, Music& music) {
		showFireFlower();
		msg.draw();
	
		int currentChoice = choose(ui);

		if (currentChoice == MUSIC_MAX_INDEX) return FLAG_TITLE;

		if (currentChoice != -1) {
			choice = currentChoice;
			music.stop();
			updateMusicDesc();
		}

		if (currentChoice != -1 || isLoading) {
			if (play(music)) {
				isLoading = false;
			}
			else {
				isLoading = true;
				music.drawLoadMsg();
			}
		}
		return FLAG_MUSIC_ROOM;
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(365, 65, strColor, "musicChoice: %d", choice);
	}

private:

	int choose(UserInput& ui) {
		return menu.choose(ui, strColorMenu);
	}

	bool reset(UserInput& ui) {}

	void updateMusicDesc() {
		MusicInfo info = infoList[choice];
		msg.setWithoutNext(info.desc, info.who);
	}

	int play(Music& music) {
		if (choice == -1) return 1;

		MusicInfo info = infoList[choice];
		string filename = getMusicFileName(info);
		if (music.musicName[0] == filename) {
			return (music.stop() && music.play(MUSIC_START_FROM_TOP));
		}
		else if (music.musicName[1] == filename) {
			return (music.swapWithoutPlay() && music.play(MUSIC_START_FROM_TOP));
		}
		else {
			music.unload(1); // 必要?
			music.load(filename.c_str());
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

	void showFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}
};
