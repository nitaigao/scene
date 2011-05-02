attribute vec4 vVertex;
attribute vec3 vNormal;

uniform vec3 vLightPosition;
uniform vec4 diffuseColor;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

varying vec4 vVaryingColor;

void main(void) {  
  
  vec3 vEyeNormal = normalMatrix * vNormal;
  
  vec4 vPosition4 = mvMatrix * vVertex;
  vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
  
  vec3 vLightDirection = normalize(vLightPosition - vPosition3);
  
  float diffuseIntensity = max(0.0, dot(vEyeNormal, vLightDirection));
  
  vVaryingColor.rgb = diffuseIntensity * diffuseColor.rgb;
  vVaryingColor.a = diffuseColor.a;
  
//  vVaryingColor = diffuseColor;
  
  gl_Position = mvpMatrix * vVertex;
}
