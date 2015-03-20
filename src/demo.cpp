#include "demo.hpp"

Demo::Demo(std::shared_ptr<gst::Logger> logger, std::shared_ptr<gst::Window> window)
    : logger(logger),
      window(window),
      controls(true, 2.8f, 9.0f),
      light_movement(true),
      light_elapsed(0.0f)
{
}

bool Demo::create()
{
    window->set_pointer_lock(true);

    auto device = std::make_shared<gst::GraphicsDeviceImpl>();
    auto synchronizer = std::make_shared<gst::GraphicsSynchronizer>(device, logger);
    auto render_state = std::make_shared<gst::RenderState>(device, synchronizer);
    renderer = gst::Renderer(device, render_state, logger);

    gst::MeshFactory mesh_factory(device, logger);
    gst::ProgramFactory program_factory(device, logger);
    gst::ProgramPool programs(program_factory);

    create_shadow_pass(programs);
    create_light_pass(programs);
    create_scene();
    create_pillars(mesh_factory);
    create_point_light();

    render_state->set_texture(shadow_map);

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

    for (auto face : gst::CUBE_FACES) {
        shadow_pass->set_face(face);
        shadow_target->set_depth({ shadow_map, face });
        renderer.render(scene, shadow_effect, shadow_target);
        renderer.check_errors();
    }

    renderer.render(scene);
    renderer.check_errors();
}

void Demo::destroy()
{
    window->set_pointer_lock(false);
}

void Demo::create_shadow_pass(gst::ProgramPool & programs)
{
    const auto shadow_map_size = 1024;
    const auto shadow_map_data = gst::CubeData();

    shadow_map = std::make_shared<gst::TextureCube>(
        shadow_map_size,
        shadow_map_data
    );
    shadow_map->set_internal_format(gst::TextureFormat::DEPTH_COMPONENT32);
    shadow_map->set_source_format(gst::PixelFormat::DEPTH_COMPONENT);
    shadow_map->set_min_filter(gst::FilterMode::LINEAR);
    shadow_map->set_mag_filter(gst::FilterMode::LINEAR);
    shadow_map->set_wrap_s(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_wrap_t(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_wrap_r(gst::WrapMode::CLAMP_TO_EDGE);
    shadow_map->set_depth_compare(gst::CompareFunc::LEQUAL);

    shadow_pass = std::make_shared<ShadowPass>();
    shadow_pass->cull_face = gst::CullFace::FRONT;
    shadow_pass->depth_test = true;
    shadow_pass->viewport = { shadow_map_size };
    shadow_pass->program = programs.create(SHADOW_VS, SHADOW_FS);

    shadow_target = std::make_shared<gst::FramebufferImpl>();

    auto uniforms = std::make_shared<gst::UniformMapImpl>(
        std::make_shared<gst::AnnotationBasic>()
    );
    shadow_effect = gst::Effect(shadow_pass, uniforms);
}

void Demo::create_light_pass(gst::ProgramPool & programs)
{
    light_pass = std::make_shared<LightPass>();
    light_pass->cull_face = gst::CullFace::BACK;
    light_pass->depth_test = true;
    light_pass->viewport = window->get_size();
    light_pass->program = programs.create(LIGHT_VS, LIGHT_FS);
    light_pass->set_projection(glm::perspective(90.0f, 1.0f, 0.1f, 15.0f));
}

void Demo::create_scene()
{
    const auto size = window->get_size();

    auto camera = std::make_shared<gst::PerspectiveCamera>(45.0f, size, 0.1f, 1000.0f);
    auto eye = std::make_shared<gst::CameraNode>(camera);
    eye->translate_y(5.0f);
    eye->translate_z(40.0f);

    scene = gst::Scene(eye);
}

void Demo::create_pillars(gst::MeshFactory & mesh_factory)
{
    auto uniforms = std::make_shared<gst::UniformMapImpl>(
        std::make_shared<gst::AnnotationStruct>("material")
    );
    uniforms->get_uniform("diffuse") = glm::vec3(1.0f, 1.0f, 1.0f);

    auto effect = gst::Effect(light_pass, uniforms);

    for (auto mesh : mesh_factory.create_from_file(PILLAR_OBJ)) {
        auto model = std::make_shared<gst::Model>(mesh, effect);
        auto model_node = std::make_shared<gst::ModelNode>(model);
        scene.add(model_node);
    }
}

void Demo::create_point_light()
{
    auto uniforms = std::make_shared<gst::UniformMapImpl>(
        std::make_shared<gst::AnnotationStruct>("light")
    );
    uniforms->get_uniform("diffuse") = glm::vec3(0.2f, 0.1f, 1.0f);
    uniforms->get_uniform("attenuation.constant") = 0.9f;
    uniforms->get_uniform("attenuation.linear") = 0.015f;
    uniforms->get_uniform("attenuation.quadratic") = 0.008f;

    auto light = std::make_shared<gst::Light>(uniforms);
    light_node = std::make_shared<gst::LightNode>(light);

    scene.add(light_node);
}

void Demo::update_input(float delta)
{
    const auto input = window->get_input();

    controls.update(delta, input, *scene.get_eye());

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
