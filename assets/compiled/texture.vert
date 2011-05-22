varying vec2 vVaryingTexCoords;

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoords;

uniform mat4 mvpMatrix;

void main(void) {  
  vVaryingTexCoords = vTexCoords;  
  gl_Position = mvpMatrix * vVertex;
}
