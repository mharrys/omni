#include "lightpass.hpp"

LightPass::LightPass(std::shared_ptr<gst::Program> program)
    : Pass(program),
      uniforms(std::make_shared<gst::UniformMapImpl>(std::unique_ptr<gst::AnnotationFormatter>(new gst::AnnotationFree()))),
      shadow_on(true),
      shadow_only(false)
{
}

void LightPass::apply(gst::ModelState & model_state)
{
    auto light_space = light_view * model_state.model;

    uniforms->get_uniform("model_view") = model_state.model_view;
    uniforms->get_uniform("projection") = model_state.projection;
    uniforms->get_uniform("nm") = model_state.normal;
    uniforms->get_uniform("ls") = light_space;
    uniforms->get_uniform("light_projection") = light_projection;
    uniforms->get_uniform("shadow_on") = shadow_on;
    uniforms->get_uniform("shadow_only") = shadow_only;
    program->merge_uniforms(*uniforms);

    for (auto light_node : model_state.light_nodes) {
        auto & light = light_node.get_light();
        light.get_uniform("position") = model_state.view * glm::vec4(light_node.position, 1.0f);
        program->merge_uniforms(light.get_uniforms());
    }
}

void LightPass::set_view(glm::mat4 view)
{
    light_view = view;
}

void LightPass::set_projection(glm::mat4 projection)
{
    light_projection = projection;
}

void LightPass::set_shadow_on(bool shadow_on)
{
    this->shadow_on = shadow_on;
}

void LightPass::set_shadow_only(bool shadow_only)
{
    this->shadow_only = shadow_only;
}

bool LightPass::get_shadow_on() const
{
    return shadow_on;
}

bool LightPass::get_shadow_only() const
{
    return shadow_only;
}
