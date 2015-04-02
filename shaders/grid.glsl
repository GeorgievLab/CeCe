[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform sampler2D data;
uniform bool interpolate = false;

float get_value(vec2 pos)
{
	return texture2D(data, pos).r;
}

float calc_value(vec2 pos)
{
	vec2 nn = vec2(pos.x, pos.y + 1);
  	vec2 ne = vec2(pos.x + 1, pos.y + 1);
	vec2 ee = vec2(pos.x + 1, pos.y);
  	vec2 se = vec2(pos.x + 1, pos.y - 1);
  	vec2 ss = vec2(pos.x, pos.y - 1);
  	vec2 sw = vec2(pos.x - 1, pos.y - 1);
  	vec2 ww = vec2(pos.x - 1, pos.y);
  	vec2 nw = vec2(pos.x - 1, pos.y + 1);
 
  	float sum = 0.;
  	sum += 0.125 * get_value(nn);
  	//sum += get_value(ne);
  	sum += 0.125 * get_value(ee);
  	//sum += get_value(se);
  	sum += 0.125 * get_value(ss);
  	//sum += get_value(sw);
  	sum += 0.125 * get_value(ww);
  	//sum += get_value(nw);
  	sum += 0.5 * get_value(pos);
 
  	return sum;
}

void main() {

	// Calculate position
	float value;	

	if (interpolate)
		value = calc_value(gl_TexCoord[0].xy);
	else
		value = get_value(gl_TexCoord[0].xy);

	gl_FragColor = vec4(vec3(value, 0, 0), 1.0);
}
[Parameters]
sampler2D data = load("../../qshaderedit/texture.png");
bool interpolate = false;
