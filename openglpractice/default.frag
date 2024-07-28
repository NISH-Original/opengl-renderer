#version 330 core
out vec4 FragColor;

// inputs the color from the vertex shader
in vec3 color;

in vec2 texCoord;

// gets texture Unit from the main function
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}