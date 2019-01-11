#include "Camera2.h"
#include "utility.h"



Camera2::Camera2(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 upDir):perspective()
{
	camBaseWorld = utility::createMat(pos, -lookDir, upDir);
	worldBaseCam = glm::inverse(camBaseWorld);
}

Camera2::Camera2(BoundingBox  b)
{
    //相机坐标系的Z轴是从被观察点指向相机的，Y轴是向上的，右手坐标系
	box = b;
    glm::vec3 zDir = glm::vec3(b.xmax, b.ymax, b.zmax) - b.center();
    glm::vec3 pos = b.center()+20.0f*zDir;
    glm::vec3 upDir = glm::vec3(0.0, 1.0, 0.0);
	camBaseWorld = utility::createMat(pos, zDir, upDir);
	worldBaseCam = glm::inverse(camBaseWorld);
	double diameter = 2 * utility::length(zDir);
    perspective = PerspectiveMat(45.0f, 8.0 / 6.0, 0.1*diameter, 30 * diameter);
}


Camera2::~Camera2()
{
}

void Camera2::rotateScene(glm::vec3 pos, float rad, glm::vec3 axis)
{
    //Model = W旧(旧相对于W), View = 机W(W相对于机)， View*Model = 机旧
	//R = 旧新， View*Model*R = 机新, 
	//其中Model为inverse(View)，移动到pos点的矩阵，R为旋转矩阵
    //其中旧为模型在旋转之前的坐标系，新为旋转视角之后的模型的坐标系
	//新的View矩阵为 View_new = View*Model*R*inverse(Model)
	glm::mat4 view = worldBaseCam;
	glm::mat4 model = utility::setPos(camBaseWorld, pos);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), rad, axis);
	glm::mat4 log = view * model * R * glm::inverse(model);
	camBaseWorld = glm::inverse(log);
	glm::vec3 posNew = glm::vec3(camBaseWorld[3][0], camBaseWorld[3][1], camBaseWorld[3][2]);
    utility::print(posNew);
	glm::vec3 zDir = glm::vec3(camBaseWorld[2][0], camBaseWorld[2][1], camBaseWorld[2][2]);
	glm::vec3 yDir = glm::vec3(camBaseWorld[1][0], camBaseWorld[1][1], camBaseWorld[1][2]);
	camBaseWorld = utility::createMat(posNew, zDir, yDir);
	worldBaseCam = glm::inverse(camBaseWorld);
}

glm::vec3 Camera2::getPos()
{
	float x = camBaseWorld[3][0];
	float y = camBaseWorld[3][1];
	float z = camBaseWorld[3][2];
	glm::vec3 pos(x, y, z);
	return pos;
}



void Camera2::bindBoundingBox(BoundingBox b)
{
	box = b;
	glm::vec3 zDir = glm::vec3(b.xmax, b.ymax, b.zmax) - b.center();
	double diameter = 2 * utility::length(zDir);
	glm::vec3 pos = b.center() + 20.0f * zDir;
	glm::vec3 upDir = glm::vec3(0.0, 1.0, 0.0);
	camBaseWorld = utility::createMat(pos, zDir, upDir);
	worldBaseCam = glm::inverse(camBaseWorld);
	perspective = PerspectiveMat(45.0f, 8.0 / 6.0, 0.1*diameter, 30 * diameter);
}

void Camera2::processMouseMove(float deltaX, float deltaY)
{
    deltaY = -1 * deltaY;
    if (utility::isZero(deltaX)&&utility::isZero(deltaY)){
        return;
    }
	float length = sqrt(pow(deltaX, 2)+pow(deltaY, 2));
    float rad = length / 800.0 * 2 * 3.14;
    glm::vec3 axis(deltaY * -1.0, deltaX, 0.0);//旋转轴为逆时针旋转90°
	rotateScene(box.center(), rad, axis);
}

void Camera2::processScroll(double yOffset)
{
	float fov = perspective.getFov();
    float fovNew = fov - yOffset/120*3;
    if (fovNew >= 1.0f && fovNew <= 88.0f)
        perspective.setFov(fovNew);
    else if (fovNew <= 1.0f)
		perspective.setFov(1.0f);
    else if (fovNew >= 88.0f)
		perspective.setFov(88.0f);
	else
		;
}

glm::mat4 Camera2::viewMatrix(){
   return worldBaseCam;
}
glm::mat4 Camera2::perspectiveMatrix(){
   return perspective.getMatrix();
}

void Camera2::viewPortRatio(int w, int h){
    float ratio = w*1.0/h;
   perspective.setRatio(ratio);
}

void Camera2::setBoundingBox(BoundingBox b){
     box = b;
}
