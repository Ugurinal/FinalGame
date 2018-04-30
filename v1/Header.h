#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

enum keys { left, right };
enum IDS { ENEMY, PLAYER };

const int NUM_floor = 5;

struct floor {
	int id;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
};

struct ball {
	int score;
	float x;
	float y;
	float vely;
	int radius;
};

floor floors[NUM_floor];