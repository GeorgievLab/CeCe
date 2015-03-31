[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

const float data[9] = float[](0.2, 0.3, 0.5, 0.7, 0.1, 0.3, 0.5, 0.8, 1.0);
const ivec2 size = ivec2(3, 3);

float get_value(ivec2 pos)
{
	if (pos.x < 0) pos.x = 0;
	if (pos.y < 0) pos.y = 0;
	if (pos.x >= size.x) pos.x = size.x - 1;
	if (pos.y >= size.y) pos.y = size.y - 1;	
	
	return data[pos.x + pos.y * size.x];
}
/*
float calc_value(ivec2 pos)
{
	ivec2 nn = ivec2(pos.x, pos.y + 1);
  	ivec2 ne = ivec2(pos.x + 1, pos.y + 1);
	ivec2 ee = ivec2(pos.x + 1, pos.y);
  	ivec2 se = ivec2(pos.x + 1, pos.y - 1);
  	ivec2 ss = ivec2(pos.x, pos.y - 1);
  	ivec2 sw = ivec2(pos.x - 1, pos.y - 1);
  	ivec2 ww = ivec2(pos.x - 1, pos.y);
  	ivec2 nw = ivec2(pos.x - 1, pos.y + 1);
 
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
*/
void main() {

	// Calculate position
	ivec2 pos = ivec2(gl_TexCoord[0].xy * size);

	//float value = calc_value(pos);
	float value = get_value(pos);

	gl_FragColor = vec4(vec3(value, 0, 0), 1.0);
}
[Parameters]
float _main_data_0[0][0] = 0.2;
float _main_data_0[0][1] = 0.3;
float _main_data_0[0][2] = 0.5;
float _main_data_0[0][3] = 0.7;
float _main_data_0[0][4] = 0.1;
float _main_data_0[0][5] = 0.3;
float _main_data_0[0][6] = 0.5;
float _main_data_0[0][7] = 0.8;
float _main_data_0[0][8] = 1;
