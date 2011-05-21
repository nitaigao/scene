
varying vec3 vVaryingTextureCoord;

uniform samplerCube cubeMap;

void main(void) {
  gl_FragColor = textureCube(cubeMap, vVaryingTextureCoord);
}

