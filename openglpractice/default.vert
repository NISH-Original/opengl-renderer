#version 330 core
layout (location = 0) in vec3 aPos; // position
layout (location = 1) in vec3 aNor; // normals
layout (location = 2) in vec3 aCol; // color
layout (location = 3) in vec2 aTex; // texture

out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	currPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
	
	normal = aNor;
	color = aCol;
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	gl_Position = camMatrix * vec4(currPos, 1.0);
}

