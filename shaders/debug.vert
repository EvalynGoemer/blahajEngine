#version 450
#extension GL_EXT_scalar_block_layout : enable

struct uboStruct {
    mat4 model;
    mat4 view;
    mat4 proj;
//     vec4 timeAndResolution;
};

layout(binding = 0) readonly buffer DataBuffer {
    uboStruct ubo[200000];
} ssboData;

layout(push_constant) uniform PushConstants {
    int id;
    int texture_id;
} pushConsts;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = ssboData.ubo[pushConsts.id].proj * ssboData.ubo[pushConsts.id].view * ssboData.ubo[pushConsts.id].model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
