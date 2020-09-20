#version 330 core

uniform sampler2D tex;

in vec2 texcoord;

void main(){
	gl_FragColor = vec4(vec3(texture2D(tex, texcoord).r), 1.0);
}