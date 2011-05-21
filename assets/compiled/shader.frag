uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec4 ambientColor;

varying vec3 vVaryingLightDir;
varying vec3 vVaryingNormal;

void main(void) {
  
  vec3 vEyeNormal = normalize(vVaryingNormal);
  vec3 vLightDirection = normalize(vVaryingLightDir);
  
  float diffuseIntensity = max(0.0, dot(vLightDirection, vEyeNormal));
  
  gl_FragColor = diffuseIntensity * diffuseColor;
  
  gl_FragColor += ambientColor;
  
  vec3 vReflection = normalize(reflect(-vLightDirection, vEyeNormal));
  float spec = max(0.0, dot(vEyeNormal, vReflection));
  
  if (diffuseIntensity != 0.0) {
    float fSpec = pow(spec, 128.0);
    gl_FragColor.rgb += vec3(fSpec, fSpec, fSpec);
  }
}