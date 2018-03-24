#version 330 core
out vec4 FragColor;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{    
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

    FragColor = vec4(result, 1.0f);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0f);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	// Combine
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_diffuse1, TexCoords));

	return (ambient + diffuse + specular);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_diffuse1, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);	
}