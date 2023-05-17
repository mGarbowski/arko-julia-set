#include <X11/Xlib.h>
#include <stdio.h>

int main() {
    Display *display;
    Window window;
    XEvent event;

    display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    unsigned long white = WhitePixel(display, screen);

    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 800, 600, 0, white, white);
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            /* Draw the window contents here */
        }

        if (event.type == KeyPress)
            break;
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
