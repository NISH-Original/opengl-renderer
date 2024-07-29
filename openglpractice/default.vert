#version 330 core
layout (location = 0) in vec3 aPos; // position
layout (location = 1) in vec3 aCol; // color
layout (location = 2) in vec2 aTex; // texture
layout (location = 3) in vec3 aNor; // normals

// outputs the color of the fragment shader
out vec3 color;

out vec2 texCoord;

out vec3 normal;
out vec3 currPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	currPos = vec3(model * vec4(aPos, 1.0f));
	
	gl_Position = camMatrix * vec4(currPos, 1.0f);
	color = aCol;
	texCoord = aTex;
	normal = aNor;
}

