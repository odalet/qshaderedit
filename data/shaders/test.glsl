[VertexShader]
varying vec3 v_V;
varying vec3 v_N;

uniform float time;

void main() {
	
	vec4 a = gl_Vertex;
	a.x = a.x * 0.5;
	a.y = a.y * 0.5;	
	a.z *= abs(cos(time) + 0.5);
	
	gl_Position = gl_ModelViewProjectionMatrix * a;
	
//	gl_Position = ftransform();
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
varying vec3 v_V;
varying vec3 v_N;

void main() {
	vec3 N = normalize(v_N);
	vec3 V = normalize(v_V);
	vec3 R = reflect(V, N);
	vec3 L = normalize(vec3(gl_LightSource[0].position));

	vec4 ambient = gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * max(dot(L, N), 0.0);
	vec4 specular = gl_FrontMaterial.specular * pow(max(dot(R, L), 0.0), gl_FrontMaterial.shininess);

	gl_FragColor = ambient + diffuse + specular;
}
