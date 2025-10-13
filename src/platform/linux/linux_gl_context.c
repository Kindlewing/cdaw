#include "linux_gl_context.h"

static bool ctx_error_occurred = false;
static int ctx_error_handler(Display *display, XErrorEvent *event) {
    (void)display;
    (void)event;
    ctx_error_occurred = true;
    return 0;
}

static bool is_extension_supported(const char *ext_list,
                                   const char *extension) {
    if(!ext_list || !extension) {
        return false;
    }

    if(*extension == '\0') {
        return false;
    }

    for(const char *p = extension; *p; ++p) {
        if(*p == ' ') {
            return false;
        }
    }
    const char *ext_ptr = ext_list;
    while(*ext_ptr) {
        // skip leading spaces
        while(*ext_ptr == ' ')
            ++ext_ptr;
        if(*ext_ptr == '\0')
            break;

        const char *token_start = ext_ptr;
        const char *token_end = ext_ptr;
        while(*token_end && *token_end != ' ')
            ++token_end;

        const char *a = token_start;
        const char *b = extension;
        while(*a && *b && a < token_end && *a == *b) {
            ++a;
            ++b;
        }

        if(a == token_end && *b == '\0') {
            return true;
        }
        ext_ptr = token_end;
    }

    return false;
}

GLXContext get_glx_context(Display *display) {
    int glx_major = 0;
    int glx_minor = 0;

    if(!glXQueryVersion(display, &glx_major, &glx_minor) ||
       (glx_major == 1 && glx_minor < 3)) {
        fprintf(stderr, "GLX 1.3 or later is required.\n");
        exit(EXIT_FAILURE);
    }
    // clang-format off
    static int fb_attribs[] = {
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

    int fb_count = 0;
    GLXFBConfig *fb_configs = glXChooseFBConfig(display, DefaultScreen(display),
                                                fb_attribs, &fb_count);
    if(!fb_configs || fb_count == 0) {
        fprintf(stderr, "Failed to retrieve framebuffer config.\n");
        exit(EXIT_FAILURE);
    }

    GLXFBConfig best_fb_config = fb_configs[0];
    XFree(fb_configs);

    const char *glx_exts =
        glXQueryExtensionsString(display, DefaultScreen(display));
    bool has_arb_create_context =
        is_extension_supported(glx_exts, "GLX_ARB_create_context");

    typedef GLXContext (*pfn_glx_create_context_attribs_arb)(
        Display *, GLXFBConfig, GLXContext, Bool, const int *);

    pfn_glx_create_context_attribs_arb glx_create_context_attribs_arb =
        (pfn_glx_create_context_attribs_arb)glXGetProcAddressARB(
            (const GLubyte *)"glXCreateContextAttribsARB");

    GLXContext ctx = 0;

    int (*old_handler)(Display *, XErrorEvent *) =
        XSetErrorHandler(&ctx_error_handler);
    ctx_error_occurred = false;

    if(has_arb_create_context && glx_create_context_attribs_arb) {
        // clang-format off
        int context_attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB,
			4,
			GLX_CONTEXT_MINOR_VERSION_ARB,
			6,
			GLX_CONTEXT_PROFILE_MASK_ARB,
			GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};
        // clang-format on
        ctx = glx_create_context_attribs_arb(display, best_fb_config, 0, True,
                                             context_attribs);
        XSync(display, False);
        if(ctx_error_occurred || !ctx) {
            fprintf(stderr, "Failed to create OpenGL 4.6 context. Falling back "
                            "to legacy.\n");
            ctx_error_occurred = false;
            int legacy_attribs[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
                                    GLX_CONTEXT_MINOR_VERSION_ARB, 1, None};
            ctx = glx_create_context_attribs_arb(display, best_fb_config, 0,
                                                 True, legacy_attribs);
        }
    } else {
        fprintf(
            stderr,
            "GLX_ARB_create_context not supported. Using old-style context.\n");
        ctx = glXCreateNewContext(display, best_fb_config, GLX_RGBA_TYPE, 0,
                                  True);
    }

    XSync(display, False);
    XSetErrorHandler(old_handler);

    if(ctx_error_occurred || !ctx) {
        fprintf(stderr, "Failed to create any OpenGL context.\n");
        exit(EXIT_FAILURE);
    }

    if(!glXIsDirect(display, ctx))
        fprintf(stderr, "Warning: indirect GLX rendering context obtained.\n");

    return ctx;
}
