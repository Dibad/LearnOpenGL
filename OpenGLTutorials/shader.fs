#version 330

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), texture(tex2, TexCoord).a * 0.2f);
}