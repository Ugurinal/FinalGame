#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum keys { left, right };
enum IDS { ENEMY, PLAYER };

const int NUM_floor = 4;

struct floor {
	int id;
	float x;
	float y;
	bool live;
	float speed;
};

struct ball {
	int score;
	float x;
	float y;
	float speed;
	int radius;
};

floor floors[NUM_floor];