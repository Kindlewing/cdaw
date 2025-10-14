#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "typedefs.h"
#include "linux_gl_context.h"

int main(void) {
    XSetWindowAttributes win_attribs = {0};
    Window window = {0};
    Display *display = {0};
    GLXContext gl_context = {0};
    XWindowAttributes window_attribs = {0};
    XEvent xevent = {0};

    display = XOpenDisplay(NULL);
    if(display == NULL) {
        perror("Error connecting to X11 server");
        exit(EXIT_FAILURE);
    }

    int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    Window root = DefaultRootWindow(display);
    XVisualInfo *vi = glXChooseVisual(display, 0, attributes);
    if(vi == NULL) {
        printf("No suitable visual found\n");
        exit(0);
    }
    printf("Visual %p selected\n", (void *)vi->visualid);
    Colormap colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    win_attribs.colormap = colormap;
    win_attribs.event_mask = ExposureMask | KeyPressMask;
    u32 x = 0;
    u32 y = 0;
    u32 width = 800;
    u32 height = 800;
    u32 border_width = 0;
    window = XCreateWindow(display, root, x, y, width, height, border_width,
                           vi->depth, InputOutput, vi->visual,
                           CWColormap | CWEventMask, &win_attribs);
    XMapWindow(display, window);
    XStoreName(display, window, "GUI");
    gl_context = get_glx_context(display);
    return -1;
    glXMakeContextCurrent(display, window, window, gl_context);
    glEnable(GL_DEPTH_TEST);

    bool window_should_close = false;
    while(!window_should_close) {
        XNextEvent(display, &xevent);
        if(xevent.type == KeyPress) {
            window_should_close = true;
        }
        XGetWindowAttributes(display, window, &window_attribs);
        glClearColor(30.0 / 255.0, 30.0 / 255.0, 46.0 / 255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window_attribs.width, window_attribs.height);
        glXSwapBuffers(display, window);
    }
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, gl_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
