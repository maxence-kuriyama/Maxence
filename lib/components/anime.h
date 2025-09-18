#pragma once

// �������A�j���[�V�����̃x�[�X�N���X
class AnimeBase {
protected:
	int cnt = 0;
	int seHandle = 0;

public:
	bool active = false;
	
	AnimeBase() {
		initialize();

		SetUseASyncLoadFlag(TRUE);
		seHandle = LoadSoundMem("sound/kick02.m4a");
		SetUseASyncLoadFlag(FALSE);
	}

	~AnimeBase() {
		DeleteSoundMem(seHandle);
	}

	void start() {
		active = true;
		playSE();
	}

	void update() {
		if (!active) return;

		_update();
		cnt++;

		if (cnt > 120) initialize();
	}

protected:

	virtual void initialize() {
		cnt = 0;
		active = false;
	}

	virtual void _update() {}

	void playSE() {
		PlaySoundMem(seHandle, DX_PLAYTYPE_BACK);
	}
};
