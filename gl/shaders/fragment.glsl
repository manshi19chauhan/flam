
precision mediump float;
varying vec2 vTexCoord;
uniform sampler2D uTexture;
uniform int uEffectMode;

void main() {
    vec4 color = texture2D(uTexture, vTexCoord);
    
    if (uEffectMode == 1) {
        // Grayscale effect
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        gl_FragColor = vec4(gray, gray, gray, color.a);
    } else if (uEffectMode == 2) {
        // Invert effect
        gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
    } else if (uEffectMode == 3) {
        // Edge detection approximation
        // This is a simplified version - real edge detection would need convolution
        float edge = length(color.rgb - vec3(0.5));
        gl_FragColor = vec4(vec3(edge), color.a);
    } else {
        // Normal
        gl_FragColor = color;
    }
}