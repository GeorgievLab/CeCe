[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform sampler2D data;
uniform ivec2 size = ivec2(16, 16);
uniform bool interpolate = false;

const int COUNT = 9;
const vec4 COLORS[COUNT] = vec4[COUNT](
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
const float STEP = (1.0 / (COUNT - 1));

float get_value(vec2 pos)
{
	return texture2D(data, pos).r;
}

float triangular(float f) {
	f = f * 0.5;
	if (f < 0.0)
		return (f + 1.0);
	else
		return (1.0 - f);
	return 0.0;
}

float interpolate_value(vec2 pos)
{
	vec2 texelSize = 1.0 / size;
	float sum = 0.0;
	float denom = 0.0;
	vec2 ab = fract(pos * size);
    
	for (int m = -1; m <=2; m++)
	{
		for (int n = -1; n <= 2; n++)
		{
			vec2 mn = vec2(m, n);
			float vecData = get_value(pos + vec2(texelSize * mn));
			vec2 f = mn - ab;
			float c = triangular(f.x) * triangular(f.y);
			sum += vecData * c;
			denom += c;
        }
    }
    return (sum / denom);
}

void main() {

	// Calculate position
	float value;	

	if (interpolate)
		value = interpolate_value(gl_TexCoord[0].xy);
	else
		value = get_value(gl_TexCoord[0].xy);

	//if (gl_TexCoord[0].y > 0.5)
	//{
	//	gl_FragColor = vec4(value);
	//}
	//else
	{
		int ix = int(value * (COUNT - 1));
	 	vec4 thermal = mix(COLORS[ix], COLORS[ix + 1], (value - (ix * STEP)) / STEP);
 		gl_FragColor = thermal;
 	}

	//gl_FragColor = vec4(vec3(value, 0, 0), 1.0);
}
[Parameters]
sampler2D data = load("../../qshaderedit/perlin.png");
ivec2 size = ivec2(16, 16);
bool interpolate = false;
