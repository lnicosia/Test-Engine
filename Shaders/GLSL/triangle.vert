#version 450

layout(set = 0, binding = 0, row_major) uniform SceneData
{
    mat4 view;
    mat4 projection;
} sceneData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(push_constant, row_major) uniform constants
{
    mat4 model;
} pushConstants;

void main()
{
    gl_PointSize = 10.0;
    gl_Position = sceneData.projection * sceneData.view * pushConstants.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}