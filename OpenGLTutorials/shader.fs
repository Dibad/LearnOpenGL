// EXERCISE 1.1: Make sure only the happy face looks in the other/reverse direction by changing the fragment shader

#version 330

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = mix(texture(tex1, TexCoord), texture(tex2, vec2(1.0f - TexCoord.x, TexCoord.y)), texture(tex2, vec2(1.0f - TexCoord.x, TexCoord.y)).a * 0.2f);
}