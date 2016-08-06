[VertexShader]
//uniform vec3 lightPosition;
varying vec3 normal, eyeVec, lightDir;
void main()
{
	vec3 lightPosition = vec3(0, 0, 0);
	vec4 vertexInEye = gl_ModelViewMatrix * gl_Vertex;
	
	eyeVec = -vertexInEye.xyz;
	lightDir = vec3(lightPosition - vertexInEye.xyz);
	normal = gl_NormalMatrix * gl_Normal;
	
	gl_Position = ftransform();
}
[FragmentShader]
//uniform vec4 lightDiffuse;
//uniform vec4 lightSpecular;
//uniform float shininess;
varying vec3 normal, eyeVec, lightDir;
void main (void) 
{ 
  vec4 lightDiffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
  vec4 lightSpecular = vec4(0.0f, .0f, .0f, 1.0f);
  float shininess = 32f;

  vec4 finalColor = gl_FrontLightModelProduct.sceneColor;
  vec3 N = normalize(normal);
  vec3 L = normalize(lightDir);
  float lambert = dot(N,L);
  if (lambert > 0.0)
  { 
    finalColor += lightDiffuse * lambert;
    vec3 E = normalize(eyeVec);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, E), 0.0), shininess);
    finalColor += lightSpecular * specular;
  } 
  gl_FragColor = finalColor;
} 
