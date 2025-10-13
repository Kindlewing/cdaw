#ifndef LINUX_GL_CONTEXT_H
#define LINUX_GL_CONTEXT_H
#include <GL/glx.h>
#include <X11/X.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
GLXContext get_glx_context(Display *display);
#endif /* LINUX_GL_CONTEXT */
