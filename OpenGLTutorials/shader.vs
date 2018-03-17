// EXERCISE 2: Specify an horizontal offset via a uniform and move the triangle
// to the right side of the screen
// EXTRA: Add an extra vertical offset via a uniform and move the triangle
// in a circle

#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float xOffset;
uniform float yOffset;

out vec3 ourColor;

void main()
{	
	gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f);
	ourColor = aColor;
}