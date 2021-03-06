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
uniform int blendshape_number;
uniform int blendshape_vertex_base;
uniform int vertex_count_per_blendshape;  // same as base mesh's vertex count

layout(std140) uniform BlendshapeWeights
{
    // Pack the floats to vec4, that is, one row has 4 floats.
    // To get the value of blendshape index i: blendshape_weights[i / 4][i % 4].
    // vec4 blendshape_weights[];  // Not ok: OpenGL requires constant indexes for unsized array access
    vec4 blendshape_weights[52];
};

struct BlendshapeVertex
{
    float position_x;
    float position_y;
    float position_z;
    float normal_x;
    float normal_y;
    float normal_z;
};

layout(std430) buffer BlendshapeVertices
{
    // To get the i-th vertex's position:
    // float pos_x = blendshape_vertices[i].position_x; ...
    BlendshapeVertex blendshape_vertices[];
};

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
    // gl_Position = projection * view * model * vec4(position, 1.0f);
    vec4 out_position = vec4(position, 1.0f);
    for (int i = 0; i < blendshape_number; ++i)
    {
        float blendshape_weight = blendshape_weights[i / 4][i % 4];
        int blendshape_vertex_index = blendshape_vertex_base + gl_VertexID - gl_BaseVertex + i * vertex_count_per_blendshape;
        out_position += blendshape_weight * vec4(blendshape_vertices[blendshape_vertex_index].position_x, blendshape_vertices[blendshape_vertex_index].position_y, blendshape_vertices[blendshape_vertex_index].position_z, 0);
    }
    gl_Position = projection * view * model * out_position;
}