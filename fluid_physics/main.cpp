#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

int main()
{
	al_init();
	al_init_font_addon();
	ALLEGRO_DISPLAY* display = al_create_display(800, 600);
	ALLEGRO_FONT* font = al_create_builtin_font();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Welcome to Fluid Physics");
	al_flip_display();

	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			al_draw_pixel(x, y, al_map_rgb(x / 10, y / 10, (x + y) / 10));
			al_flip_display();
		}
	}
	al_rest(5.0);

	return 0;
}