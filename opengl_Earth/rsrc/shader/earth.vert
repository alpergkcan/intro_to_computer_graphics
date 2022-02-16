#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 vp;
uniform mat4 m;
uniform sampler2D TexGrey;
uniform int HeightFactor;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoord;

vec3  calculateNormal(vec4 model, vec4 center);
vec3 calculateNormal2(vec4 model, vec4 center);


void main() {
	TexCoord = tex;


	vec4 center = m * vec4(0, 0, 0, 1);
	vec4 model  = m * vec4(pos.xyz, 1.0);

    Normal = normalize(model.xyz - center.xyz);
	model  = vec4(model.xyz + Normal * HeightFactor * texture(TexGrey, tex).r, model.a);
	Pos = model.xyz;

	Normal = normalize(calculateNormal(model, center));
	
    gl_Position = vp * model;
}




vec3 calculateNormal(vec4 model, vec4 center) {
	float vs = 1.0 / 125.0;
	float hs = 1.0 / 250.0;

	float alpha, beta;
	float radius = 600;
	float pi = 3.1415926559;

	vec2 neighbour1_tex = vec2(tex.x - hs, tex.y);
	alpha = neighbour1_tex.x * 2 * pi;
	beta  = neighbour1_tex.y * pi;
	vec3 neighbour1_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour1_pos = neighbour1_pos + (neighbour1_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour1_tex).r;


	vec2 neighbour2_tex = vec2(tex.x, tex.y + vs);
	alpha = neighbour2_tex.x * 2 * pi;
	beta  = neighbour2_tex.y * pi;
	vec3 neighbour2_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour2_pos = neighbour2_pos + (neighbour2_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour2_tex).r;

	
	vec2 neighbour3_tex = vec2(tex.x + hs, tex.y + vs);
	alpha = neighbour3_tex.x * 2 * pi;
	beta  = neighbour3_tex.y * pi;
	vec3 neighbour3_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour3_pos = neighbour3_pos + (neighbour3_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour3_tex).r;

	
	vec2 neighbour4_tex = vec2(tex.x + hs, tex.y);
	alpha = neighbour4_tex.x * 2 * pi;
	beta  = neighbour4_tex.y * pi;
	vec3 neighbour4_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour4_pos = neighbour4_pos + (neighbour4_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour4_tex).r;

	
	vec2 neighbour5_tex = vec2(tex.x, tex.y - vs);
	alpha = neighbour5_tex.x * 2 * pi;
	beta  = neighbour5_tex.y * pi;
	vec3 neighbour5_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour5_pos = neighbour5_pos + (neighbour5_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour5_tex).r;


	vec2 neighbour6_tex = vec2(tex.x - hs, tex.y - vs);
	alpha = neighbour6_tex.x * 2 * pi;
	beta  = neighbour6_tex.y * pi;
	vec3 neighbour6_pos = (m * vec4(radius * vec3(sin(beta) * cos(alpha), sin(beta) * sin(alpha), cos(beta)), 1)).xyz;
	neighbour6_pos = neighbour6_pos + (neighbour6_pos - center.xyz) * HeightFactor * texture(TexGrey, neighbour6_tex).r;

	Pos = model.xyz;
	vec3 tr1_normal = normalize(cross(neighbour1_pos - Pos, neighbour2_pos - Pos));
	vec3 tr2_normal = normalize(cross(neighbour2_pos - Pos, neighbour3_pos - Pos));
	vec3 tr3_normal = normalize(cross(neighbour3_pos - Pos, neighbour4_pos - Pos));
	vec3 tr4_normal = normalize(cross(neighbour4_pos - Pos, neighbour5_pos - Pos));			
	vec3 tr5_normal = normalize(cross(neighbour5_pos - Pos, neighbour6_pos - Pos));			
	vec3 tr6_normal = normalize(cross(neighbour6_pos - Pos, neighbour1_pos - Pos));

	return (tr1_normal + tr2_normal + tr3_normal + tr4_normal + tr5_normal + tr6_normal) / 6.0;
}
