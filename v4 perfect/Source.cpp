#include "Header.h"

void initfloors(floor floors[], int size);
void drawfloors(floor floors[], int size);
void startfloors(floor floors[], int size);
void updatefloors(floor floors[], int size);

// GLOBALS
const int width = 1024;
const int height = 768;

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

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

	ball myball;
	myball.x = width / 2;
	myball.y = height / 2;
	myball.score = 0;
	myball.speed = 1;
	myball.radius = 20;

	initfloors(floors, NUM_floor);

	srand(time(NULL));

	int FPS = 480;

	bool done = true;
	bool redraw = true;
	bool keys[2] = { false, false }; // left and right keys

	timer = al_create_timer(1.0 / FPS);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_install_keyboard();
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display)); // events for display
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	while (done) {

		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		myball.y += myball.speed;
		
		printf("%f %f\n", myball.y, myball.speed);
		if (myball.y + myball.radius > height && myball.speed > 0) { //Ball has hit the floor and is moving downward
			myball.y = height; //Set the ball to just on top of the floor
			myball.speed = -myball.speed; //Bounce
		}
		if (myball.y + myball.radius < height / 2 && myball.speed < 0) {
			myball.speed = -myball.speed;
		}
		
		

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				keys[left] = true; // tu�a bas�l� tutma i�lemi
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[right] = true;  // tu�a bas�l� tutma i�lemi
				break;
			}


		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				keys[left] = false; // tu�u b�rakma
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[right] = false;  // tu�u b�rakma
				break;

			case ALLEGRO_KEY_ESCAPE:
				done = false;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = false;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) { // For smooth movement
			redraw = true;
			myball.x -= keys[left] * 1;
			myball.x += keys[right] * 1;

			startfloors(floors, NUM_floor);

			updatefloors(floors, NUM_floor);

		}


		while (myball.x >= 1004) {  // Sa� taraf�n s�n�r� i�in
			myball.x -= 1;
		}
		while (myball.x <= 20) {  // Sol taraf�n s�n�r� i�in
			myball.x += 1;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_draw_textf(font24, al_map_rgb(0, 255, 0), 50, 10, ALLEGRO_ALIGN_CENTER, "Score : %i", myball.score);
			al_draw_circle(myball.x, myball.y, myball.radius, al_map_rgb(0, 191, 255), 1);
			drawfloors(floors, NUM_floor);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0)); // buffer� temizleyip arkadaki renkleri s�f�rl�yor �nceki hareketi ekranda kalm�yor.
		}

	}


	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font24);
	return 0;
}



void initfloors(floor floors[], int size) {
	for (int i = 0; i < NUM_floor; i++) {    // initializing floors
		floors[i].id = ENEMY;
		floors[i].live = false;
		floors[i].speed = 0.1;

		printf("%d", floors[i].x);
	}
}
void drawfloors(floor floors[], int size) {
	for (int i = 0; i < NUM_floor; i++) {
		if (floors[i].live) {
			al_draw_rectangle(floors[i].x, floors[i].y, floors[i].x + 100, floors[i].y, al_map_rgb(255, 255, 255), 50);
		}

	}
}
void startfloors(floor floors[], int size) {
	for (int i = 0; i < NUM_floor; i++) {
		if (!floors[i].live) {
			if (rand() % 500 == 0) {
				floors[i].live = true;
				floors[i].x = 100 + rand() % (height - 30);
				floors[i].y = 0;

				break;
			}
		}
	}
}
void updatefloors(floor floors[], int size) {
	for (int i = 0; i < NUM_floor; i++) {
		if (floors[i].live) {

			floors[i].y += floors[i].speed;
			if (floors[i].y > height) {
				floors[i].live = false;
			}
		}

	}
}