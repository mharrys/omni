#ifndef DEMO_HPP_INCLUDED
#define DEMO_HPP_INCLUDED

#include "perspectivecamera.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "world.hpp"

#include "lib/gl.hpp"

#include <memory>

class Demo : public World {
public:
    Demo();

    bool on_create(int window_width, int window_height);
    void on_update(seconds delta, seconds elapsed);
    void on_render();
    void on_destroy();
private:
    void render_scene();
    void render_pass_shadow();
    void render_pass_light();
private:
    int window_width;
    int window_height;

    Program shadow_program;
    Program light_program;

    PerspectiveCamera camera;
    PerspectiveCamera light_camera;

    std::unique_ptr<WorldObject> model;

    int shadow_map_resolution;
    std::vector<glm::mat4> rotations;

    GLuint shadow_map;
    GLuint framebuffer;
};

#endif
