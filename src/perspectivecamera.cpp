#include "perspectivecamera.hpp"

PerspectiveCamera::PerspectiveCamera()
    : PerspectiveCamera(45.0f, 1.0, 0.1f, 1000.0f)
{
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, float near, float far)
    : fov(fov),
      aspect_ratio(aspect_ratio),
      near(near),
      far(far)
{
}

void PerspectiveCamera::set_fov(float fov)
{
    this->fov = fov;
}

void PerspectiveCamera::set_aspect_ratio(float aspect_ratio)
{
    this->aspect_ratio = aspect_ratio;
}

void PerspectiveCamera::set_near(float near)
{
    this->near = near;
}

void PerspectiveCamera::set_far(float far)
{
    this->far = far;
}

float PerspectiveCamera::get_fov() const
{
    return fov;
}

float PerspectiveCamera::get_aspect_ratio() const
{
    return aspect_ratio;
}

float PerspectiveCamera::get_near() const
{
    return near;
}

float PerspectiveCamera::get_far() const
{
    return far;
}

glm::mat4 PerspectiveCamera::projection() const
{
    return glm::perspective(fov, aspect_ratio, near, far);
}

