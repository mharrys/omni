#ifndef LIGHTPASS_HPP_INCLUDED
#define LIGHTPASS_HPP_INCLUDED

#include "pass.hpp"

// The responsibility of this class is to setup a program object for a
// shaded shader pass with shadow mapping.
class LightPass : public gst::Pass {
public:
    LightPass();
    void apply(gst::ModelState const & state) final;
    // Set the light camera view.
    void set_view(glm::mat4 view);
    // Set distance of near plane in projection used by light camera.
    void set_near(float near);
    // Set distance of far plane in projection used by light camera.
    void set_far(float far);
    // Set shadow visibility state.
    void set_shadow_on(bool shadow_on);
    // Set shadow without shading state.
    void set_shadow_only(bool shadow_only);
    // Return shadow visibility state.
    bool get_shadow_on() const;
    // Return shadow without shading state.
    bool get_shadow_only() const;
private:
    std::shared_ptr<gst::UniformMap> uniforms;
    glm::mat4 light_view;
    float light_near;
    float light_far;
    bool shadow_on;
    bool shadow_only;
};

#endif
