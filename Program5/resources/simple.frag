#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

smooth in mat4 smoothColor;


out vec4 fragColor;

vec4 phong(){
	//return smoothColor[0];

	int alpha = 10;
	vec4 normal = (normalize(smoothColor[0]) / 0.5f) - 1.0f;

	vec4 rotNorm = smoothColor[0];
	vec4 lightLocation = L * lightPos;
	vec4 camLocation = camPos* C * mR * mT;
	vec4 rotatedPos = mT * mR * smoothColor[1];
	//vector to light, vector to cam
	vec4 lightVec = normalize(lightLocation - rotatedPos);
	vec4 camVec = normalize(camLocation - rotatedPos);



	//AMBIENT: DONE
	vec3 ambient = vec3(0.1,0.1,0.1) * vec3(1, 1, 1);


	//DIFFUSE: DONE
	vec3 kd = vec3(smoothColor[0].xyz);
	kd = (kd + 1.0f) * 0.5f;
	vec3 id = vec3(1, 1, 1);
	vec3 diffuse = kd  * id * max(0.0,dot(lightVec, rotNorm));

	vec3 ks = vec3(1,1,1);
	vec3 ls = vec3(1,1,1);
	

	vec4 r = reflect(-lightVec,normalize(rotNorm));
	float theDot = max(0.0,dot(r, camVec));
	float alphaApplied = pow(theDot, alpha);

	vec3 spec = ks * ls * alphaApplied;
	return ambient + diffuse + spec;
	//return (diffuse + ambient + spec);

}

void main()
{
	fragColor = smoothColor[0];

	if(shadingMode == 0)
        fragColor = smoothColor[0];
    else if (shadingMode == 1)
        fragColor = smoothColor[0];
    else
        fragColor = phong();

    //TODO add gouraud and phong shading support
}
