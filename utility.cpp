#include "utility.h"
#include <iostream>
#include <sstream>
glm::mat4 utility::createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir)
{
	zDir = glm::normalize(zDir);
    glm::vec3 xDir = glm::normalize(glm::cross(upDir, zDir));
    glm::vec3 yDir = glm::normalize(glm::cross(zDir, xDir));
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

glm::mat4 utility::setPos(glm::mat4& matrix, glm::vec3 pos)
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

glm::mat3 utility::RInMatrix(glm::mat4 m){
    glm::mat3 R;

    R[0][0] = m[0][0];
    R[0][1] = m[0][1];
    R[0][2] = m[0][2];

    R[1][0] = m[1][0];
    R[1][1] = m[1][1];
    R[1][2] = m[1][2];

    R[2][0] = m[2][0];
    R[2][1] = m[2][1];
    R[2][2] = m[2][2];

    return R;
}
glm::vec3 utility::lineRotatePoint(glm::vec3 anchor, glm::vec3 dir, glm::vec3 p, float alpha){
    if(isParallel(dir, p - anchor)){
        return p;
    }
    float coe = glm::dot(dir, p-anchor)/glm::dot(dir, dir);
    glm::vec3 nearest = anchor + dir*coe;
    glm::vec3 dirNew = glm::cross(dir, p - nearest);
    glm::vec3 dirX = p - nearest;
    glm::vec3 dirY = glm::normalize(dirNew) * (float)utility::length(dirX);
    glm::vec3 pNew = nearest + dirX*cos(alpha) + dirY*sin(alpha);
    return pNew;
}
glm::vec3 utility::lineRotateVector(glm::vec3 line, glm::vec3 vec, float alpha){
    return lineRotatePoint(glm::vec3(0), line, vec, alpha);
}

Vertex utility::lineRotateVertex(glm::vec3 anchor, glm::vec3 dir, Vertex p, float alpha){
    glm::vec3 pp = lineRotatePoint(anchor, dir, p.vertex, alpha);
    glm::vec3 norm = lineRotateVector(dir, p.normal, alpha);
    Vertex v = {pp, norm, p.coordinate};
    return v;
}

bool utility::isParallel(glm::vec3 vec1, glm::vec3 vec2){
    vec1 = glm::normalize(vec1);
    vec2 = glm::normalize(vec2);
    glm::vec3 c =glm::cross(vec1, vec2);
    if (length(c) < 1e-5){
        return true;
    }
    else{
        return false;
    }
}

glm::mat4 utility::setXDir(glm::mat4& matrix, glm::vec3 pos){
    matrix[0][0] = pos[0];
    matrix[0][1] = pos[1];
    matrix[0][2] = pos[2];
    return matrix;
}

glm::mat4 utility::setYDir(glm::mat4& matrix, glm::vec3 pos){
    matrix[1][0] = pos[0];
    matrix[1][1] = pos[1];
    matrix[1][2] = pos[2];
    return matrix;
}

glm::mat4 utility::setZDir(glm::mat4& matrix, glm::vec3 pos){
    matrix[2][0] = pos[0];
    matrix[2][1] = pos[1];
    matrix[2][2] = pos[2];
    return matrix;
}

Mesh utility::generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> edge, float angle){

    vector<Vertex> verVec;
    int COLUMN = edge.size()-1;
    int ROW = 100;
    for (int i = 0; i< ROW+1; i++){
        float alpha_i = angle/ROW*i;
        for (auto p:edge){
            Vertex pNew = utility::lineRotateVertex(anchor, dir, p, alpha_i);
            verVec.push_back(pNew);
        }
    }
    auto INDEX = [COLUMN](int i, int j)->int{return i+(COLUMN+1)*j;};
    vector<unsigned int> indexVec;
    for (int i =0; i<COLUMN; i++){
        for(int j = 0; j< ROW; j++){
            //可能是normal出现了问题
            indexVec.push_back(INDEX(i,j));
            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i, j+1));

            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i+1, j+1));
            indexVec.push_back(INDEX(i, j+1));
        }
    }
    //可以优化成return Mesh(verVec, indexVec);
    Mesh m(verVec, indexVec);
    return m;
}
