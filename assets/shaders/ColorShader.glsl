#vertexShader
#version 450 core
		
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;

void main()
{
	v_color = color;
 	gl_Position = vec4(position.x, position.y, position.z, 1.0);	 
}

#fragmentShader
#version 450 core
layout(location = 0) out vec4 color;

layout(location = 0) in vec3 v_color;

void main(){
	color = vec4(v_color.x, v_color.y, v_color.z,1.0);
}
