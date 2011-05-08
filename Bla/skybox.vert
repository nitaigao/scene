attribute vec4 vVertex;
uniform mat4 mvpMatrix;

varying vec3 vVaryingTextureCoord;

void main(void) {  
  vVaryingTextureCoord = normalize(vVertex.xyz);
  gl_Position = mvpMatrix * vVertex;
}
