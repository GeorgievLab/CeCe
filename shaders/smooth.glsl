[VertexShader]
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120
uniform sampler2D data;
uniform ivec2 size = ivec2(16, 16);

vec4 get_pixel(vec2 pos) 
{
    return texture2D(data, pos);
}

float triangular(float f) 
{
    f = f * 0.5;
    if (f < 0.0)
        return (f + 1.0);
    else
        return (1.0 - f);
    return 0.0;
}

vec4 interpolate_value(vec2 pos) 
{
    vec2 texelSize = 1.0 / size;
    vec4 sum = vec4(0.0);
    vec4 denom = vec4(0.0);
    vec2 ab = fract(pos * size);
    
    for (int m = -1; m <= 2; m++) {
        for (int n = -1; n <= 2; n++) {
            vec2 mn = vec2(m, n);
            vec4 pix = get_pixel(pos + vec2(texelSize * mn));
            vec2 f = mn - ab;
            float c = triangular(f.x) * triangular(f.y);
            sum += pix * c;
            denom += c;
        }
    }

    return (sum / denom);
}

void main() 
{
    gl_FragColor = interpolate_value(gl_TexCoord[0].xy);
}
[Parameters]
sampler2D data = load("../../qshaderedit/texture.png");
ivec2 size = ivec2(16, 16);
