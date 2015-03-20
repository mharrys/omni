#include "demo.hpp"
#include "highresolutionclock.hpp"
#include "stdoutlogger.hpp"
#include "window.hpp"
#include "windowbuilder.hpp"
#include "worldrunner.hpp"

int main()
{
    auto logger = std::make_shared<gst::StdoutLogger>();

    gst::WindowBuilder builder(logger);
    builder.set_title("Omnidirectional Shadow Mapping");
    std::shared_ptr<gst::Window> window = builder.build();

    if (window) {
        auto runner = gst::WorldRunner();
        auto clock = gst::HighResolutionClock();
        auto demo = Demo(logger, window);
        return runner.control(demo, clock, *window);
    } else {
        return 1;
    }
}
