#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

// outputs the color of the fragment shader
out vec3 color;

out vec2 texCoord;

uniform float scale;

// inputs the matrices reqd for 3D view with perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	color = aCol;
	texCoord = aTex;
}

