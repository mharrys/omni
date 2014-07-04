#version 130

uniform samplerCubeShadow shadow_map;

// material ambient and diffuse
uniform vec3 Ka = vec3(0.3);
uniform vec3 Kd = vec3(0.2);

// light source ambient and diffuse
uniform vec3 La = vec3(0.2);
uniform vec3 Ld = vec3(0.2);

uniform float light_near;
uniform float light_far;
uniform vec4 light_position;

uniform bool shadow_on = true;
uniform bool shadow_only = false;

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
    float depth = (z_ndc + 1.0) / 2.0;

    // samples texel from shadow map at specified coordinates, since this is a
    // sampleCubeShadow the texture function will use the texture compare function
    // and return 0 for fragment position in shadow, 1 otherwise
    return texture(shadow_map, vec4(position_ls, depth));
}

vec3 lambert_model(float shadow)
{
    // direction vector from the point on surface towards the light source
    vec3 s = normalize(light_position.xyz - position.xyz);

    vec3 ambient = La * Ka;
    vec3 diffuse = Ld * Kd * max(dot(s, normal), 0.0);

    return ambient + diffuse * shadow;
}

void main()
{
    float shadow = shadow_on ? depth_test() : 1.0;

    if (shadow_only) {
        frag_color = vec4(vec3(shadow), 1.0);
    } else {
        frag_color = vec4(lambert_model(shadow), 1.0);
    }
}
