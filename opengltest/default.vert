#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main(){
	fragPos = vec3(modelMatrix * vec4(aPos,1.0f));
	gl_Position = camMatrix * vec4(fragPos, 1.0f);
	color = aColor;
	texCoord = aTex;
	normal = aNormal;
}