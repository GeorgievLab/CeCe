[VertexShader]

void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
[FragmentShader]
#version 120

uniform sampler2D data;

vec4 colormap(float x);

void main() {
	vec4 pix = texture2D(data, gl_TexCoord[0].xy);
	
	if (pix.a > 0.5)
		gl_FragColor = colormap(pix.r);
	else 
		gl_FragColor = pix;
}

// https://github.com/kbinani/glsl-colormap
// https://github.com/kbinani/glsl-colormap/blob/master/shaders/MATLAB_jet.frag

float colormap_red(float x) {
    if (x < 0.7) {
        return 4.0 * x - 1.5;
    } else {
        return -4.0 * x + 4.5;
    }
}

float colormap_green(float x) {
    if (x < 0.5) {
        return 4.0 * x - 0.5;
    } else {
        return -4.0 * x + 3.5;
    }
}

float colormap_blue(float x) {
    if (x < 0.3) {
       return 4.0 * x + 0.5;
    } else {
       return -4.0 * x + 2.5;
    }
}

vec4 colormap(float x) {
    float r = clamp(colormap_red(x), 0.0, 1.0);
    float g = clamp(colormap_green(x), 0.0, 1.0);
    float b = clamp(colormap_blue(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}
[Parameters]
sampler2D data = load("../../../qshaderedit/noise.png");
