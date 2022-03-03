#vertexShader
#version 450 core
		
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;

// layout(std140, binding = 0) buffer Camera
// {
	// mat4 matrix;
// } camera;

layout(std430, binding = 0) buffer Name {  //ssbo

	mat4 transform;
	vec4 color;

} Cam;


// layout(std430, binding = 2) buffer MyStruct2 { 

// 	mat4 transform;
// 	vec2 pos;
// 	vec2 sth;

// } ;


layout(std140, binding = 0) uniform Camera //ubo
{
	mat4 u_ViewProjection;
} some_name;

layout(push_constant) uniform ExamplePush
{
	mat4 matrix;
}some_name_2;

// layout(std430, binding = 2) buffer test{
	// sampler2D myTexture;
// }testI;
	

void main()
{
	v_color = color;
 	gl_Position = vec4(position.x, position.y, position.z, 1.0);	 
	//test bla blasds
}

#fragmentShader
#version 450 core
layout(location = 0) out vec4 color;
//test again
//test tracking
//test tracking again ert
//test tracking again
layout(location = 0) in vec3 v_color;

void main(){
	color = vec4(v_color.x, v_color.y, v_color.z,1.0);
}
