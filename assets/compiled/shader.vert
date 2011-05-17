varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;

attribute vec4 vVertex;
attribute vec3 vNormal;

uniform vec3 vLightPosition;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

void main(void) {  
  
  vVaryingNormal = normalMatrix * vNormal;
  
  vec4 vPosition4 = mvMatrix * vVertex;
  vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
  
  vVaryingLightDir = normalize(vLightPosition - vPosition3);
  
  gl_Position = mvpMatrix * vVertex;
}
