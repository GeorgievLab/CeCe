[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform vec2 membraneCenter = vec2(0.5, 0.5);
uniform vec2 nucleusCenter = vec2(0.55, 0.43);
uniform float membraneSize = 0.48;
uniform float nucleusSize = 0.2;
uniform vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);

const vec4 membraneColor = vec4(0.5, 0.5, 0.5, 0.7);
const vec4 nucleusColor = vec4(0.5, 0.5, 0.5, 0.5);
const vec4 backgroundColor = vec4(0, 0, 0, 0);

/**
 * @brief Return circle radius offset based on angle.
 *
 * @param angle Angle in radians.
 *
 * @return Radius offset.
 */
float get_offset(float angle) 
{
	return 
		0.3 * sin(angle * 2) + 
		0.2 * cos(angle * 6) +
		0.5 * sin(angle * 3)
	;
}

float get_offset(vec2 pos) 
{
	return sin(pos.x);
}

/**
 * @brief Draw circle.
 */
vec4 draw_circle(vec4 curColor, vec4 color, vec2 center, float size, float offsetCoeff, float angleCoeff, float base) 
{
	vec2 diff = gl_TexCoord[0].xy - center;
	float dist = distance(gl_TexCoord[0].xy, center);
	float angle = atan(diff.y, diff.x);
	float offset = offsetCoeff * get_offset(angle * angleCoeff);
	float sizeMax = size + offset;
	
	if (dist <= sizeMax)
	{
		float alpha = smoothstep(sizeMax - 0.08, sizeMax, dist);
		curColor = mix(curColor, color, base + alpha * alpha);
			
		curColor = mix(curColor, g_color, 0.5);
	}
	else
	{
		color = mix(color, g_color, 0.5);
		float alpha = 1 - smoothstep(sizeMax, sizeMax + 0.01, dist);
		curColor = mix(curColor, color * (1.0 + base), alpha);
	}
	
	return curColor;
}

/**
 * @brief Main function.
 */
void main() 
{
	vec4 color = backgroundColor;

	// Membrane	
	color = draw_circle(color, membraneColor, membraneCenter, membraneSize, 0.008, 2, 0.5);
	
	// Nucleus	
	color = draw_circle(color, nucleusColor, nucleusCenter, nucleusSize, 0.01, 1, 0.2);

	gl_FragColor = color;
}
[Parameters]
vec2 membraneCenter = vec2(0.5, 0.5);
vec2 nucleusCenter = vec2(0.55, 0.43);
float membraneSize = 0.48;
float nucleusSize = 0.2;
vec4 g_color = vec4(0, 0, 1, 1);
