#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(binding = 1) uniform sampler2D texSamplers[];

layout(push_constant) uniform PushConstants {
    int id;
    int texture_id;
} pushConsts;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSamplers[pushConsts.texture_id], fragTexCoord).xyzw;
    if (outColor.w < 0.7) {
        discard;
    }
}
