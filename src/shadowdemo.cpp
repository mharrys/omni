#include "shadowdemo.hpp"

#include "mesh.hpp"
#include "objloader.hpp"
#include "worldobject.hpp"

#include "lib/gl.hpp"

#include <iostream>

ShadowDemo::ShadowDemo()
    : shadow_program("assets/shaders/shadow.vert", "assets/shaders/shadow.frag"),
      light_program("assets/shaders/light.vert", "assets/shaders/light.frag"),
      shadow_map_resolution(1024)
{
}

bool ShadowDemo::on_create(int window_width, int window_height)
{
    this->window_width = window_width;
    this->window_height = window_height;

    if (!shadow_program.is_valid()) {
        std::cerr << "ShadowDemo::on_create: shadow program is invalid." << std::endl;
        return false;
    }

    if (!light_program.is_valid()) {
        std::cerr << "ShadowDemo::on_create: light program is invalid." << std::endl;
        return false;
    }

    // create shadow map storage
    glGenTextures(1, &shadow_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // comparision operator
    //     result = 1.0 if r <= D
    //     result = 0.0 if r > D
    //  where r is the interpolated and clamped texture coordinate that is
    //  compared to the value for the currently bound depth texture D
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    for (GLint face = 0; face < 6; face++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            0,
            GL_DEPTH_COMPONENT24,
            shadow_map_resolution,
            shadow_map_resolution,
            0,
            GL_DEPTH_COMPONENT,
            GL_UNSIGNED_BYTE,
            0
        );
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (!glIsTexture(shadow_map)) {
        std::cerr << "ShadowDemo::on_create: could not create shadow map." << std::endl;
        return false;
    }

    // create framebuffer for offscreen renderings into the shadow map storage
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // validate each face in the shadow map
    for (GLint face = 0; face < 6; face++) {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            shadow_map,
            0
        );
        GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ShadowDemo::on_create: framebuffer not complete, status: " << fbo_status << std::endl;
            return false;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!glIsFramebuffer(framebuffer)) {
        std::cerr << "ShadowDemo::on_create: could not create framebuffer." << std::endl;
        return false;
    }

    model = OBJLoader::load("assets/models/pillar.obj");
    if (!model) {
        std::cerr << "ShadowDemo::on_create: unable to load model." << std::endl;
        return false;
    }

    camera.set_aspect_ratio(window_width / static_cast<float>(window_height));
    camera.rotate_x(-35.0f);
    camera.translate_y(-2.5f);
    camera.translate_z(40.0f);
    camera.update_world_transform();

    light_camera.set_aspect_ratio(1.0f);
    light_camera.set_fov(90.0f);
    light_camera.set_far(15.0f);
    light_camera.update_world_transform();

    // prepare rotation matrices for orienting the light camera to point in
    // all directions of a cube
    glm::vec3 origin(0.0f);
    rotations.push_back(glm::lookAt(origin, glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f))); // +x
    rotations.push_back(glm::lookAt(origin, glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f))); // -x
    rotations.push_back(glm::lookAt(origin, glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f))); // +y
    rotations.push_back(glm::lookAt(origin, glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f))); // -y
    rotations.push_back(glm::lookAt(origin, glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f))); // +z
    rotations.push_back(glm::lookAt(origin, glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))); // -z

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void ShadowDemo::on_update(seconds delta, seconds elapsed)
{
    if (shadow_program.is_modified()) {
        shadow_program.reload();
    }

    if (light_program.is_modified()) {
        light_program.reload();
    }

    // move the light around in a circle with a period of 10 seconds
    float light_distance = 3.5f;
    float w = ((2.0f * PI) / 10.0f);
    float x = light_distance * sinf(w * elapsed.count());
    float y = 6.0f;
    float z = light_distance * cosf(w * elapsed.count());
    light_camera.set_position(glm::vec3(x, y, z));
    light_camera.update_world_transform();

    model->rotate_y(-30.0f * delta.count());
    model->update_world_transform();
}

void ShadowDemo::on_render()
{
    render_pass_shadow();
    render_pass_light();
}

void ShadowDemo::on_destroy()
{
    if (glIsTexture(shadow_map)) {
        glDeleteTextures(1, &shadow_map);
    }

    if (glIsFramebuffer(framebuffer)) {
        glDeleteFramebuffers(1, &framebuffer);
    }
}

void ShadowDemo::render_scene()
{
    model->traverse([&](WorldObject & object) {
        object.draw();
    });
}

void ShadowDemo::render_pass_shadow()
{
    shadow_program.use();

    glm::mat4 m = model->world_transform();
    glm::mat4 v = light_camera.view();
    glm::mat4 mv = v * m;

    glViewport(0, 0, shadow_map_resolution, shadow_map_resolution);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.1f, 2.8f);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);

    for (GLint face = 0; face < 6; face++) {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            shadow_map,
            0
        );

        glm::mat4 mvp = light_camera.projection() * rotations[face] * mv;
        shadow_program.set_uniform("mvp", mvp);

        glClear(GL_DEPTH_BUFFER_BIT);
        render_scene();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void ShadowDemo::render_pass_light()
{
    light_program.use();

    glm::mat4 m = model->world_transform();
    glm::mat4 v = camera.view();
    glm::mat4 p = camera.projection();
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv));

    // light position in eye space
    glm::vec4 light_position = v * glm::vec4(light_camera.get_position(), 1.0);

    light_program.set_uniform("m", m);
    light_program.set_uniform("mv", mv);
    light_program.set_uniform("mvp", mvp);
    light_program.set_uniform("nm", nm);
    light_program.set_uniform("light_position", light_position);
    light_program.set_uniform("light_near", light_camera.get_near());
    light_program.set_uniform("light_far", light_camera.get_far());
    light_program.set_uniform("light_view", light_camera.view());

    glViewport(0, 0, window_width, window_height);
    glCullFace(GL_BACK);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_scene();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
