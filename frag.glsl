#define TWO_PI   6.283185307179586
#define THIRD_PI 1.0471975511965976

// Radius of colour picker
#define R0 160.0

// Radius of hue wheel
#define R1 200.0

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

// Modify the saturation of the given color.
vec4 adjustSat(vec4 color, float sat) {
	return 0.5 * (1.0 - sat) + sat * color;
}

// Modify the saturation and luminosity of the given color.
// Note: adjustSatLux(color, sat, 0.0) == adjustSat(color, sat);
vec4 adjustSatLux(vec4 color, float sat, float lux) {
	return (lux < 0.0) ? (1.0 + lux) * adjustSat(color, sat) : lux + (1.0 - lux) * adjustSat(color, sat);
}

// Given a hue value (in radians), generate the corresponding RGB value.
vec4 colorHue(float hue) {

	hue = mod(hue, TWO_PI);

	// Calculate fractional components
	float p = fract(hue / THIRD_PI);
	float q = 1.0 - p;

	// Construct RGB component arrays
	float compR[6], compG[6], compB[6];
	compR[0] = compG[2] = compB[4] = 1.0;
	compR[1] = compG[3] = compB[5] = q;
	compR[2] = compG[4] = compB[0] = 0.0;
	compR[3] = compG[5] = compB[1] = 0.0;
	compR[4] = compG[0] = compB[2] = p;
	compR[5] = compG[1] = compB[3] = 1.0;

	// Calculate hexant
	int i = int(hue / THIRD_PI);

	return vec4(compR[i], compG[i], compB[i], 1.0);
}

// Given a hue value (in radians), saturation, and luminosity, generate the corresponding RGB value.
vec4 colorHSL(float hue, float sat, float lux) {
	return adjustSatLux(colorHue(hue), sat, lux);
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
		float sat = ray.x / sqrt(1.0 - ray.y * ray.y);
		float lux = abs(ray.y) * ray.y;
		gl_FragColor = colorHSL(hue, sat, lux);
	}
	else if (r < R1) {
		gl_FragColor = colorHue(hue + theta);
	}
	else {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
