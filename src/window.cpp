#include "window.hpp"

#include "lib/gl.hpp"
#include "lib/sdl.hpp"

#include <iostream>

static const WindowSetting DEFAULT_WINDOW_SETTING {
    "OpenGL Window",
    800,
    600,
    false,
    true,
    true
};

Window::Window()
    : Window(DEFAULT_WINDOW_SETTING)
{
}

Window::Window(WindowSetting setting)
    : native(new NativeWindow()),
      setting(setting)
{
    reset();
}

Window::~Window()
{
    if (is_open()) {
        close();
    }
}

bool Window::open()
{
    if (is_open()) {
        std::cerr << "Window::open: attempted to open already open window." << std::endl;
        return false;
    }

    GLenum glew_error;

    int mask = SDL_WINDOW_OPENGL;
    if (setting.fullscreen) {
        mask |= SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        std::cerr << "Window::open: unable to initialize SDL: " << SDL_GetError() << std::endl;
        goto err_init;
    }

    native->window = SDL_CreateWindow(
        setting.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // respect the window manager
        setting.width,
        setting.height,
        mask
    );
    if (!native->window) {
        std::cerr << "Window::open: could not create native window: " << SDL_GetError() << std::endl;
        goto err_window;
    }

    native->context = SDL_GL_CreateContext(native->window);
    if (!native->context) {
        std::cerr << "Window::open: could not create native window context: " << SDL_GetError() << std::endl;
        goto err_context;
    }

    glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        std::cerr << "Window::open: could not initialize glew: " << glewGetErrorString(glew_error) << std::endl;
        goto err_glew;
    }

    if (!GLEW_VERSION_3_0) {
        std::cerr << "Window::open: OpenGL 3.0 API support is required." << std::endl;
        goto err_glew;
    }

    return true;

err_glew:
    SDL_GL_DeleteContext(native->context);
err_context:
    SDL_DestroyWindow(native->window);
err_window:
    SDL_Quit();
err_init:
    reset();
    return false;
}

void Window::close()
{
    if (!is_open()) {
        std::cerr << "Window::close: attempted to close already closed window." << std::endl;
    } else {
        SDL_GL_DeleteContext(native->context);
        SDL_DestroyWindow(native->window);
        SDL_Quit();
        reset();
    }
}

void Window::swap()
{
    if (!is_open()) {
        std::cerr << "Window::swap: attempted to swap buffers on closed window." << std::endl;
    } else {
        SDL_GL_SwapWindow(native->window);
    }
}

void Window::poll()
{
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
        case SDL_QUIT:
            if (setting.exit_on_close) {
                exit_flag = true;
            }
            break;
        case SDL_KEYUP:
            if (setting.exit_on_esc && sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                exit_flag = true;
            }
            break;
        }
    }
}

bool Window::is_open() const
{
    return (native->window || native->context);
}

bool Window::should_close() const
{
    return exit_flag;
}

std::pair<unsigned int, unsigned int> Window::dimension() const
{
    return std::make_pair(setting.width, setting.height);
}

void Window::reset()
{
    native->window = 0;
    native->context = 0;
    exit_flag = false;
}
