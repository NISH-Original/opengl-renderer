#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

// gets texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
	// shoot light in all directions
	// and decrease the intensity as distance increases
	// with the help of quadratic formula
	vec3 lightVec = lightPos - currPos;
	float dist = length(lightVec);
	float a = 0.05f;
	float b = 0.01f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
	
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 nor = normalize(normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(nor, lightDir), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDir, nor);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 directLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 nor = normalize(normal);
	vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(nor, lightDir), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDir, nor);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 nor = normalize(normal);
	vec3 lightDir = normalize(lightPos - currPos);
	float diffuse = max(dot(nor, lightDir), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDir, nor);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specularAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
	FragColor = directLight();
}