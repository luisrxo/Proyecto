#version 330 core

in vec3 our_uv;
out vec4 color;

uniform samplerCube skybox;
uniform float lowerlimit = 0.0;
uniform float upperlimit=0.15;
uniform vec3 fogColor;

void main()
{
    color = texture(skybox, our_uv);
	float factor = (our_uv.y - lowerlimit) / (upperlimit-lowerlimit);
	color = mix(vec4(fogColor,1.0),color, factor);
}
