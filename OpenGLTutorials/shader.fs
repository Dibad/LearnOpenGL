#version 330

out vec4 FragColor;
in vec3 vertexPosition;

void main()
{
	FragColor = vec4(vertexPosition, 1.0f);
}