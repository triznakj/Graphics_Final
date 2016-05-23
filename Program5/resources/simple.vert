#version 330

uniform mat4 P; //projection matrix
uniform mat4 C; //camera matrix
uniform mat4 mT; //model transform
uniform mat4 mR; //model rotation
uniform mat4 M; //modelView matrix = C*mR*mT
uniform mat4 N; //inverse transpose of M
uniform mat4 L; //light rotation matrix
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

in vec3 pos;
in vec3 colorIn;

smooth out mat4 smoothColor;

vec4 justColor()
{
    return vec4(colorIn, 1);
}

vec4 gouraud()
{
	int alpha = 10;
	vec3 normal = (colorIn / 0.5f) - 1.0f;

	vec4 rotNorm = mR *vec4(normal, 1);
	vec4 lightLocation = L * lightPos;
	vec4 camLocation = camPos* M;
	vec4 rotatedPos = mT * mR *vec4(pos, 1);
	

	vec4 lightVec = normalize(lightLocation - rotatedPos);
	vec4 camVec = normalize(camLocation - rotatedPos);



	//AMBIENT: DONE
	vec3 ambient = vec3(0.1,0.1,0.1) * vec3(1, 1, 1);


	//DIFFUSE: DONE
	vec3 kd = colorIn;
	vec3 id = vec3(1, 1, 1);
	vec3 diffuse = kd  * id * max(0.0,dot(lightVec, rotNorm));



	vec3 ks = vec3(1,1,1);
	vec3 ls = vec3(1,1,1);

	vec4 r = reflect(-lightVec,normalize(rotNorm));
	float theDot = max(0.0,dot(r, camVec));
	float alphaApplied = pow(theDot, alpha);

	vec3 spec = ks * ls * alphaApplied;
	
	vec3 sum = diffuse+ambient+spec;
	vec4 final = vec4(sum, 1);
	return final;
}

vec4 phong(){
	return justColor();
}


void main()
{
    //TODO add gouraud and phong shading support
    
	vec4 posV = vec4(pos, 1);
	
	gl_Position = P*M*posV;
    
    if(shadingMode == 0)
        smoothColor[0] = justColor();
		
		
    else if (shadingMode == 1)
        smoothColor[0].xyzw = gouraud();
		
    else
        smoothColor[0].xyzw = gouraud();
	smoothColor[1] = posV;
	smoothColor[2] = vec4(colorIn,1);
		
}