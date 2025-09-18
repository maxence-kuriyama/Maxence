#pragma once

#define VERSION		"Maxence 0.7.3"
#define FPS			60
#define DEBUG		1
#define FIRE_FLOWER_NUM		3

#define TEXT1_X		220
#define TEXT1_Y		310
#define TEXT2_X		270
#define TEXT2_Y		340
#define TEXT3_X		320
#define TEXT3_Y		370
#define TEXT4_X		370
#define TEXT4_Y		400
#define TEXT_SAVE_X			220
#define TEXT_SAVE_Y			440
#define TEXT_RESET_X		340
#define TEXT_RESET_Y		440

#define RWD_VICTORY			1.0
#define RWD_DOMINANT		1.0
#define RWD_PUT				0.0
#define RWD_CAN_PUT			0.0
#define RWD_FAULT			-0.5
#define RWD_NOT_UPDATED		-100.0

#define BATTLE_PLAYER_NONE		-1
#define BATTLE_PLAYER_YELLOW	0
#define BATTLE_PLAYER_BLUE		1
#define BATTLE_PLAYER_RED		2
#define BATTLE_PLAYER_GREEN		3
#define BATTLE_PLAYER_DEER		4
#define BATTLE_PLAYER_PLAYER	5
#define BATTLE_PLAYER_TUTO_RED	101

#define COMMENT_CHANGE_TYPE_SEQUENTIAL		"seq"
#define COMMENT_CHANGE_TYPE_PLAY_TURN		"play"
#define COMMENT_CHANGE_TYPE_LONG_THINKING	"long"
#define COMMENT_CHANGE_TYPE_LONG_THINKING_TIME	10 * FPS

#define VICTORY_NONE		0
#define VICTORY_BLACK		1
#define VICTORY_WHITE		-1
#define VICTORY_DRAW		10

#define TEBAN_SENKO			0
#define TEBAN_KOKO			1

#define MACHINE_INPUT_SIZE		180
#define MACHINE_OUTPUT_SIZE		81

#define SAVE_FILE_MUSIC_ROOM	"./data/savemr.dat"
#define SAVE_FILE_BATTLE_GAME	"./data/savegb.dat"
#define SAVE_FILE_SCENARIO		"./data/savesc.dat"
#define SAVE_FILE_SCENARIO_GAME	"./data/savegs.dat"
