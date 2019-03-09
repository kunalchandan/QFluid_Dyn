#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <math.h>
#include "FluidCube.cpp"

float map(float t_min, float t_max, float i_min, float i_max, float num) {
	return (((num - i_min) * (t_max - t_min) / (i_max - i_min))) + t_min;
}

int IX(int x, int y, int z, int N) {
	return ((x)+(y * N) + (z * N * N));
}

int main()
{
	int len = 32;
	int sc = 8;
	al_init();
	al_init_font_addon();
	ALLEGRO_DISPLAY* display = al_create_display(len*sc, len*sc);
	ALLEGRO_FONT* font = al_create_builtin_font();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Fluid Physics");
	al_flip_display();
	
	FluidCube *cube = new FluidCube(len, 0.2, 0, 0.0000001);
	for (int t = 0; t < 100000; t++) {
		cube->FluidCube_Step();
		float max = * std::max_element(cube->Vx, cube->Vx + int (std::powf(len, 3)));
		float min = 0;
		ALLEGRO_COLOR colour = {0, 0, 0, 1};
		for (int x = 0; x < len; x++) {
			for (int y = 0; y < len; y++) {
				cube->AddDensity(x, len/2, 0, 0.04);
				cube->AddVelocity(x, len / 2, 0, 0.04, 0.04, 0);
				float value = map(min, max, 0, 255, cube->Vx[IX(x, y, 0, len)]);

				colour = {value, value, value, 1};
				for (int i = 0; i < sc; i++) {
					for (int j = 0; j < sc; j++) {
						al_draw_pixel(x * sc + i, y * sc + j, colour);
					}
				}
			}
		}
		std::cout << "step: " << t << std::endl;
		al_flip_display();
	}

	char a; std::cin >> a;
	return 0;
}