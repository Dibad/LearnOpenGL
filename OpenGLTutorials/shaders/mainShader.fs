#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D wallTexture;
uniform sampler2D wallTexture_normal;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;

uniform bool show_normal;


vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;
	// Attenuation
	float max_distance = 1.5f;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0f / distance;

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

void main()
{
	vec3 color = texture(wallTexture, fs_in.TexCoords).rgb;
	vec3 lighting = vec3(0.0f);
	vec3 normal;

	if(show_normal)
	{
		normal = texture(wallTexture_normal, fs_in.TexCoords).rgb;
		normal = normalize(normal * 2.0f - 1.0f);
	}
	else
	{
		normal = normalize(fs_in.Normal);
	}


	for(int i = 0; i < 4; ++i)
		lighting += BlinnPhong(normal, fs_in.FragPos, lightPositions[i], lightColors[i]);
	
	color *= lighting;

	FragColor = vec4(color, 1.0f);
}