#include "shadowpass.hpp"

#include "annotationbasic.hpp"
#include "program.hpp"
#include "shadoweddata.hpp"
#include "uniformmapimpl.hpp"

ShadowPass::ShadowPass()
    : uniforms(std::make_shared<gst::UniformMapImpl>(std::make_shared<gst::AnnotationBasic>())),
      light_projection(glm::perspective(90.0f, 1.0f, 0.1f, 15.0f))
{
    glm::vec3 origin(0.0f);
    rotations = {
        glm::lookAt(origin,  X_UNIT, -Y_UNIT),
        glm::lookAt(origin, -X_UNIT, -Y_UNIT),
        glm::lookAt(origin,  Y_UNIT,  Z_UNIT),
        glm::lookAt(origin, -Y_UNIT, -Z_UNIT),
        glm::lookAt(origin,  Z_UNIT, -Y_UNIT),
        glm::lookAt(origin, -Z_UNIT, -Y_UNIT),
    };
    active_face = rotations[0];
}

void ShadowPass::apply(gst::ModelState const & state)
{
    uniforms->get_uniform("mvp") = light_projection * active_face * light_view * state.model;
    program->set_uniforms(*uniforms);
}

void ShadowPass::set_face(gst::CubeFace face)
{
    active_face = rotations[static_cast<int>(face)];
}

void ShadowPass::set_view(glm::mat4 view)
{
    light_view = view;
}
