// EXERCISE 3: Use a uniform variable as the mix function's third
// parameter to vary the amount the two textures are visible.
// Use up an ddown arrow keys to change how much of each is visible

#version 330

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float alphaValue;

void main()
{
	FragColor = mix(texture(tex1, TexCoord).rgba, texture(tex2, TexCoord).rgba, alphaValue);
}