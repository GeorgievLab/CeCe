[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform bool g_hasBud = true;
uniform vec2 g_centerMain = vec2(0.5, 0.75);
uniform float g_sizeMain = 0.49;
uniform float g_sizeBud = 0.29;

const vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);
const vec4 g_borderColor = vec4(0, 0, 0, 1);
const vec4 g_backgroundColor = vec4(1, 0, 0, 0);

/**
 * @brief Main function.
 */
void main() 
{
	vec2 centerBud = g_centerMain + vec2(0, (g_sizeBud + g_sizeMain) * 0.5);

	vec2 diffMain = gl_TexCoord[0].xy - g_centerMain;
	vec2 diffBud = gl_TexCoord[0].xy - centerBud;

	float a = 0.70;
	
	// Main ellipse
	vec2 ellipseMain = pow(diffMain, vec2(2)) / pow(vec2(a, a / 2), vec2(2));
   	float distanceMain = ellipseMain.x + ellipseMain.y;
   	
	vec2 ellipseBud = pow(diffBud, vec2(2)) / pow(vec2(a, a / 2), vec2(2));
   	float distanceBud = ellipseBud.x + ellipseBud.y;

	if (distanceMain <= g_sizeMain)
	{
		if (distanceMain <= g_sizeMain - 0.01)
			gl_FragColor = g_color;
		else
			gl_FragColor = g_borderColor;
	}
	else if (g_hasBud && distanceBud <= g_sizeBud)
	{
		if (distanceBud <= g_sizeBud - 0.01)
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
vec2 g_centerMain = vec2(0.5, 0.25);
bool g_hasBud = true;
float g_sizeBud = 0.1;
float g_sizeMain = 0.49;
