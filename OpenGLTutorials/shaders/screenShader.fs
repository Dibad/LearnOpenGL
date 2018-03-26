#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0f / 300.0f;

void main()
{
	vec3 color = texture(screenTexture, TexCoords).rgb;

	FragColor = vec4(color, 1.0f);	
}