#include "demo.hpp"

Demo::Demo(std::shared_ptr<gst::Logger> logger, std::shared_ptr<gst::Window> window)
    : logger(logger),
      window(window),
      renderer(gst::Renderer::create(logger)),
      controls(true, 2.8f, 9.0f),
      programs(logger),
      render_size(window->get_size()),
      shadow_size(1024),
      light_movement(true),
      light_elapsed(0.0f)
{
}

bool Demo::create()
{
    window->set_pointer_lock(true);

    create_shadow_pass();
    create_light_pass();
    create_scene();
    create_model();
    create_point_light();

    return true;
}

void Demo::update(float delta, float)
{
    update_input(delta);
    update_light(delta);
    scene.update();

    auto light_view = glm::inverse(light_node->world_transform);
    shadow_pass->set_view(light_view);
    light_pass->set_view(light_view);

    renderer.set_viewport(shadow_size);
    for (auto face : gst::CUBE_FACES) {
        shadow_pass->set_face(face);
        shadow_target->set_depth({ shadow_map, face });
        renderer.render(scene, shadow, shadow_target);
    }

    renderer.set_viewport(render_size);
    renderer.render(scene);
}

void Demo::destroy()
{
    window->set_pointer_lock(false);
}

void Demo::create_shadow_pass()
{
    auto shadow_map_size = shadow_size.get_width();

    shadow_map = std::make_shared<gst::TextureCube>(gst::TextureCube::create_empty(shadow_map_size));
    shadow_map->set_internal_format(gst::TextureFormat::DEPTH_COMPONENT32);
    shadow_map->set_source_format(gst::PixelFormat::DEPTH_COMPONENT);
    shadow_map->set_wrap_s(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_wrap_t(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_wrap_r(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_depth_compare(gst::CompareFunc::LEQUAL);

    auto shadow_program = programs.create(SHADOW_VS, SHADOW_FS);
    shadow_pass = std::make_shared<ShadowPass>(shadow_program);
    shadow_pass->set_cull_face(gst::CullFace::FRONT);
    shadow_pass->set_depth_test(true);
    shadow = gst::Filter(gst::Material::create_free(), shadow_pass);

    shadow_target = std::make_shared<gst::FramebufferImpl>();
}

void Demo::create_light_pass()
{
    auto light_program = programs.create(LIGHT_VS, LIGHT_FS);
    light_pass = std::make_shared<LightPass>(light_program);
    light_pass->set_cull_face(gst::CullFace::BACK);
    light_pass->set_depth_test(true);
    light_pass->set_projection(glm::perspective(90.0f, 1.0f, 0.1f, 15.0f));
}

void Demo::create_scene()
{
    auto camera = std::unique_ptr<gst::Camera>(new gst::PerspectiveCamera(45.0f, render_size, 0.1f, 1000.0f));
    auto eye = std::make_shared<gst::CameraNode>(std::move(camera));
    eye->position = glm::vec3(0.0f, 5.0f, 40.0f);
    scene = gst::Scene(eye);
}

void Demo::create_model()
{
    auto material = gst::Material::create_struct("material");
    material.get_uniform("diffuse") = glm::vec3(1.0f);

    const auto unit = 0;
    material.get_textures()[unit] = shadow_map;
    material.get_uniform("shadow_map") = unit;

    gst::MeshFactory mesh_factory(logger);
    for (auto mesh : mesh_factory.create_from_file(PILLAR_OBJ)) {
        auto model = gst::Model(mesh, material, light_pass);
        auto model_node = std::make_shared<gst::ModelNode>(model);
        scene.add(model_node);
    }
}

void Demo::create_point_light()
{
    auto light = gst::Light::create_struct("light");
    light.get_uniform("diffuse") = glm::vec3(0.2f, 0.1f, 1.0f);
    light.get_uniform("attenuation.constant") = 0.9f;
    light.get_uniform("attenuation.linear") = 0.015f;
    light.get_uniform("attenuation.quadratic") = 0.008f;

    light_node = std::make_shared<gst::LightNode>(light);
    scene.add(light_node);
}

void Demo::update_input(float delta)
{
    const auto input = window->get_input();

    controls.update(delta, input, scene.get_eye());

    if (input.pressed(gst::Key::F1)) {
        light_pass->set_shadow_on(!light_pass->get_shadow_on());
    }

    if (input.pressed(gst::Key::F2)) {
        light_pass->set_shadow_only(!light_pass->get_shadow_only());
    }

    if (input.pressed(gst::Key::SPACE)) {
        light_movement = !light_movement;
    }
}

void Demo::update_light(float delta)
{
    if (light_movement) {
        light_elapsed += delta;
        const float light_distance = 3.5f;
        const float w = ((2.0f * PI) / 10.0f);
        light_node->position.x = light_distance * sinf(w * light_elapsed);
        light_node->position.y = 10.0f;
        light_node->position.z = light_distance * cosf(w * light_elapsed);
    }
}
