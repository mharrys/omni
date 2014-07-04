#ifndef PERSPECTIVECAMERA_HPP_INCLUDED
#define PERSPECTIVECAMERA_HPP_INCLUDED

#include "camera.hpp"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    PerspectiveCamera(float fov, float aspect_ratio, float near, float far);

    void set_fov(float fov);
    void set_aspect_ratio(float aspect_ratio);
    void set_near(float near);
    void set_far(float far);

    float get_fov() const;
    float get_aspect_ratio() const;
    float get_near() const;
    float get_far() const;

    glm::mat4 projection() const override;
private:
    float fov;
    float aspect_ratio;
    float near;
    float far;
};

#endif
