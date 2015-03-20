#ifndef DEMO_HPP_INCLUDED
#define DEMO_HPP_INCLUDED

#include "assets.hpp"
#include "lightpass.hpp"
#include "shadowpass.hpp"

#include "gust.hpp"

class Demo : public gst::World {
public:
    Demo(std::shared_ptr<gst::Logger> logger, std::shared_ptr<gst::Window> window);
    bool create() final;
    void update(float delta, float elapsed) final;
    void destroy() final;
private:
    void create_shadow_pass(gst::ProgramPool & programs);
    void create_light_pass(gst::ProgramPool & programs);
    void create_scene();
    void create_pillars(gst::MeshFactory & factory);
    void create_point_light();
    void update_input(float delta);
    void update_light(float delta);

    std::shared_ptr<gst::Logger> logger;
    std::shared_ptr<gst::Window> window;

    gst::Renderer renderer;
    gst::Scene scene;
    gst::FirstPersonControl controls;

    std::shared_ptr<gst::LightNode> light_node;

    std::shared_ptr<gst::TextureCube> shadow_map;
    std::shared_ptr<ShadowPass> shadow_pass;
    std::shared_ptr<gst::Framebuffer> shadow_target;
    gst::Effect shadow_effect;

    std::shared_ptr<LightPass> light_pass;

    bool light_movement;
    float light_elapsed;
};

#endif
