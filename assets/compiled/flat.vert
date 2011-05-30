uniform mat4 mvpMatrix;
varying vec4 vVaryingColor;

attribute vec4 vVertex;
attribute vec4 vColor;

void main(void) {  
  vVaryingColor = vColor;  
  gl_Position = mvpMatrix * vVertex;
}
