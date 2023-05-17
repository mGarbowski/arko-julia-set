#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

int WIDTH = 800;
int HEIGHT = 640;

void generateJuliaSet(uint8_t *pixels, int width, int height, double escapeRadius, double cReal, double cImag) {
    int centerReal = width / 2;
    int centerImag = height / 2;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            double zReal = (col - centerReal) * escapeRadius / width;
            double zImag = (row - centerImag) * escapeRadius / height;

            int iteration = 0;
            int maxIteration = 1000;

            while (zReal * zReal + zImag * zImag < escapeRadius * escapeRadius && iteration < maxIteration) {
                double tmpReal = zReal * zReal - zImag * zImag;
                zImag = 2 * zReal * zImag + cImag;
                zReal = tmpReal + cReal;

                iteration++;
            }

            uint8_t r, g, b;
            if (iteration == maxIteration) {
                r = 0;
                g = 0;
                b = 0;
            } else {
                r = 255;
                g = 0;
                b = 255;
            }

            int pixelIdx = 3 * (row * height + col);
            *(pixels + pixelIdx) = r;
            *(pixels + pixelIdx + 1) = g;
            *(pixels + pixelIdx + 2) = b;

        }
    }
}

int main()
{
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    uint8_t pixels[WIDTH*HEIGHT*3];
    generateJuliaSet(pixels, WIDTH, HEIGHT, 2.0, 0.248, 0.1);

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            for (int row = 0; row < HEIGHT; row++) {
                for (int col = 0; col < WIDTH; col++) {
                    int pixelIdx = 3 * (row * HEIGHT + col);
                    al_draw_pixel(col, row, al_map_rgb(pixels[pixelIdx], pixels[pixelIdx+1], pixels[pixelIdx+2]));
                }
            }
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}