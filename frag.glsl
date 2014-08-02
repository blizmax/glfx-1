uniform float t;
uniform vec2 windowSize;

float rings(vec2 point, vec2 origin) {
	return (mod(length(point - origin) - 20.0 * t, 40) < 15.0) ? 0.0 : 1.0;
}

void main() {
	vec2 point = vec2(gl_FragCoord.x, gl_FragCoord.y);
	vec2 centre = 0.5 * windowSize;
	gl_FragColor.r = rings(point, centre - vec2(80.0, 0.0));
	gl_FragColor.g = rings(point, centre);
	gl_FragColor.b = rings(point, centre + vec2(80.0, 0.0));
}
