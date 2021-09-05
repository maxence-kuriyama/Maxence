
class fireflower {
private:
	int cnt;			// 花火の寿命カウンタ
	double particle[12][2];
	double velocity[12][2];
	double diffuse;

public:
	int sound = 0;		// SEを出すか否か

	fireflower() {
		initialize();
	}

	void initialize() {
		cnt = rand() % 40;
		diffuse = (2.5 * rand()) / RAND_MAX + 0.5;
		double inix = (440.0 * rand()) / RAND_MAX + 100.0;
		double iniy = (200.0 * rand()) / RAND_MAX + 280.0;
		for (int i = 0; i < 12; ++i) {
			particle[i][0] = inix;
			particle[i][1] = iniy;
			velocity[i][0] = diffuse * cos(2 * M_PI * i / 12);
			velocity[i][1] = diffuse * sin(2 * M_PI * i / 12);
		}
	}

	void tick() {
		cnt++;
		if (cnt <= 80) {
			for (int i = 0; i < 12; ++i) {
				particle[i][0] += velocity[i][0] * 0.1 * ((double)rand() / RAND_MAX - 0.5);
				particle[i][1] += -0.015 * particle[i][1] * (1.0 + 0.2 * ((double)rand() / RAND_MAX - 0.5));
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
