#version 450
#extension GL_EXT_nonuniform_qualifier : enable
#define MAX_TEXTURE_COUNT

layout(binding = 1) uniform sampler texSampler;
layout(binding = 2) uniform texture2D textures[MAX_TEXTURE_COUNT];
layout(push_constant) uniform constants
{
    uint textureIndex;
} pushConstants;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(sampler2D(textures[0], texSampler), fragTexCoord);
}