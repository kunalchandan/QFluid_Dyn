#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <math.h>
#include "FluidCube.cpp"

#define IX(x, y, z, N) ((x) + (y) * N + (z) * N * N)

float map(float t_min, float t_max, float i_min, float i_max, float num) {
	return (((num - i_min) * (t_max - t_min) / (i_max - i_min))) + t_min;
}

int main()
{
	int len = 64;
	al_init();
	al_init_font_addon();
	ALLEGRO_DISPLAY* display = al_create_display(len, len);
	ALLEGRO_FONT* font = al_create_builtin_font();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Fluid Physics");
	al_flip_display();
	
	FluidCube *cube = new FluidCube(len, 2, 2, 0.001);
	for (int x = 0; x < pow(len, 3); x+=10) {
		cube->Vx[x] = 9;
		std::cout << x << std::endl;
	}
	for (int t = 0; t < 100000; t++) {
		al_flip_display();
		cube->FluidCube_Step();
		for (int x = 0; x < len; x++) {
			for (int y = 0; y < len; y++) {
				al_rest(.01);
				cube->AddDensity(len/2, len/2, len/2, 1);
				cube->AddVelocity(len/2, len/2, len/2, 2, 2, 2);

				float max = * std::max_element(cube->density, cube->density + int (std::powf(cube->size, 3)));
				float min = 0;

				float value = map(min, max, 0, 255, cube->density[IX(x, y, 0, cube->size)]);

				ALLEGRO_COLOR colour = {value, value, value, 1};
				al_draw_pixel(x, y, colour);
			}
		}
	}

	char a; std::cin >> a;
	return 0;
}