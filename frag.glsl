#version 130

#define TWO_PI   6.283185307179586
#define THIRD_PI 1.0471975511965976

#define rgb(R, G, B) vec4(R, G, B, 1.0)

// Radius of colour picker
#define R0 130

// Radius of hue wheel
#define R1 180

uniform float hue;  // Hue (in radians)
uniform vec2 windowSize;

// A note on the colour space used:
// Hue values (hue) are in radians.
// Saturation (sat) is in the range (-1, +1); 0 saturation generates 
// a grey value, +1 saturation a bright colour and -1 a bright but
// inverted colour.
// Lightness (lux) is also in the range (-1, +1); -1 yields black,
// +1 yields white, and values near 0 yield midtones.
// Note that inverting the saturation is the same as rotating the 
// hue by 180deg.

// Given a hue value (in radians), generate the corresponding RGB value.
vec4 colorHue(float hue) {

	hue = mod(hue, TWO_PI);

	// Calculate fractional components
	float p = fract(hue / THIRD_PI);
	float q = 1 - p;

	// Calculate hexant
	int i = int(hue / THIRD_PI);

	switch (i) {
	case 0: return rgb(1, p, 0);
	case 1: return rgb(q, 1, 0);
	case 2: return rgb(0, 1, p);
	case 3: return rgb(0, q, 1);
	case 4: return rgb(p, 0, 1);
	case 5: return rgb(1, 0, q);
	}
}

// Given a hue value (in radians) and saturation, generate the corresponding RGB value.
vec4 colorHS(float hue, float sat) {
  vec4 color = colorHue(hue);
	return 0.5 * (1 - sat) + sat * color;
}

// Given a hue value (in radians), saturation, and luminosity, generate the corresponding RGB value.
vec4 colorHSL(float hue, float sat, float lux) {
  vec4 color = colorHS(hue, sat);
	return (lux < 0) ? (1 + lux) * color : lux + (1 - lux) * color;
}

float angle(vec2 v) { return atan(v.y, v.x); }

void main() {

	vec2 coord = vec2(gl_FragCoord.x, gl_FragCoord.y);
	vec2 centre = 0.5 * windowSize;
	vec2 offset = coord - centre;

	float r = length(offset);
	float theta = angle(offset);

	if (r < R0) {
		vec2 ray = offset / R0;
		float sat = ray.x / sqrt(1 - ray.y * ray.y);
		float lux = abs(ray.y) * ray.y;
		gl_FragColor = colorHSL(hue, sat, lux);
	}
	else if (r < R1) {
		gl_FragColor = colorHue(hue + theta);
	}
	else {
		gl_FragColor = rgb(0, 0, 0);
	}
}
