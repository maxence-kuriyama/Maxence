#pragma once

#include <sstream>
#include "lib/mode.h"
#include "lib/utils/encrypter.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"
#include "./scenario/gameover.h"

const int SCENE_ACTION_EQ(11);
const int SCENE_ACTION_WAIT(12);
const int SCENE_ACTION_ENDING(13);
const int SCENE_ACTION_LOSE(14);

const int SCENARIO_ID_BEFORE_RED(17);
const int SCENARIO_ID_AFTER_RED(25);
const int SCENARIO_ID_BEFORE_GREEN(37);
const int SCENARIO_ID_AFTER_GREEN(45);
const int SCENARIO_ID_BEFORE_BLUE(56);
const int SCENARIO_ID_AFTER_BLUE(64);
const int SCENARIO_ID_BEFORE_YELLOW(79);
const int SCENARIO_MAX_EQ_CNT(4);

const int MESSAGE_WHO_CARD(11);

const string SCENARIO_MSG_LOST_BATTLE("敗北してしまった…\n目の前が暗くなる…");

// シナリオ管理用クラス
class Scenario : public ScenarioBase {

public:
	Sprite card;

	Scenario() {
		imgRoom = LoadGraph("graph/room0.png");
		imgCard = LoadGraph("graph/card.bmp");

		mrK[0].set(170, 30);
		mrK[0].setExpand(SPRITE_EXPAND_RATE, 72, 85);
		mrK[0].setSerialImages(16, "graph/move", 1);
		mrK[1].set(320, 80, "graph/sprite12.png", 1);
		mrK[2].set(200, 230, "graph/sprite13.png", 1);
		mrK[3].set(500, 260, "graph/sprite14.png", 1);
		deer.set(300, 160, "graph/sprite15.png", 0);
		card.set(230, 160, "graph/sprite16.png", 0);

		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		card.setSayings(sayingsCard);
		sceneList.initialize(scenes);

		initialize();

		// 使用するSEを一度メモリに乗せておく
		SetUseASyncLoadFlag(TRUE);
		seHandleKick = LoadSoundMem("sound/kick01.m4a");
		SetUseASyncLoadFlag(FALSE);
	}

	~Scenario() {
		DeleteSoundMem(seHandleKick);
		DeleteGraph(imgRoom);
		DeleteGraph(imgCard);
	}

private:
	int eqCnt = 0;
	int eqX = 0;
	int eqY = 0; // eq = earthquake
	int endingCnt = 0;
	int numContinue = 0;

	int imgRoom;
	int imgCard;
	int seHandleKick = 0;
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);
	int strColorDebug = GetColor(100, 30, 30);

	Menu menu;
	Button btnSave;
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);
	GameOver gameover;

	string music_name[2] = { "", "" };
	string saveFilePath = SAVE_FILE_SCENARIO;
	string saveGameFilePath = SAVE_FILE_SCENARIO_GAME;

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"play" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"unload" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_DESC,		"sound/bgm04.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"――世界は１つの部屋で出来ている" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"10" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_CARD,		"exibit_nowait" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	MESSAGE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"「Mr.Kが世界を滅ぼす」" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_GRAPH,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"なにっ！？" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"20" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_red" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"30" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_RED,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"貴様か！？\nMr.K！！！"},
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_RED,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_RED,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"負けたのか！？\nこの俺が！！"},
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_RED,		"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_RED,		"sound/bgm05.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"馬鹿な…\nそんなはずではなかったのに…" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"最後の最期は後悔しないと決めていたのに、こんな死に様とはな" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"だが、俺が死ねば世界の崩壊が止まるというのなら" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"俺の死にも意味を持たせられるというものじゃないか……" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_RED,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"さぁ教えてくれ" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_GREEN,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"そうか…\nそうだったのか…" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,		"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_GREEN,		"sound/bgm06.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"俺は見えたよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"残ったお前にもいつか見える時が来るだろう……" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"先に逝っているよ" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_GREEN,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"exibit_nowait" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_blue" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"50" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"貴方が邪魔なだけなのですよ" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_BLUE,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_BLUE,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"You win!!\n貴方の勝ちです！" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_BLUE,		"end" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_BLUE,		"sound/bgm07.ogg" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"馬鹿な…\nそんな…" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"世界の王に成れるチャンスを逃してしまうなんて…\n有り得ない" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"……ああ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"生きていたこと自体が大きなチャンスだったのか……" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"しまったな" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_YELLOW,		"sound/bgm08.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"Mr.Kは王になろうとしていたのか……" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"王、そうか！" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"Kとは King のことだったのか" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"Mr.K、僕も見えたよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"さぁ、後はこの世界にいるのは僕だけだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"だからこの王である僕を殺して世界を救ってくれ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"君の手で！" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,		"pop" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_YELLOW,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"ありがとう。" },
		{ SCENE_ACTION_ENDING,	MESSAGE_WHO_DESC,		"prepare" },
		{ SCENE_ACTION_ENDING,	MESSAGE_WHO_DESC,		"go" },
//		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
//		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,		"stop" },
//		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"ありがとう。" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"しかと見届けたぞ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"鹿と見届けたぞ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,		"このゲームの王が世界を滅ぼすというなら…" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,		"王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"気づいてしまったか" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"ならば鹿他無い\n唯、私が上であることを確認するだけだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"かかって来なさい" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"…………" },
		{ SCENE_ACTION_END_OF_LIST,	-1,					"" },
	};
	struct Saying sayings1[20] = {
		{ "10",		MESSAGE_WHO_BLUE,		"貴方は私にとって利用価値のある存在です" },
		{ "10",		MESSAGE_WHO_BLUE,		"これからも是非お付き合いください" },
		{ "10",		MESSAGE_WHO_YELLOW,		"……" },
		{ "20",		MESSAGE_WHO_BLUE,		"あのカード…" },
		{ "20",		MESSAGE_WHO_BLUE,		"本当か分からないが、チャンスかも知れないな" },
		{ "20",		MESSAGE_WHO_YELLOW,		"心の声、漏れているよ" },
		{ "20",		MESSAGE_WHO_BLUE,		"ホッ！" },
		{ "30",		MESSAGE_WHO_BLUE,		"ブツブツ…\n（これは使える！やりましょう！）" },
		{ "30",		MESSAGE_WHO_YELLOW,		"何言ってんだこいつ" },
		{ "40",		MESSAGE_WHO_DESC,		"机: ………" },
		{ "40",		MESSAGE_WHO_DESC,		"机: …………………" },
		{ "40",		MESSAGE_WHO_DESC,		"私は机だ！\nまだ話しかけるな！！" },
		{ "50",		MESSAGE_WHO_BLUE,		"あなた1人になるまで隠れていました" },
		{ "50",		MESSAGE_WHO_BLUE,		"他の2人を消してくれてありがとう\nこうなることはカードを見た時から分かっていたからね" },
		{ "50",		MESSAGE_WHO_BLUE,		"そして連戦の貴方を始末すれば" },
		{ "50",		MESSAGE_WHO_BLUE,		"ここは私の世界になる訳だ\nそうだろう？" },
		{ "50",		MESSAGE_WHO_BLUE,		"私は世界を滅ぼすつもりじゃないのです" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings2[20] = {
		{ "10",		MESSAGE_WHO_RED,		"俺たちはチーム！\nそして何より正義だ！" },
		{ "10",		MESSAGE_WHO_YELLOW,		"あぁ、そうだな\nこれから先もだ" },
		{ "20",		MESSAGE_WHO_RED,		"何だこのカードはぁ！\n俺を馬鹿にしているのかぁ！！" },
		{ "20",		MESSAGE_WHO_RED,		"SE[sound/kick01.m4a]" },
		{ "20",		MESSAGE_WHO_RED,		"（何かを蹴る音）" },
		{ "20",		MESSAGE_WHO_BLUE,		"！" },
		{ "20",		MESSAGE_WHO_YELLOW,		"うわぁ、落ち着けよ\n怖いなぁ" },
		{ "30",		MESSAGE_WHO_RED,		"世界が滅びるというのは本当の事のようだな"},
		{ "30",		MESSAGE_WHO_RED,		"しかし、一体我々のうちの誰が滅ぼすというのだ？"},
		{ "999",	-1,						"" },
	};
	struct Saying sayings3[20] = {
		{ "10",		MESSAGE_WHO_GREEN,	"この平和の中、何のために生きているのだろう" },
		{ "10",		MESSAGE_WHO_YELLOW,	"お前はいつもそれだな\n考えてもしょうがないのに" },
		{ "10",		MESSAGE_WHO_BLUE,	"私はこの世界を手に入れたいですねぇ" },
		{ "20",		MESSAGE_WHO_GREEN,	"何かが動き出したのですね…\n貴方はどう見ますか？" },
		{ "20",		MESSAGE_WHO_YELLOW,	"さっき突然現れた奴が何者かが気になるなぁ" },
		{ "20",		MESSAGE_WHO_GREEN,	"私も同感です\n奴の意図は読めませんね" },
		{ "30",		MESSAGE_WHO_GREEN,	"この地震と先程のカードの関係はあるのでしょうか？" },
		{ "30",		MESSAGE_WHO_YELLOW,	"でもこのままでは世界が保たないぞ" },
		{ "30",		MESSAGE_WHO_GREEN,	"そうですね\nそろそろですねぇ…" },
		{ "40",		MESSAGE_WHO_GREEN,	"貴様、Mr.Kを殺したな！" },
		{ "40",		MESSAGE_WHO_GREEN,	"何故いつも分かり合えないのだろう" },
		{ "40",		MESSAGE_WHO_GREEN,	"でも、それが同じもの同士が集まった時の性なのだろう…" },
		{ "40",		MESSAGE_WHO_GREEN,	"だからMr.Kよ、殺し合おう\nその中で生きた意味を見出そうじゃないか" },
		{ "999",	-1,					"" },
	};
	struct Saying sayingsCard[5] = {
		{ "20",		MESSAGE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "30",		MESSAGE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "40",		MESSAGE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "50",		MESSAGE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "999",	-1,					"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		gameover.initialize();
		card.hide();

		// ボタン初期化
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "中断");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "タイトル");
		menu.set(btnSave, btnReset);

		mrK[0].set(170, 30);
		mrK[0].turn(SPRITE_KEY_DOWN);
		endingCnt = 0;
		numContinue = 0;
	}

	int show() {
		SetBackgroundColor(0, 0, 0);

		if (Music::drawLoadMsg()) return MODE_SCENARIO;

		// 背景・人物の描画
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		card.draw(eqX);

		Scene scene = sceneList.get();

		bool is_reset = (!battle.isOnGame() && !hasMsg(scene) && saveOrReset());
		
		int res = ScenarioBase::show();
		switch (scene.action) {
		case SCENE_ACTION_EQ:
			performEQ(scene.how);
			break;
		case SCENE_ACTION_WAIT:
			waitClick();
			break;
		case SCENE_ACTION_ENDING:
			res = prepareEnding(scene.how);
			break;
		case SCENE_ACTION_LOSE:
			res = lostBattle(scene.who);
			break;
		default:
			break;
		}

		music_name[0] = Music::getMusicName(0);
		music_name[1] = Music::getMusicName(1);

		if (is_reset) {
			return MODE_TITLE;
		}
		else if (res != SCENE_RES_DEFAULT) {
			return res;
		}
		else {
			return MODE_SCENARIO;
		}
	}

	void route(Mode& mode, int res) {
		if (res == MODE_TITLE) {
			Music::unloadAll();
			mode.goTitle();
			initialize();
		}
		else if (res == MODE_ENDING) {
			Music::unloadAll();
			mode.goEnding();
			initialize();
		}
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 165, strColor, "trigger: %s", battle.getTrigger());
		DrawFormatString(245, 185, strColor, "eqX: %d", eqX);
		DrawFormatString(245, 205, strColor, "continue: %d", numContinue);

		ScenarioBase::debugDump();
	}

	/*===========================*/
	//    Save and Load
	/*===========================*/
	bool hasSaveFile() {
		ifstream file(saveFilePath);
		if (file) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}

	void load() {
		Encrypter encrypter(saveFilePath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "Scenario loaded: " << res.dump();
		Logger::log();

		initialize();

		loadMusic(res);
		loadScenario(res["flg"]);
		battle.load(res, saveGameFilePath);

		numContinue = res["numContinue"];
		mrK[0].trigger = res["mrk_trigger0"];
		mrK[1].trigger = res["mrk_trigger1"];
		mrK[2].trigger = res["mrk_trigger2"];
		mrK[3].trigger = res["mrk_trigger3"];

		remove(saveFilePath.c_str());
	}

private:

	void save() {
		nlohmann::json content = build_save_content();
		save(content);
	}

	void save(nlohmann::json content) {
		Encrypter encrypter(saveFilePath);
		encrypter.write(content);

		battle.save(saveGameFilePath);
	}

	nlohmann::json build_save_content() {
		nlohmann::json data = {
			{"flg", sceneList.getCurrentId()},
			{"onBattle", battle.isOnGame()},
			{"battle_trigger", battle.getTrigger()},
			{"mrk_trigger0", mrK[0].trigger},
			{"mrk_trigger1", mrK[1].trigger},
			{"mrk_trigger2", mrK[2].trigger},
			{"mrk_trigger3", mrK[3].trigger},
			{"music_name0", music_name[0]},
			{"music_name1", music_name[1]},
			{"numContinue", numContinue},
		};
		return data;
	}

	nlohmann::json build_save_content_for_lost() {
		nlohmann::json data = {
			{"flg", getContinueId()},
			{"onBattle", false},
			{"battle_trigger", battle.getTrigger()},
			{"mrk_trigger0", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger1", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger2", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger3", SPRITE_TRIGGER_FIRED},
			{"music_name0", getContinueMusic()},
			{"music_name1", music_name[0]},
			{"numContinue", numContinue + 1},
		};
		return data;
	}

	int getContinueId() {
		int currentId = sceneList.getCurrentId();
		if (currentId < SCENARIO_ID_AFTER_RED) return SCENARIO_ID_BEFORE_RED;
		if (currentId < SCENARIO_ID_AFTER_GREEN) return SCENARIO_ID_BEFORE_GREEN;
		if (currentId < SCENARIO_ID_AFTER_BLUE) return SCENARIO_ID_BEFORE_BLUE;
		return SCENARIO_ID_BEFORE_YELLOW;
	}

	string getContinueMusic() {
		int currentId = sceneList.getCurrentId();
		if (currentId < SCENARIO_ID_AFTER_BLUE) return "sound/bgm03.ogg";
		return "sound/bgm07.ogg";
	}

	void loadScenario(int flg_saved) {
		int new_flg = 0;
		int old_flg = 0;
		while (sceneList.getCurrentId() < flg_saved) {
			old_flg = sceneList.getCurrentId();
			Scene scene = sceneList.get();
			// if (scene.action != SCENE_ACTION_MUSIC) show(dummy_com);
			if (scene.action != SCENE_ACTION_MUSIC) show();
			new_flg = sceneList.getCurrentId();
			if (new_flg == old_flg) goNext();
		}
		msg.finishTalk();
		state.initialize();
	}

	void loadMusic(nlohmann::json res) {
		music_name[0] = res["music_name0"];
		music_name[1] = res["music_name1"];
		Music::unloadAll();
		Music::loadSync(music_name[0].c_str());
		Music::loadSync(music_name[1].c_str());
		Music::enableSwap();
		Music::play();
	}

	void performEQ(string how) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			waitClick();
		}
		else {
			// stopEQ
			eqX = 0;
			// 背景差し替え
			eqCnt++;
			updateImgRoom();
			goNext();
		}
	}

	void updateImgRoom() {
		if (eqCnt > SCENARIO_MAX_EQ_CNT) eqCnt = SCENARIO_MAX_EQ_CNT;

		ostringstream ss;
		ss << "./graph/room" << eqCnt << ".png";
		string filename = ss.str();

		DeleteGraph(imgRoom);
		imgRoom = LoadGraph(filename.c_str());
	}

	int prepareEnding(string how) {
		if (how == "go") {
			DrawBox(-1, -1, 641, 481, Black, TRUE);
			return MODE_ENDING;
		}

		endingCnt++;
		if (how == "prepare") {
			if (endingCnt > 160.0) {
				goNext();
			}

			// fadeout
			double fade = min(255.0, 0.01 * pow(max(0.0, endingCnt), 2.0));
			SetDrawBlendMode(DX_BLENDMODE_SUB, fade);
			DrawBox(-1, -1, 641, 481, White, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// fadeoutMusic
			Music::changeVolume(7.57 * pow(160.0 - endingCnt, 0.6));
			Music::play();
		}
		return SCENE_RES_DEFAULT;
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
			enemyCom = new Enemy();
			Scene scene = sceneList.get();
			if (scene.who == MESSAGE_WHO_RED) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			}
			else if (scene.who == MESSAGE_WHO_GREEN) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_GREEN);
			}
			else if (scene.who == MESSAGE_WHO_BLUE) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_BLUE);
			}
			else if (scene.who == MESSAGE_WHO_YELLOW) {
				battle.start(BATTLE_PLAYER_PLAYER, BATTLE_PLAYER_YELLOW);
			}
			else {
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
			}
			goNext();
			return;
		}
		return ScenarioBase::setBattle(how);
	}

	// override
	int doBattle() {
		ScenarioBase::doBattle();

		if (saveOrReset()) {
			battle.initialize();
			return MODE_TITLE;
		}

		// 対戦スキップ（一人用デバッグ）
		if (FlagStore::isDebug() && UserInput::onKeySkipDebug()) {
			goNext();
		}
			
		return MODE_SCENARIO;
	}

	// override
	bool playByCom() {
		if (FlagStore::isDebug()) return playByPlayer();

		return ScenarioBase::playByCom();
	}

	int lostBattle(int who) {
		if (battle.isLost()) {
			if (gameover.isActivated()) {
				int res = gameover.show();
				if (gameover.goingContinue()) load();
				return res;
			}

			msg.readNext(SCENARIO_MSG_LOST_BATTLE, MESSAGE_WHO_DESC);
			if (state.isOnReturnOrClicked() && msg.skip()) {
				nlohmann::json content = build_save_content_for_lost();
				save(content);

				gameover.activate(who);
			}
		}
		else {
			goNext();
		}
		return MODE_SCENARIO;
	}

	bool saveOrReset() {
		bool no_keyboard = true;
		int choice = menu.choose(strColorMenu, no_keyboard);

		// save
		if (choice == 0) save();

		//reset
		if (choice == 0 || choice == 1) {
			battle.resetGame();
			return true;
		}

		return false;
	}

	// override
	void showGraph() {
		if (state.getGraph() == "card") {
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
		}
	}

	// override
	void drawMrKs() {
		// 一旦全員描画する
		for (int i = 1; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		mrK[0].draw(eqX);

		// 0番より前にいるヤツをもう一度描画
		for (int i = 1; i < 4; ++i) {
			if (mrK[i].y > mrK[0].y) mrK[i].draw(eqX);
		}
	}

	// override
	int checkMrK() {
		int nearest = ScenarioBase::checkMrK();
		if (nearest != 0) return nearest;

		if (abs(card.y - mrK[0].y) < 45 && abs(card.x - mrK[0].x) < 45) {
			return MESSAGE_WHO_CARD;
		}
		return 0;
	}

	// override
	void startTalkMrK(int who) {
		ScenarioBase::startTalkMrK(who);

		if (who == MESSAGE_WHO_CARD) {
			msg.startTalk();
			card.talkReset();
		}
	}

	// override
	Sprite* getObject(int who) {
		if (who == MESSAGE_WHO_CARD) return &card;
		return ScenarioBase::getObject(who);
	}

	// override
	bool hasMsg(Scene scene) {
		if (ScenarioBase::hasMsg(scene)) return true;
		
		return (scene.action == SCENE_ACTION_LOSE);
	}
};
