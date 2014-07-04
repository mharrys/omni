#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLDUED

#include <memory>
#include <string>
#include <utility>

struct NativeWindow;

struct WindowSetting {
    std::string title;
    unsigned int width;
    unsigned int height;
    bool fullscreen;
    bool exit_on_close;
    bool exit_on_esc;
};

class Window {
public:
    Window();
    Window(WindowSetting setting);
    ~Window();

    bool open();
    void close();

    void swap();
    void poll();

    bool is_open() const;
    bool should_close() const;

    std::pair<unsigned int, unsigned int> dimension() const;
private:
    void reset();
private:
    std::unique_ptr<NativeWindow> native;

    bool exit_flag;
    WindowSetting setting;
};

#endif
