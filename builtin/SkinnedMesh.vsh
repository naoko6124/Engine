#version 330
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in ivec4 in_bone;
layout (location = 4) in vec4 in_weight;

out vec2 uv;

uniform mat4 model;
uniform mat4 camera;

uniform mat4 bones[32];

void main()
{
    vec4 skin = bones[in_bone[0]] * in_weight[0] * vec4(in_position, 1.0);
    skin += bones[in_bone[1]] * in_weight[1] * vec4(in_position, 1.0);
    gl_Position = camera * model * skin;
    uv = in_uv;
}