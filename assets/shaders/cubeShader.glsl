#vertexShader
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_cameraMatrix;
};

layout(std140, binding = 1) uniform ModelData
{
	mat4 u_transformation;
};

void main(){
    gl_Position = u_cameraMatrix * u_transformation * vec4(position, 1.0);
    v_color = color;
}
 

#fragmentShader
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 0) in vec3 v_color;

void main(){
    color = vec4(v_color, 1.0);
}