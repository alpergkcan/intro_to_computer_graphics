#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 vp;
uniform mat4 m;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
	TexCoord = tex;

	vec4 center = m * vec4(0, 0, 0, 1);
	vec4 model  = m * vec4(pos.xyz, 1.0);

    Normal = normalize(model.xyz - center.xyz);
	Pos = model.xyz;
	
    gl_Position = vp * model;
}
