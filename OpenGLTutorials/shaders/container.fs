#version 330

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	// Calculate ambient light
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Calculate diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	// Calculate specular light
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor =  vec4(result, 1.0f);
}