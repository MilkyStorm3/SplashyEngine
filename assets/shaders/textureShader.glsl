#vertexShader
#version 450 core
layout(location=0) in vec3 position;
layout(location=1) in vec2 tex_coord;
layout(location=2) in float tex_id;

layout(location=0) out vec2 v_texCoord;
layout(location=1) out float v_texId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_projection;
};

void main(){
    v_texCoord = tex_coord;
    v_texId = tex_id;
    gl_Position = u_projection * vec4(position, 1.0);
}
 

#fragmentShader
#version 450 core

layout(location = 0) out vec4 color;

layout(location=0) in vec2 v_texCoord;
layout(location=1) in float v_texId;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main(){
    int index = int(v_texId);
    color = texture(u_Textures[index],v_texCoord);

}