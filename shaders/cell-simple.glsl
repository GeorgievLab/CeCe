[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform vec2 g_center = vec2(0.5, 0.5);
uniform float g_size = 0.49;

const vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);
const vec4 g_borderColor = vec4(0, 0, 0, 1);
const vec4 g_backgroundColor = vec4(0, 0, 0, 0);

/**
 * @brief Main function.
 */
void main() 
{
	vec4 color = g_backgroundColor;

	vec2 diff = gl_TexCoord[0].xy - g_center;
	float dist = distance(gl_TexCoord[0].xy, g_center);

	if (dist <= g_size)
	{
		if (dist <= g_size - 0.01)
			gl_FragColor = g_color;
		else
			gl_FragColor = g_borderColor;
	}
	else
	{
		gl_FragColor = g_backgroundColor;
	}
}
[Parameters]
vec2 g_center = vec2(0.5, 0.5);
float g_size = 0.49;
