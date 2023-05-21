#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "generateJuliaSet.h"

int WIDTH = 512;
int HEIGHT = 512;

double max(double a, double b) {
    return (a < b) ? b : a;
}

void generateJuliaSetC(uint8_t *pixels, int width, int height, double escapeRadius, double cReal, double cImag, double centerReal, double centerImag, double zoom) {
    for (int row = height-1; row >= 0; row--) {
        for (int col = width-1; col >= 0; col--) {
            double zReal = (col - centerReal) * escapeRadius * 2 / (width * zoom);
            double zImag = (row - centerImag) * escapeRadius * 2 / (height * zoom);

            int iteration = 0;
            int maxIteration = 100;

            while (zReal * zReal + zImag * zImag < escapeRadius * escapeRadius && iteration < maxIteration) {
                double tmpReal = zReal * zReal - zImag * zImag;
                zImag = 2 * zReal * zImag + cImag;
                zReal = tmpReal + cReal;

                iteration++;
            }

            uint8_t r, g, b;
            r = ((maxIteration - iteration) * 255) / maxIteration;
            g = 0;
            b = 0;

            int pixelIdx = 3 * (row * height + col);
            *(pixels + pixelIdx) = r;
            *(pixels + pixelIdx + 1) = g;
            *(pixels + pixelIdx + 2) = b;

        }
    }
}

/**
 * Display 24-bit RGB pixels stored in an array on the currently active bitmap
 * @param pixelArray - array of RGB pixel values
 * @param width - width of the pixel array
 * @param height - height of the pixel array
 */
void displayRGBPixels(uint8_t *pixelArray, int width, int height) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int pixelIdx = 3 * (row * height + col);
            al_draw_pixel(col, row, al_map_rgb(
                    pixelArray[pixelIdx],
                    pixelArray[pixelIdx + 1],
                    pixelArray[pixelIdx + 2]
            ));
        }
    }
}

int main() {
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    uint8_t pixels[WIDTH * HEIGHT * 3];
    double offsetReal = (double)WIDTH / 2;
    double offsetImag = (double)HEIGHT / 2;
    double zoom = 1.0;

    double cReal = 0.248;
    double cImag = 0.1;
    double escapeRadius = 2.0;

    double deltaOffset = 10.0;
    double deltaC = 0.05;
    double deltaZoom = 0.05;

    generateJuliaSet(pixels, WIDTH, HEIGHT, escapeRadius, cReal, cImag, offsetReal, offsetImag, zoom);
    displayRGBPixels(pixels, WIDTH, HEIGHT);


    al_start_timer(timer);
    while (true) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                break;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                cReal -= deltaC;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                cReal += deltaC;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                cImag += deltaC;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                cImag -= deltaC;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                offsetImag -= deltaOffset;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                offsetReal -= deltaOffset;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                offsetImag += deltaOffset;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                offsetReal += deltaOffset;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
                zoom = max(zoom - deltaZoom, 0);
            } else if (event.keyboard.keycode == ALLEGRO_KEY_E) {
                zoom += deltaZoom;
            }

            redraw = true;

        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            generateJuliaSet(pixels, WIDTH, HEIGHT, escapeRadius, cReal, cImag, offsetReal, offsetImag, zoom);
            displayRGBPixels(pixels, WIDTH, HEIGHT);
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