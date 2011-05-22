uniform sampler2D colorMap;
varying vec2 vVaryingTexCoords;

void main(void) {
  // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  gl_FragColor = texture2D(colorMap,  vVaryingTexCoords.st);
    
}