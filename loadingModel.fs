#version 430 core

out vec4 FragColor;
in vec2 texCoor;

//uniform sampler2D texture_diffuse1;

void main(){
	//FragColor = texture(texture_diffuse1, texCoor);
	FragColor = vec4(1.0f);
}
