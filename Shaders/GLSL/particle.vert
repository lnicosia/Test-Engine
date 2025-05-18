#version 450

layout(set = 0, binding = 0, row_major) uniform SceneData
{
    mat4 view;
    mat4 projection;
	mat4 viewProj;
} sceneData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main()
{
	gl_PointSize = 5.0;
	gl_Position = sceneData.projection * sceneData.view * vec4(inPosition, 1.0);
	//gl_Position = vec4(inPosition.xy, 0.8, 1.0);
	outColor = inColor;
}