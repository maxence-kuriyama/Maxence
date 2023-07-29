#pragma once

#include "lib/modes/common/scenario_base.h"
#include "lib/encrypter.h"


const int SCENE_ACTION_EQ(11);
const int SCENE_ACTION_WAIT(12);

const int SCENE_WHO_CARD(11);

// シナリオ管理用クラス
class Scenario : public ScenarioBase {

public:
	MrK card;

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");

		mrK[0].set(170, 30);
		mrK[0].setExpand(SPRITE_EXPAND_RATE, 72, 85);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
		mrK[1].set(320, 80, "graph/sprite12.png", 1);
		mrK[2].set(200, 230, "graph/sprite13.png", 1);
		mrK[3].set(500, 260, "graph/sprite14.png", 1);
		deer.set(300, 160, "graph/sprite15.png", 0);
		card.set(230, 160, "graph/sprite16.png", 0);

		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		card.setSayings(sayingsCard);

		for (int i = 0; i < MAX_SCENE_NUM; i++) {
			sceneList[i] = scenes[i];
		}
		initialize();
		msg.initialize();
	}

private:
	int eqX = 0;
	int eqY = 0; // eq = earthquake
	int imgRoom;
	int imgCard;

	Menu menu;
	Button btnSave;
	Button btnReset;

	int strColorMenu = GetColor(255, 255, 255);
	string music_name[2] = { "", "" };
	string save_filepath = "./data/savesc.dat";
	string save_game_filepath = "./data/savegs.dat";

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"play" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"unload" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_DESC,		"sound/bgm04.ogg" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"――世界は１つの部屋で出来ている" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"10" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_CARD,		"exibit_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"「Mr.Kが世界を滅ぼす」" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"なにっ！？" },
//		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: あれ、Mr.Kが居ないぞ、何なんだ！" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"20" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_red" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"30" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"swap" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"貴様か！？\nMr.K！！！"},
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"start" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"負けたのか！？\nこの俺が！！"},
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"end" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_RED,		"sound/bgm05.ogg" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"馬鹿な…\nそんなはずではなかったのに…" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"最後の最期は後悔しないと決めていたのに、こんな死に様とはな" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"だが、俺が死ねば世界の崩壊が止まるというのなら" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"俺の死にも意味を持たせられるというものじゃないか……" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide" },
		{ SCENE_ACTION_WAIT,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"さぁ教えてくれ" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"start" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"ぐあー！" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"end" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_GREEN,	"sound/bgm06.ogg" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"俺は見えたよ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"残ったお前にもいつか見える時が来るだろう……" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"先に逝っているよ" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_WAIT,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"exibit_nowait" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_blue" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"50" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"貴方が邪魔なだけなのですよ" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"start" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"You win!" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"end" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_BLUE,		"sound/bgm07.ogg" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"馬鹿な…\nそんな…" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"世界の王に成れるチャンスを逃してしまうなんて…\n有り得ない" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"……ああ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"生きていたこと自体が大きなチャンスだったのか……" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"しまったな" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_WAIT,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_YELLOW,	"sound/bgm08.ogg" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.Kは王になろうとしていたのか……" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"王、そうか！" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Kとは King のことだったのか" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K、僕も見えたよ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"さぁ、後はこの世界にいるのは僕だけだ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"だからこの王である僕を殺して世界を救ってくれ" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"start" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"…" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"end" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"ありがとう。" },
		{ SCENE_ACTION_STOP,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"しかと見届けたぞ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿と見届けたぞ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"このゲームの王が世界を滅ぼすというなら…" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"気づいてしまったか" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"ならば鹿他無い\n唯、私が上であることを確認するだけだ" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"かかって来なさい" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"…………" },
		{ -1,					-1,					"" },
	};
	struct Saying sayings1[20] = {
		{ "10",		SCENE_WHO_BLUE,		"貴方は私にとって利用価値のある存在です" },
		{ "10",		SCENE_WHO_BLUE,		"これからも是非お付き合いください" },
		{ "10",		SCENE_WHO_YELLOW,	"……" },
		{ "20",		SCENE_WHO_BLUE,		"あのカード…" },
		{ "20",		SCENE_WHO_BLUE,		"本当か分からないが、チャンスかも知れないな" },
		{ "20",		SCENE_WHO_YELLOW,	"心の声、漏れているよ" },
		{ "20",		SCENE_WHO_BLUE,		"ホッ！" },
		{ "30",		SCENE_WHO_BLUE,		"ブツブツ…\n（これは使える！やりましょう！）" },
		{ "30",		SCENE_WHO_YELLOW,	"何言ってんだこいつ" },
		{ "40",		SCENE_WHO_DESC,		"机: ………" },
		{ "40",		SCENE_WHO_DESC,		"机: …………………" },
		{ "40",		SCENE_WHO_DESC,		"私は机だ！\nまだ話しかけるな！！" },
		{ "50",		SCENE_WHO_BLUE,		"あなた1人になるまで隠れていました" },
		{ "50",		SCENE_WHO_BLUE,		"他の2人を消してくれてありがとう\nこうなることはカードを見た時から分かっていたからね" },
		{ "50",		SCENE_WHO_BLUE,		"そして連戦の貴方を始末すれば" },
		{ "50",		SCENE_WHO_BLUE,		"ここは私の世界になる訳だ\nそうだろう？" },
		{ "50",		SCENE_WHO_BLUE,		"私は世界を滅ぼすつもりじゃないのです" },
		{ "999",	-1,					"" },
	};
	struct Saying sayings2[20] = {
		{ "10",		SCENE_WHO_RED,		"俺たちはチーム！\nそして何より正義だ！" },
		{ "10",		SCENE_WHO_YELLOW,	"あぁ、そうだな\nこれから先もだ" },
		{ "20",		SCENE_WHO_RED,		"何だこのカードはぁ！\n俺を馬鹿にしているのかぁ！！" },
		{ "20",		SCENE_WHO_RED,		"SE[sound/kick01.m4a]" },
		{ "20",		SCENE_WHO_RED,		"（何かを蹴る音）" },
		{ "20",		SCENE_WHO_BLUE,		"！" },
		{ "20",		SCENE_WHO_YELLOW,	"うわぁ、落ち着けよ\n怖いなぁ" },
		{ "30",		SCENE_WHO_RED,		"世界が滅びるというのは本当の事のようだな"},
		{ "30",		SCENE_WHO_RED,		"しかし、一体我々のうちの誰が滅ぼすというのだ？"},
		{ "999",	-1,					"" },
	};
	struct Saying sayings3[20] = {
		{ "10",		SCENE_WHO_GREEN,	"この平和の中、何のために生きているのだろう" },
		{ "10",		SCENE_WHO_YELLOW,	"お前はいつもそれだな\n考えてもしょうがないのに" },
		{ "10",		SCENE_WHO_BLUE,		"私はこの世界を手に入れたいですねぇ" },
		{ "20",		SCENE_WHO_GREEN,	"何かが動き出したのですね…\n貴方はどう見ますか？" },
		{ "20",		SCENE_WHO_YELLOW,	"さっき突然現れた奴が何者かが気になるなぁ" },
		{ "20",		SCENE_WHO_GREEN,	"私も同感です\n奴の意図は読めませんね" },
		{ "30",		SCENE_WHO_GREEN,	"この地震と先程のカードの関係はあるのでしょうか？" },
		{ "30",		SCENE_WHO_YELLOW,	"でもこのままでは世界が保たないぞ" },
		{ "30",		SCENE_WHO_GREEN,	"そうですね\nそろそろですねぇ…" },
		{ "40",		SCENE_WHO_GREEN,	"貴様、Mr.Kを殺したな！" },
		{ "40",		SCENE_WHO_GREEN,	"何故いつも分かり合えないのだろう" },
		{ "40",		SCENE_WHO_GREEN,	"でも、それが同じもの同士が集まった時の性なのだろう…" },
		{ "40",		SCENE_WHO_GREEN,	"だからMr.Kよ、殺し合おう\nその中で生きた意味を見出そうじゃないか" },
		{ "999",	-1,					"" },
	};
	struct Saying sayingsCard[5] = {
		{ "20",		SCENE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "30",		SCENE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "40",		SCENE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "50",		SCENE_WHO_DESC,	"「Mr.Kが世界を滅ぼす」" },
		{ "999",	-1,					"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		card.hide();
		// ボタン初期化
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "中断");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "タイトル");
		menu.set(btnSave, btnReset);
		mrK[0].set(170, 30);
		mrK[0].turn(MRK_KEY_DOWN);
	}

	int show(UserInput& ui, COM& com, Music& music, bool debug = false) {
		// 背景・人物の描画
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		card.draw(eqX);

		bool is_reset = (!onBattle && !isTalking && saveOrReset(ui));
		
		int res = ScenarioBase::show(ui, com, music, debug);
		showAdditionalAction(ui);
		music_name[0] = music.musicName[0];
		music_name[1] = music.musicName[1];

		if (is_reset) {
			return FLAG_TITLE;
		}
		else if (res != SCENE_RES_DEFAULT) {
			return res;
		}
		else {
			return FLAG_SCENARIO;
		}
	}

	void setSenko() {
		game.setSenko();
	}

	void setKoko() {
		game.setKoko();
	}


	/*===========================*/
	//    Save and Load
	/*===========================*/
	bool hasSaveFile() {
		ifstream file(save_filepath);
		if (file) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}

	void save() {
		Encrypter encrypter(save_filepath);
		nlohmann::json data = {
			{"flg", flg},
			{"onBattle", onBattle},
			{"battle_trigger", battle_trigger},
			{"mrk_trigger0", mrK[0].trigger},
			{"mrk_trigger1", mrK[1].trigger},
			{"mrk_trigger2", mrK[2].trigger},
			{"mrk_trigger3", mrK[3].trigger},
			{"music_name0", music_name[0]},
			{"music_name1", music_name[1]},
		};
		encrypter.write(data);

		if (onBattle) {
			game.save(save_game_filepath);
		}
	}

	void load(Music& music) {
		Encrypter encrypter(save_filepath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "Scenario loaded: " << res.dump();
		Logger::log();

		initialize();

		loadMusic(music, res);
		loadScenario(res["flg"], music);

		onBattle = res["onBattle"];
		if (onBattle) {
			game.load(save_game_filepath);
		}

		battle_trigger = res["battle_trigger"];
		mrK[0].trigger = res["mrk_trigger0"];
		mrK[1].trigger = res["mrk_trigger1"];
		mrK[2].trigger = res["mrk_trigger2"];
		mrK[3].trigger = res["mrk_trigger3"];

		remove(save_filepath.c_str());
	}

	void loadScenario(int flg_saved, Music& music) {
		Mouse mouse;
		Key key;
		UserInput dummy_ui = { &key, &mouse };
		COM dummy_com(false);
		int new_flg = 0;
		int old_flg = 0;
		while (flg < flg_saved) {
			old_flg = flg;
			Scene scene = getCurrentScene();
			if (scene.action != SCENE_ACTION_MUSIC) show(dummy_ui, dummy_com, music);
			new_flg = flg;
			if (new_flg == old_flg) goNext();
		}
		hasMsg = false;
		isTalking = false;
	}

	void loadMusic(Music& music, nlohmann::json res) {
		music_name[0] = res["music_name0"];
		music_name[1] = res["music_name1"];
		music.unloadAll();
		music.load(music_name[0].c_str(), 0);
		music.load(music_name[1].c_str(), 0);
		music.enableSwap();
		music.play();
	}


	/*===========================*/
	//    Battle Mode
	/*===========================*/
	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 165, strColor, "trigger: %s", battle_trigger);
		DrawFormatString(245, 245, strColor, "eqX: %d", eqX);

		ScenarioBase::debugDump();
	}


private:

	int showAdditionalAction(UserInput& ui) {
		Scene scene = getCurrentScene();

		switch (scene.action) {
		case SCENE_ACTION_EQ:
			performEQ(scene.how, *ui.mouse);
			break;
		case SCENE_ACTION_WAIT:
			waitClick(*ui.mouse);
		default:
			break;
		}

		return SCENE_RES_DEFAULT;
	}

	void performEQ(string how, Mouse &mouse) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			waitClick(mouse);
		}
		else {
			// stopEQ
			eqX = 0;
			goNext();
		}
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
			Scene scene = getCurrentScene();
			if (scene.who == SCENE_WHO_RED) {
				game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			}
			else if (scene.who == SCENE_WHO_GREEN) {
				game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_GREEN);
			}
			else if (scene.who == SCENE_WHO_BLUE) {
				game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_BLUE);
			}
			else if (scene.who == SCENE_WHO_YELLOW) {
				game.prepare(BATTLE_PLAYER_PLAYER, BATTLE_PLAYER_YELLOW);
			}
			game.setVsCOM();
			onBattle = true;
		}
		else if (how == "end") {
			initializeBattle();
		}
		goNext();
	}

	// override
	int doBattle(UserInput& ui, COM& com, bool debug) {
		ScenarioBase::doBattle(ui, com, debug);

		if (saveOrReset(ui)) {
			initializeBattle();
			return FLAG_TITLE;
		}

		// 対戦スキップ（一人用デバッグ）
		if (debug && skipBattle(ui)) goNext();
			
		return FLAG_SCENARIO;
	}

	// TODO: getCurrentScene()に応じて使うCOMを変えること
	// com.playの中で変えるべきか？
	bool playByCom(COM& com) {
		// MinMaxNode node(game.board, game.currentSide());
		// int depth = 2;
		// int index = node.search(depth);
		// Coordinate choice = Board::coordinates(index);

		VectorXd input = game.stateToInput();
		com.play(input, game.board, game.currentSide());
		Coordinate choice = com.choice;

		double res = game.update(choice);
		return game.isUpdated(res);
	}

	// デバッグ用
	bool skipBattle(UserInput& ui) {
		if (ui.onKeySkipDebug()) {
			initializeBattle();
			return true;
		}
		return false;
	}

	bool saveOrReset(UserInput& ui) {
		bool no_keyboard = true;
		int choice = menu.choose(ui, strColorMenu, no_keyboard);

		// save
		if (choice == 0) save();

		//reset
		if (choice == 0 || choice == 1) {
			ui.reset();
			game.reset();
			return true;
		}

		return false;
	}

	// override
	void showGraph() {
		if (imgFront == "card") {
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
			return SCENE_WHO_CARD;
		}
		return 0;
	}

	// override
	void talkResetMrK(int who) {
		ScenarioBase::talkResetMrK(who);

		if (who == SCENE_WHO_CARD) {
			isTalking = true;
			card.talkReset();
		}
	}

	// override
	MrK* getObject(int who) {
		if (who == SCENE_WHO_CARD) return &card;
		return ScenarioBase::getObject(who);
	}
};
