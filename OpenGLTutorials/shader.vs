// EXERCISE 3: Ouptut the vertex position to the fragment shader using
// the out keyword and set the fragment's color equal to this
// vertex position.
// Answer this: Why is the bottom-left side of our triangle black?

#version 330

layout (location = 0) in vec3 aPos;

out vec3 vertexPosition;

void main()
{	
	gl_Position = vec4(aPos, 1.0f);
	vertexPosition = aPos;
}


// Answer, because the blue vertex does have negative coordinates, so
// values are clamped to 0.0f, which is black