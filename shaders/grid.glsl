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

const vec4 colors[9] = vec4[9](
 	vec4(0.208, 0.165, 0.529, 1.0), // '#352a87'
	vec4(0.059, 0.361, 0.867, 1.0), // '#0f5cdd'
	vec4(0.078, 0.506, 0.839, 1.0), // '#1481d6'
	vec4(0.024, 0.643, 0.792, 1.0), // '#06a4ca'
 	vec4(0.180, 0.718, 0.643, 1.0), // '#2eb7a4'
 	vec4(0.529, 0.749, 0.467, 1.0), // '#87bf77'
 	vec4(0.820, 0.733, 0.349, 1.0), // '#d1bb59'
 	vec4(0.996, 0.784, 0.196, 1.0), // '#fec832'
 	vec4(0.976, 0.984, 0.055, 1.0)  // '#f9fb0e'
);
	
void main() {

	// Calculate position
	float value;	

	if (interpolate)
		value = calc_value(gl_TexCoord[0].xy);
	else
		value = get_value(gl_TexCoord[0].xy);

	int ix = int(value * 8);
 	vec4 thermal = mix(colors[ix], colors[ix + 1], (value - float(ix) * (1.0 / 9.0)) / (1.0 / 9.0));
 	gl_FragColor = thermal;

	//gl_FragColor = vec4(vec3(value, 0, 0), 1.0);
}
[Parameters]
sampler2D data = load("../../qshaderedit/texture.png");
bool interpolate = false;
