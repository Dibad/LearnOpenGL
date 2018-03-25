#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0f / 300.0f;

void main()
{
	vec3 color = texture(screenTexture, TexCoords).rgb;
	
	//// Inverted
	//FragColor = vec4(1.0f - color, 1.0f);

	//// Grayscales
	//float average = (color.r + color.g + color.b) / 3.0f;						// Averaged
	//float average = 0.2126f * color.r + 0.7152f * color.g + 0.072f * color.b;	// Sensitive
	//FragColor = vec4(average, average, average, 1.0f);

	//// Kernels
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
    );

	vec3 sampleTex[9];
	for(itn i = 0; i < 9; ++i)
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));

	color = vec3(0.0f);

	for(int i = 0; i < 9; ++i)
		color +=sampleTex[i] * kernel[i];

	FragColor = vec4(color, 1.0f);	
}