[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0 * vec4(1, 2, 1, 1);
}
[FragmentShader]
#version 120

uniform bool g_hasBud = true;
uniform vec2 g_centerMain = vec2(0.5, 0.5);
uniform float g_sizeMain = 0.45;
uniform float g_sizeBud = 0.25;
uniform vec4 g_color = vec4(0.5, 0.5, 0.5, 0.5);
uniform vec4 g_idColor = vec4(0, 0, 0, 0);

const vec4 g_backgroundColor = vec4(0, 0, 0, 0);
const vec4 g_membraneColor = vec4(0.5, 0.5, 0.5, 0.7);
//const vec4 nucleusColor = vec4(0.5, 0.5, 0.5, 0.5);

const float SMOOTH = 0.25;

/**
 * @brief Draw circle.
 */
vec4 draw_circle(vec4 curColor, vec4 color, vec2 center, float size, vec2 center2, float size2, float base) 
{
	float dist = distance(gl_TexCoord[0].xy, center);
	float dist2 = distance(gl_TexCoord[0].xy, center2);
	
	if (dist <= size)
	{
		if (g_hasBud && dist2 < size2)
		{
			float alpha2 = smoothstep(size2 - SMOOTH * size2, size2, dist2);
			curColor = mix(curColor, color, base + alpha2 * alpha2);
			
			float alpha = smoothstep(size - SMOOTH * size, size, dist);
			curColor = mix(g_backgroundColor, curColor, base + alpha * alpha);
		}
		else
		{
			float alpha = smoothstep(size - SMOOTH * size, size, dist);
			curColor = mix(curColor, color, base + alpha * alpha);
		}
		
		curColor = mix(curColor, g_color, 0.5);
	}
	
	return curColor;
}

/**
 * @brief Main function.
 */
void main() 
{
	vec2 centerBud = g_centerMain + vec2(0, (g_sizeBud + g_sizeMain) - 0.1);

	// Lengths
   	float sizeMain = distance(gl_TexCoord[0].xy, g_centerMain);
   	float sizeBud = distance(gl_TexCoord[0].xy, centerBud);
   	float d = distance(g_centerMain, centerBud);
   	float yJoin = (d * d - sizeMain * sizeMain + sizeBud * sizeBud) / (2 * d * d);
   	
   	bool inMain = sizeMain <= g_sizeMain;
   	bool inBud = g_hasBud && sizeBud <= g_sizeBud;
   	//bool inJoin = gl_TexCoord[0].y > yJoin - 0.03 && gl_TexCoord[0].y < yJoin + 0.03;

	vec4 color = g_backgroundColor;

	color = draw_circle(color, g_membraneColor, g_centerMain, g_sizeMain, centerBud, g_sizeBud, 0.1);
	//color = draw_circle(color, g_membraneColor, g_centerMain + vec2(-0.1), g_sizeMain * 0.3, centerBud, g_sizeBud * 0.5, 0.1);
	
	if (g_hasBud)
	{
		color = draw_circle(color, g_membraneColor, centerBud, g_sizeBud, g_centerMain, g_sizeMain, 0.1);
		//color = draw_circle(color, g_membraneColor, centerBud + vec2(0.05), g_sizeBud * 0.3, g_centerMain, g_sizeMain * 0.5, 0.1);
	}
	
	gl_FragColor = color;
}
[Parameters]
bool g_hasBud = true;
vec2 g_centerMain = vec2(0.5, 0.5);
float g_sizeMain = 0.5;
float g_sizeBud = 0.25;
vec4 g_color = vec4(0, 0, 0, 1);
vec4 g_idColor = vec4(0, 1, 0, 0.5);
