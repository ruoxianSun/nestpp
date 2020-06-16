#version 330
layout (location = 0) in vec3 VertexPosition;  
layout (location = 1) in vec3 VertexNormal;  

struct LightInfo{
        vec3 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};


uniform LightInfo Light;
uniform	MaterialInfo Material;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out vec3 LightIntensity;

void getEyeSpace(out vec3 position,out vec3 norm)
{
    norm = VertexNormal;// normalize(NormalMatrix * VertexNormal);
    position = VertexPosition;// vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
}

vec3 phongModel(vec3 position, vec3 norm)
{
    vec3 lightPos=vec3(1,-500,1);
    vec3 lightColor=vec3(0.3,0.4,0.5);
    vec3 lightDir = normalize(lightPos - position);
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = lightColor* sDotN;
    vec3 spec = vec3(0.0);


    vec3 eyeVec = normalize(-position);
    vec3 reflectVec = reflect(lightDir, norm);
    if(sDotN >0.0)
        spec = lightColor* pow(max(dot(eyeVec,reflectVec), 0.0), 128);
    return ambient + diffuse + spec;

}

void main()
{
    vec3 eyeNorm;
    vec3 eyePosition;
    getEyeSpace(eyePosition,eyeNorm);
    LightIntensity = phongModel(eyePosition,eyeNorm);
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
