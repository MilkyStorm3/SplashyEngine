#vertexShader
#version 450 core
		
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;


void main()
{
 	gl_Position = vec4(position, 0.0);
}

#fragmentShader
#version 450 core
out vec4 color;

void main()
{
	color = vec4(0.8, 0.2,0.2,1.0);
}
