#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;

// Output data ; will be interpolated for each fragment.
out vec2 texcoord;

void main(){
	gl_Position =  vec4(pos,1);
	texcoord = (pos.xy+vec2(1,1))/2.0;
}

