#version 330 core
out vec4 FragColor;

// inputs the color from the vertex shader
in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currPos;

// gets texture Unit from the main function
uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;

void main()
{
	vec3 nor = normalize(normal);
	vec3 lightDir = normalize(lightPos - currPos);

	float diffuse = max(dot(normal, lightDir), 0.0f);

	FragColor = texture(tex0, texCoord) * lightColor * diffuse;
}