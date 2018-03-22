#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outercutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// Calculate ambient light
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// Calculate diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// Calculate specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// Spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outercutOff;
	float intensity = clamp((theta - light.outercutOff) / epsilon, 0.0, 1.0f);

	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// Result
	FragColor =  vec4(ambient + diffuse + specular, 1.0f);

}