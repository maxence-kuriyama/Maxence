#pragma once

// �������A�j���[�V�����̃x�[�X�N���X
class AnimeBase {
protected:
	int cnt = 0;
	bool active = false;

public:
	
	AnimeBase() {
		initialize();
	}

	void start() {
		active = true;
	}

protected:

	void initialize() {
		cnt = 0;
		active = false;
	}
};
