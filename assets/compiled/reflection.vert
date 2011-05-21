
varying vec3 vVaryingTextureCoord;

attribute vec4 vVertex;
attribute vec3 vNormal;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform mat4 mInverseCamera;

void main(void) {  
  
  vec3 vEyeNormal = normalMatrix * vNormal;
  
  vec4 vPosition4 = mvMatrix * vVertex;
  vec3 vEyePosition = vPosition4.xyz / vPosition4.w;
  
  vec4 vReflected = vec4(reflect(vEyePosition, vEyeNormal), 1.0);
  
  vReflected = mInverseCamera * vReflected;
  vVaryingTextureCoord.xyz = normalize(vReflected.xyz);
  
  gl_Position = mvpMatrix * vVertex;
}
