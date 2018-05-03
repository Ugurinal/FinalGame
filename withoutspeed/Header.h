#define _CRT_SECURE_NO_WARNINGS
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5\allegro_windows.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>

enum keys { left, right };
enum IDS { ENEMY, PLAYER };

const int NUM_floor = 5;

struct floor {
	int id;
	float x;
	float y;
	int w;
	int h;
	bool live;
	float speed;

};

struct ball {
	int score;
	float x;
	float y;
	int w;
	int h;
	float speed;
	int radius;
};

floor floors[NUM_floor];