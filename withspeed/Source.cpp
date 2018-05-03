#include "Header.h"

void initfloors(floor floors[], int size);
void drawfloors(floor floors[], int size);
void startfloors(floor floors[], int size);
void updatefloors(floor floors[], int size);

const int width = 1024;
const int height = 768;

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *ballsound = NULL;
	ALLEGRO_SAMPLE *timesound = NULL;

	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	display = al_create_display(width, height);
	if (!display) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	if (!al_init_font_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_font_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	if (!al_init_ttf_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_ttf_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	if (!al_init_primitives_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_primitives_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	ALLEGRO_FONT *font24 = al_load_font("arial.ttf", 24, 0);
	ALLEGRO_FONT *font72 = al_load_font("arial.ttf", 72, 0);

	time_t start, end;
	time(&start);

	ball myball;
	myball.x = width / 2;
	myball.y = height - 250;
	myball.score = 0;
	myball.speed = 0;
	myball.radius = 20;

	initfloors(floors, NUM_floor);

	srand(time(NULL));

	int FPS = 80;

	bool done = true;
	bool redraw = true;
	bool keys[2] = { false, false };
	bool Collision = false;

	timer = al_create_timer(1.0 / FPS);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10);
	ballsound = al_load_sample("ballsound.wav");
	timesound = al_load_sample("timesound.wav");

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);


	while (done) {

		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		myball.y += myball.speed;
		myball.speed += 0.5;
		if (myball.y + myball.radius > height && myball.speed > 0) {
			myball.y = height;
			myball.speed = -myball.speed;

		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				keys[left] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[right] = true;
				break;
			}


		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				keys[left] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[right] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				al_draw_textf(font72, al_map_rgb(0, 255, 0), 500, 250, ALLEGRO_ALIGN_CENTER, "Score : %d", myball.score);
				al_draw_textf(font72, al_map_rgb(255, 0, 0), 500, 350, ALLEGRO_ALIGN_CENTER, "GAME OVER !");
				al_draw_textf(font24, al_map_rgb(0, 255, 0), 500, 650, ALLEGRO_ALIGN_CENTER, "This display will close in 3 seconds...");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				Sleep(3000);
				done = false;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			al_draw_textf(font72, al_map_rgb(0, 255, 0), 500, 250, ALLEGRO_ALIGN_CENTER, "Score : %d", myball.score);
			al_draw_textf(font72, al_map_rgb(255, 0, 0), 500, 350, ALLEGRO_ALIGN_CENTER, "GAME OVER !");
			al_draw_textf(font24, al_map_rgb(0, 255, 0), 500, 650, ALLEGRO_ALIGN_CENTER, "This display will close in 3 seconds...");
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			Sleep(3000);
			done = false;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			myball.x -= keys[left] * 20;
			myball.x += keys[right] * 20;

			startfloors(floors, NUM_floor);

			updatefloors(floors, NUM_floor);

			int i;
			for (i = 0; i <= NUM_floor; i++) {
				if (myball.x < floors[i].x + floors[i].x + 150 &&
					myball.x + 2 * myball.radius > floors[i].x &&
					myball.y < floors[i].y + 50 &&
					2 * myball.radius + myball.y > floors[i].y) {
					Collision = true;// collision detected!
					floors[i].x = 1500;
					floors[i].y = 1500;
				}
				else {
					Collision = false;
				}

				if (Collision) {
					floors[i].live = true;
					myball.score += 1;
					myball.speed *= -1;
					al_play_sample(ballsound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				}
			}

		}


		while (myball.x >= 1004) {
			myball.x -= 10;
		}
		while (myball.x <= 20) {
			myball.x += 10;
		}
		time(&end);


		if (difftime(end, start) == 50) {
			al_play_sample(timesound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

		}

		if (difftime(end, start) == 10) {
			for (int i = 0; i <= NUM_floor; i++) {
				if (floors[i].live) {

					floors[i].speed += 0.01;

				}
			}
		}
		if (difftime(end, start) == 20) {
			for (int i = 0; i <= NUM_floor; i++) {
				if (floors[i].live) {

					floors[i].speed += 0.01;
				}
			}
		}
		if (difftime(end, start) == 30) {
			for (int i = 0; i <= NUM_floor; i++) {
				if (floors[i].live) {

					floors[i].speed += 0.01;
				}
			}
		}
		if (difftime(end, start) == 40) {
			for (int i = 0; i <= NUM_floor; i++) {
				if (floors[i].live) {

					floors[i].speed += 0.01;
				}
			}
		}
		if (difftime(end, start) == 50) {
			for (int i = 0; i <= NUM_floor; i++) {
				if (floors[i].live) {

					floors[i].speed += 0.01;
				}
			}
		}


		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_draw_textf(font24, al_map_rgb(0, 255, 0), 60, 10, ALLEGRO_ALIGN_CENTER, "Score : %d", myball.score);
			al_draw_textf(font24, al_map_rgb(0, 255, 0), 60, 40, ALLEGRO_ALIGN_CENTER, "Time : %.f", difftime(start, end) + 60);
			al_draw_circle(myball.x, myball.y, myball.radius, al_map_rgb(0, 191, 255), 1);
			drawfloors(floors, NUM_floor);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (difftime(end, start) == 60) {

				al_draw_textf(font72, al_map_rgb(0, 255, 0), 500, 250, ALLEGRO_ALIGN_CENTER, "Score : %d", myball.score);
				al_draw_textf(font72, al_map_rgb(255, 0, 0), 500, 350, ALLEGRO_ALIGN_CENTER, "GAME OVER !");
				al_draw_textf(font24, al_map_rgb(0, 255, 0), 500, 650, ALLEGRO_ALIGN_CENTER, "This display will close in 3 seconds...");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				Sleep(3000);
				done = false;
			}
		}

	}


	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_sample(ballsound);
	al_destroy_sample(timesound);
	al_destroy_font(font24);
	al_destroy_font(font72);


	return 0;
}



void initfloors(floor floors[], int size) {
	for (int i = 0; i <= NUM_floor; i++) {
		floors[i].id = ENEMY;
		floors[i].live = false;
		floors[i].speed = 3;

	}
}
void drawfloors(floor floors[], int size) {
	for (int i = 0; i <= NUM_floor; i++) {
		if (floors[i].live) {
			al_draw_rectangle(floors[i].x, floors[i].y, floors[i].x + 150, floors[i].y, al_map_rgb(255, 255, 255), 50);
		}

	}
}
void startfloors(floor floors[], int size) {
	static int first = 0;


	if (!first) {
		for (int i = 0; i <1; i++) {
			floors[i].live = true;
		}

		floors[0].x = 100;
		floors[0].y = 400;
		first++;
	}
	else {

		for (int i = 0; i <= NUM_floor; i++) {
			if (!floors[i].live) {
				if (rand() % 150 == 0) {
					floors[i].live = true;
					floors[i].x = rand() % (width - 180);
					floors[i].y = 0;


					break;
				}
			}
		}
	}

}
void updatefloors(floor floors[], int size) {
	for (int i = 0; i <= NUM_floor; i++) {
		if (floors[i].live) {

			floors[i].y += floors[i].speed;

			if (floors[i].y > height + 150) {

				floors[i].live = false;

			}
		}

	}
}