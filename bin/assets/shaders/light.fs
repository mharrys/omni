#version 130

uniform samplerCubeShadow shadow_map;

struct Material {
    vec3 diffuse;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Light {
    bool enabled;
    vec4 position;
    vec3 diffuse;
    Attenuation attenuation;
};

uniform Material material;
uniform Light light;

uniform float light_near;
uniform float light_far;

uniform bool shadow_on;
uniform bool shadow_only;

in vec3 position;
in vec3 position_ls;
in vec3 normal;

out vec4 frag_color;

float depth_test()
{
    // select the correct cube map face using the fragment position in light
    // space as a direction vector
    vec3 abs_position_ls = abs(position_ls);
    float major_axis_magnitude = max(abs_position_ls.x, max(abs_position_ls.y, abs_position_ls.z));

    // compute normalized device coordinates
    float f = light_far;
    float n = light_near;
    float z = -major_axis_magnitude;
    float z_ndc = ((f + n) / (f - n)) + ((2.0 * f * n) / (z * (f - n)));

    // compute texture coordinates
    float depth = (z_ndc + 1.0) * 0.5;

    // samples texel from shadow map at specified coordinates, since this is a
    // sampleCubeShadow the texture function will use the texture compare function
    // and return 0 for fragment position in shadow, 1 otherwise
    return texture(shadow_map, vec4(position_ls, depth));
}

vec3 lambert_model()
{
    // direction vector from the point on surface towards the light source
    vec3 s = normalize(light.position.xyz - position);
    vec3 diffuse = (light.diffuse * material.diffuse) * max(dot(normal, s), 0.0);
    return clamp(diffuse, 0.0, 1.0);
}

float attenuation(Light light)
{
    float light_distance = distance(light.position.xyz, position.xyz);
    float denom = 0.0;
    denom += light.attenuation.constant;
    denom += light.attenuation.linear * light_distance;
    denom += light.attenuation.quadratic * light_distance * light_distance;
    return 1.0 / denom;
}

void main()
{
    float shadow = shadow_on ? depth_test() : 1.0;

    vec3 color = vec3(shadow);

    if (!shadow_only) {
        color = lambert_model() * shadow * attenuation(light);
    }

    frag_color = vec4(color, 1.0);
}
