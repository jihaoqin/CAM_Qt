#include "Camera2.h"
#include "utility.h"
#include <string>



Camera2::Camera2(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 upDir):perspective()
{
	camBaseWorld = utility::createMat(pos, -lookDir, upDir);
	worldBaseCam = glm::inverse(camBaseWorld);
    initializeLineLight();
}

Camera2::Camera2(BoundingBox  b)
{
    //相机坐标系的Z轴是从被观察点指向相机的，Y轴是向上的，右手坐标系
    bindBoundingBox(b);
    initializeLineLight();
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

void Camera2::setUniform(std::shared_ptr<GLProgram> p){
    //设置灯光
    for(int i = 0; i<4; i++){
        std::string rgbName = "lineLight_" + std::to_string(i) + ".color";
        std::string dirNmae = "lineLight_" + std::to_string(i) + ".direction";
        p->setVec3(rgbName.c_str(), lightGroup[i].color);
        glm::vec3 dir = utility::RInMatrix(camBaseWorld) * lightGroup[i].direction;
        p->setVec3(dirNmae.c_str(), dir);
    }
    //设置camera的位置
    p->setVec3("viewPos", getPos());
    p->setMat4("view", viewMatrix());
    p->setMat4("perspective", perspectiveMatrix());
}

void Camera2::initializeLineLight(){
    LineLight light_0(glm::vec3(0.7f), glm::vec3(0,0,-1.0));
    lightGroup[0] = light_0;

    LineLight light_1(glm::vec3(0.7f), glm::vec3(0, -1, -1));
    lightGroup[1] = light_1;

    LineLight light_2(glm::vec3(0.7f), glm::vec3(1, 1, -1.0));
    lightGroup[2] = light_2;

    LineLight light_3(glm::vec3(0.7f), glm::vec3(-1, 1.0, -1.0));
    lightGroup[3] = light_3;
}

float Camera2::scrollCoefficient(float fov){
    //用抛物线函数设定视窗角度的改变速率
    //max代表最快的速率
    float max = 3;//degree
    float A = max/(-49*39);
    return A*(fov -1)*(fov - 89);
}

BoundingBox Camera2::boundingBox(){
    return box;
}

void Camera2::processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    //unProject函数的第一个输入，应该是glViewPort所指定的坐标系的坐标，及屏幕左下角为(0,0)
    //屏幕上的点+z buffer的深度信息后，都可以映射到世界坐标系内
    //处理平移是通过将鼠标在屏幕上的两点间移动，映射到世界坐标系内anchor plane上两点的移动
    //anchor plane 就是包围盒中心点所定义的面
    mPos.ry() = viewPort[3] - mPos.y();
    mLastPos.ry() = viewPort[3] - mLastPos.y();
    glm::mat4 viewModel = viewMatrix()*glm::mat4(1.0);
    glm::vec3 centerScr = glm::project(box.center(), viewModel, perspectiveMatrix(), viewPort);
    glm::vec3 lastScr = glm::vec3(mLastPos.x(), mLastPos.y(), centerScr.z);
    glm::vec3 nowScr = glm::vec3(mPos.x(), mPos.y(), centerScr.z);
    glm::vec3 lastObj = glm::unProject(lastScr, viewModel, perspectiveMatrix(), viewPort);
    glm::vec3 nowObj = glm::unProject(nowScr, viewModel, perspectiveMatrix(), viewPort);
    glm::vec3 deltaCam = lastObj - nowObj;
    glm::vec3 newCamPos = getPos() + deltaCam;
    camBaseWorld = utility::setPos(camBaseWorld, newCamPos);
    worldBaseCam = glm::inverse(camBaseWorld);
}

void Camera2::processScroll(double yOffset)
{
    float fov = perspective.getFov();
    float fovNew = fov - yOffset/120*scrollCoefficient(fov);
    if (fovNew >= 1.0f && fovNew <= 88.0f)
        perspective.setFov(fovNew);
    else if (fovNew <= 1.0f)
        perspective.setFov(1.0f);
    else if (fovNew >= 88.0f)
        perspective.setFov(88.0f);
    else
        ;
}

/*
void Camera2::processRotation(float deltaX, float deltaY)
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
*/

void Camera2::processRotation(QPoint mPos, QPoint lastPos, glm::vec4 viewPort){
    float deltaX = mPos.x() - lastPos.x();
    float deltaY = mPos.y() - lastPos.y();
    if (utility::isZero(deltaX)&&utility::isZero(deltaY)){
        return;
    }
    deltaY = -1*deltaY;
    viewPort = glm::vec4(0, 0, 800, 577);
    glm::mat4 viewModel = viewMatrix()*glm::mat4(1.0);
    glm::vec3 centerScr = glm::project(box.center(), viewModel, perspectiveMatrix(), viewPort);
    glm::vec3 p(400, 288.5, centerScr.z);
    glm::vec3 pObj = glm::unProject(p, viewModel, perspectiveMatrix(), viewPort);
    float length = sqrt(pow(deltaX, 2)+pow(deltaY, 2));
    float rad = length / 800.0 * 2 * 3.14;
    glm::vec3 axis(deltaY * -1.0, deltaX, 0.0);//旋转轴为逆时针旋转90°
    rotateScene(pObj, rad, axis);
}
