#include "linux_gl_context.h"

GLXContext get_glx_context(Display *display) {
    GLXFBConfig *fbc;
    int fbcount;
    // clang-format off
    int fb_attribs[] = {
		GLX_X_RENDERABLE,
		True,
		GLX_DRAWABLE_TYPE,
		GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,
		GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,
		GLX_TRUE_COLOR,
		GLX_RED_SIZE,
		8,
		GLX_GREEN_SIZE,
		8,
		GLX_BLUE_SIZE,
		8,
		GLX_ALPHA_SIZE,
		8,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		True,
		None
	};

    // clang-format on
    fbc = glXChooseFBConfig(display, DefaultScreen(display), fb_attribs,
                            &fbcount);
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
        (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB(
            (const GLubyte *)"glXCreateContextAttribsARB");

    // clang-format off
    int contextAttribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB,
		4,
		GLX_CONTEXT_MINOR_VERSION_ARB,
		6,
		GLX_CONTEXT_PROFILE_MASK_ARB,
		GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};
    // clang-format on
    GLXContext ctx =
        glXCreateContextAttribsARB(display, fbc[0], 0, True, contextAttribs);
    return ctx;
}
