layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coord;
layout (location = 3) in ivec4 bone_ids;
layout (location = 4) in vec4 bone_weights;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bone_transforms[170];

void main()
{
    // mat4 bone_transform = bone_transforms[bone_ids[0]] * bone_weights[0];
    // if (bone_ids[1] > 0) {
    //   bone_transform  += bone_transforms[bone_ids[1]] * bone_weights[1];
    // }
    // if (bone_ids[2] > 0) {
    //   bone_transform  += bone_transforms[bone_ids[2]] * bone_weights[2];
    // }
    // if (bone_ids[3] > 0) {
    //   bone_transform  += bone_transforms[bone_ids[3]] * bone_weights[3];
    // }


    // gl_Position = projection * view * model * bone_transform * vec4(position, 1.0f);
    tex_coord = vec2(texture_coord.x, 1.f - texture_coord.y);
    gl_Position = projection * view * model *  vec4(position, 1.0f);
}