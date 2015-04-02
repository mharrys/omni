#ifndef SHADOWPASS_HPP_INCLUDED
#define SHADOWPASS_HPP_INCLUDED

#include "gust.hpp"

// The responsibility of this class is to setup a program object for a
// shadow shader pass of a point light.
class ShadowPass : public gst::Pass {
public:
    ShadowPass(std::shared_ptr<gst::Program> program);
    void apply(gst::ModelState & state) final;
    // Set the active face from which the camera (placed in a cube) will be looking
    // out through.
    void set_face(gst::CubeFace face);
    // Set the active light camera view from spatial.
    void set_view(glm::mat4 view);
private:
    std::shared_ptr<gst::UniformMap> uniforms;

    // rotation matrices for orienting the light camera in all directions of a
    // cube
    std::vector<glm::mat4> rotations;
    glm::mat4 active_face;

    // light camera projection must have a square aspect ratio due to using a
    // texture cube map, and it must have a 90 degrees field of view to "see"
    // all areas without gaps when combining all face renders
    glm::mat4 light_projection;
    glm::mat4 light_view;
};

#endif
