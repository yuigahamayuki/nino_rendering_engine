#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coord;
layout (location = 3) in ivec4 bone_ids;
layout (location = 4) in vec4 bone_weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 512;
uniform mat4 bone_transforms[MAX_BONES];

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}