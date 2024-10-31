#pragma once

#include <sstream>
#include <iomanip>
#include "lib/nlohmann/json.hpp"
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
const int MUSIC_MESSAGE_INT_X(30);
const int MUSIC_MESSAGE_END_X(610);

// 音楽室モード
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 215, 0);
	string save_filepath = "./data/savemr.dat";

	struct MusicInfo {
		int id;
		string ext;
		string name;
		string desc;
	};

	nlohmann::json unlockedIds = nlohmann::json::array({ 1,2 });
	struct MusicInfo infoList[MUSIC_NUM] = {};
	const struct MusicInfo infoMaster[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					"オープニング曲。一番最初に作りました。\n冒険に胸躍らせるわくわく感が伝われば嬉しいです♪\nえ、本編で冒険してないって？ 気のせいでしょ。" },
		{ 2,	"ogg",	"Mr.K is talking to you",			"Mr.Kのテーマ。チュートリアルで流れます。\n突然語りかけてくるアイツの怪しさを表現しました♪\nちなみに彼は全裸サングラスです。イカしてますね！" },
		{ 3,	"ogg",	"海と樹海と私",						"世界（部屋）を包み込む不穏な曲...樹海！樹海！\n樹海！樹海！樹海！樹海！樹海！樹海！樹海！樹海！\nさて問題！いま何回「樹海」っていったでしょうか？" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			"激昂して襲いかかってくる赤いアイツのテーマ。\n赤いアイツはモデルとなる人がいるとかいないとか。\nおっと誰か来たようだ..." },
		{ 5,	"ogg",	"Mr.K is smiling on you",			"仲間の死を悼む緑のアイツのテーマ。\n緑のアイツは割といいやつなイメージがあります。\n彼は最後にいったい何を悟ったのでしょうか？？" },
		{ 6,	"ogg",	"Mr.K is lying to you",				"謀略に長けた青いアイツのテーマ。\n野心から仲間を切り捨て王を目指す悲しいアイツ。\n狡猾さと悲しさが表現できていれば幸いです♪" },
		{ 7,	"ogg",	"運命の平衡点",						"真実を悟る時の曲。曲名は脚本のA氏による命名。\n冬に寒い寒いって思いながら作った記憶があります。\n運命にも平衡点はあるんだよなぁ..." },
		{ 8,	"ogg",	"Mr.K is playing with you",			"操作キャラだった黄色いアイツと戦う時のテーマ。\nチュートリアル曲のアレンジになっています。\nMr.Kらしい曲を目指しました♪" },
		{ 9,	"ogg",	"鹿は静かに森に帰る",				"表エンディング曲。曲名は脚本のA氏による命名。\nなぜ鹿なのか？本当に鹿なのか？なぜ森に帰るのか？\nそれは鹿のみが知っているのである。" },
		{ 10,	"ogg",	"Waterly blue mountain",			"プログラム担当のT氏との合作。ボーナストラック。\n曲名の由来は製作者の一人の名前という噂が...\nこれ以上書くと怒られちゃいそうなのでこの辺で。" },
		{ 11,	"ogg",	"Dear The Deer",					"裏ルート突入！！謎の鹿のテーマ曲。\n戦い前の静かだけどバチバチとした雰囲気を\n感じられる曲になっていれば嬉しいです♪" },
		{ 12,	"ogg",	"The biggest deer",					"裏ラスボス戦の曲。直訳すると最も大きな鹿。\n空間を操る能力があるとか。\n得体の知れない感じが表現できていれば嬉しいです♪" },
		{ 13,	"ogg",	"行き場を失った博士たちのブルース",	"真エンディング。ここまで遊んでくれてありがとう。\nあなたこそが本当のMr.Kなのかもしれない。\nブルースって多分こんな感じだよね、たぶん..." },
		{ 14,	"ogg",	"Fading memories",					"けっこう壮大な雰囲気の曲になりました。\n作ったはいいけど使う場面がなくて...\n...いわゆるボーナストラックというやつですね！" },
		{ 15,	"ogg",	"釣り船",							"ゲームオーバー時に流れます。\n一回も聞かずにクリアできたあなたはスゴイ！！\n侘び寂びを表現することを目指して作りました。" },
	};
	const struct MusicInfo lockedMusicInfo = { 0, "wav", "??????????", "この曲はまだ聞いたことがないよ" };
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
		load();
		initializeMusicInfo();
		initializeMessage();
		initializeButtons();
		initializeFireFlower();
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
			showMusicDesc();
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

	bool isUnlocked(MusicInfo& info) {
		for (nlohmann::json::iterator it = unlockedIds.begin(); it != unlockedIds.end(); ++it) {
			if (*it == info.id) return true;
		}
		return false;
	}

	/*===========================*/
	//    描画系関数
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

	/*===========================*/
	//    Initialization
	/*===========================*/
	void initializeMusicInfo() {
		for (int i = 0; i < MUSIC_NUM; ++i) {
			struct MusicInfo info = infoMaster[i];
			if (isUnlocked(info)) {
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
	}

	void initializeFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize(60.0, 600.0, 520.0, 600.0);
		}
	}

	/*===========================*/
	//    Save and Load
	/*===========================*/
public:
	void save() {
		Encrypter encrypter(save_filepath);
		nlohmann::json data = {
			{"unlockedIds", unlockedIds},
		};
		encrypter.write(data);
	}

private:
	void load() {
		Encrypter encrypter(save_filepath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "MusicRoom loaded: " << res.dump();
		Logger::log();

		unlockedIds = res["unlockedIds"];
	}
};
