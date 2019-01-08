#include "utility.h"
#include <iostream>
#include <sstream>
glm::mat4 utility::createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir)
{
	zDir = glm::normalize(zDir);
	glm::vec3 xDir = glm::cross(upDir, zDir);
	glm::vec3 yDir = glm::cross(zDir, xDir);
	glm::mat4 matrix(1.0f);
	matrix[0][0] = xDir[0];
	matrix[0][1] = xDir[1];
	matrix[0][2] = xDir[2];

	matrix[1][0] = yDir[0];
	matrix[1][1] = yDir[1];
	matrix[1][2] = yDir[2];

	matrix[2][0] = zDir[0];
	matrix[2][1] = zDir[1];
	matrix[2][2] = zDir[2];

	matrix[3][0] = pos[0];
	matrix[3][1] = pos[1];
	matrix[3][2] = pos[2];
	return matrix;
}

glm::mat4 utility::setPos(glm::mat4 matrix, glm::vec3 pos)
{
	matrix[3][0] = pos[0];
	matrix[3][1] = pos[1];
	matrix[3][2] = pos[2];
	return matrix;
}

double utility::length(glm::vec3 v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	return l;
}

bool utility::isZero(double num){
   double MIN = 1e-6;
   if (abs(num) < MIN){
       return true;
   }
   else{
       return false;
   }
}

void utility::print(glm::vec3 v){
    std::cout<<"["<<v.x<<", "<<v.y<<", "<<v.z<<"]\n";
}

std::string utility::matLog(glm::mat4 m){
    std::string str;
    std::ostringstream stream;
    for (int i = 0; i<4 ; i++){
        //stream<<m[i][0] <<", "<<m[i][1] <<", "<<m[i][2] <<", "<<m[i][3] <<",\n";
        stream<<m[0][i] <<", "<<m[1][i] <<", "<<m[2][i] <<", "<<m[3][i] <<",\n";
        str = str + stream.str();
        stream.str("");
    }
    str.insert(0, "[");
    str.insert(str.end()-1,']');
    return str;
}
