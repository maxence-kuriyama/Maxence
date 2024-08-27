#pragma once


// 花火オブジェクト
class fireflower {
private:
	int cnt;			// 花火の寿命カウンタ
	double particle[12][2];
	double velocity[12][2];
	double diffuse;
	double minX = 100.0;
	double maxX = 540.0;
	double minY = 280.0;
	double maxY = 480.0;

public:
	int sound = 0;		// SEを出すか否か

	fireflower() {
		initialize();
	}

	void initialize() {
		cnt = rand() % 40;
		diffuse = 2.5 * rand() / RAND_MAX + 0.5;
		double inix = (maxX - minX) * rand() / RAND_MAX + minX;
		double iniy = (maxY - minY) * rand() / RAND_MAX + minY;
		for (int i = 0; i < 12; ++i) {
			particle[i][0] = inix;
			particle[i][1] = iniy;
			velocity[i][0] = diffuse * cos(2 * M_PI * i / 12);
			velocity[i][1] = diffuse * sin(2 * M_PI * i / 12);
		}
	}

	void initialize(double srcMinX, double srcMaxX, double srcMinY, double srcMaxY) {
		setRange(srcMinX, srcMaxX, srcMinY, srcMaxY);
		initialize();
	}

	void setRange(double srcMinX, double srcMaxX, double srcMinY, double srcMaxY) {
		minX = srcMinX;
		maxX = srcMaxX;
		minY = srcMinY;
		maxY = srcMaxY;
	}

	void tick() {
		cnt++;
		if (cnt <= 80) {
			for (int i = 0; i < 12; ++i) {
				particle[i][0] += velocity[i][0] * 0.1 * ((double)rand() / RAND_MAX - 0.5);
				particle[i][1] += -0.015 * min(480.0, particle[i][1]) * (1.0 + 0.2 * ((double)rand() / RAND_MAX - 0.5));
			}
		}
		else if (cnt <= 200) {
			for (int i = 0; i < 12; ++i) {
				particle[i][0] += velocity[i][0] + 0.5 * ((double)rand() / RAND_MAX - 0.5);
				particle[i][1] += velocity[i][1] + 0.5 * ((double)rand() / RAND_MAX - 0.1);
				velocity[i][0] = velocity[i][0] * 0.99;
				velocity[i][1] = velocity[i][1] * 0.99;
			}
		}
		else {
			initialize();
		}
		if (sound == 1 && cnt == 80) PlaySound("sound/owin31.wav", NULL, SND_ASYNC);
	}

	void draw() {
		double norm;
		for (int i = 0; i < 12; ++i) {
			norm = sqrt(pow(velocity[i][0], 2.0) + pow(velocity[i][1], 2.0));
			DrawCircle(particle[i][0], particle[i][1], 10 * sqrt(norm), GetColor((int)255 * cos(norm), (int)255 * sin(norm), 255 * rand() / RAND_MAX), TRUE);
		}
	}
};
