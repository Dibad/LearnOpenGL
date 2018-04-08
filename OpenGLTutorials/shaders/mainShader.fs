#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	vec3 Normal;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool show_normal;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// Ambient

	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal,halfwayDir), 0.0f), 32.0f);
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
	vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 lighting = vec3(0.0f);
	vec3 normal;

	vec3 lightColor = vec3(1.0f);

	if(show_normal)
	{
		// obtain normal from normal map in range [0,1]
		normal = texture(normalMap, fs_in.TexCoords).rgb;
		// transform normal vector to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
		lighting += BlinnPhong(normal, fs_in.TangentFragPos, fs_in.TangentLightPos, lightColor);
	}
	else
	{
		normal = normalize(fs_in.Normal);
		lighting += BlinnPhong(normal, fs_in.FragPos, lightPos, lightColor);
	}

	color *= lighting;

	FragColor = vec4(color, 1.0f);

}