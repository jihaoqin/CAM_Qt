#version 430 core

out vec4 FragColor;
in vec3 normal;
in vec3 worldObject;

struct Material{
        vec3 color;
};

struct LineLight{
   vec3 direction;
   vec3 color;
};

uniform Material material;
uniform LineLight lineLight_1;
uniform LineLight lineLight_2;
uniform LineLight lineLight_3;
uniform LineLight lineLight_0;
uniform vec3 viewPos;

vec4 calcLineLight(LineLight light);

void main(){
        vec4 color_0 = calcLineLight(lineLight_0);
        vec4 color_1 = calcLineLight(lineLight_1);
        vec4 color_2 = calcLineLight(lineLight_2);
        vec4 color_3 = calcLineLight(lineLight_3);
        FragColor = color_0 + color_1 + color_2 + color_3;
        //FragColor = color_2;
}

vec4 calcLineLight(LineLight light){
    //pre-process
        light.direction = normalize(light.direction);
    //ambient
        vec3 ambient = light.color * material.color * 0.1;
    //diffuse
        float coe = 0.3;
        vec3 norm = normalize(normal);
        float diff = max(dot(light.direction*(-1.0f), norm), 0.0f);
        vec3 diffuse = material.color * light.color * diff * coe;
    //specular
        coe = 0.2;
        vec3 viewDir = normalize(viewPos - worldObject);
        vec3 reflectDir = reflect(light.direction, norm);
        float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 8);
        vec3 specular = spec * material.color * light.color * coe;
        vec3 result =ambient + diffuse + specular;
        //vec3 result = diffuse;
        return vec4( result, 1.0f);
}
