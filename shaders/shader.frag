#version 450
#extension GL_EXT_scalar_block_layout : enable

struct AlignedInt {
    int i;
};

layout(binding = 0) uniform UniformBufferObject {
    AlignedInt tileMap[24*24];
    mat4 model;
    mat4 view;
    mat4 proj;
    int textureAtlasWidth;
    int textureAtlasHeight;
    int tileWidth;
    int tileHeight;
    int tileMapWidth;
    int tileMapHeight;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec2 tileSize = vec2(ubo.tileMapWidth, ubo.tileMapHeight);
    vec2 chunkSize = vec2(32.0, 32.0);

    vec2 tiledCoord = mod(fragTexCoord * tileSize, 1.0);

    vec2 chunkCoord = tiledCoord * chunkSize;

    int tileX = int(fragTexCoord.x * float(ubo.tileMapWidth));
    int tileY = int(fragTexCoord.y * float(ubo.tileMapHeight));

    int tileID = ubo.tileMap[tileX * ubo.tileMapWidth + tileY].i;

    chunkCoord.x = chunkCoord.x + (tileID * ubo.tileWidth);

    outColor = texture(texSampler, chunkCoord / textureSize(texSampler, 0));
}
