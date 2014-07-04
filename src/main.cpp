#include "runner.hpp"
#include "shadowdemo.hpp"

int main()
{
    Window window;
    if (window.open()) {
        Runner runner;
        ShadowDemo demo;

        return runner.control(window, demo);
    }

    return 1;
}
