#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "ModelReader.h"
#include "PerspectiveMat.h"
#include "LineLight.h"
#include <QOpenGLShaderProgram>
class Camera2
{
public:
	Camera2(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
    Camera2(BoundingBox b);
	~Camera2();
public:
	void rotateScene(glm::vec3 pos, float rad, glm::vec3 axis);
	glm::vec3 getPos();
    glm::mat4 viewMatrix();
    glm::mat4 perspectiveMatrix();
	void bindBoundingBox(BoundingBox b);
    BoundingBox boundingBox();
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
	void processScroll(double yOffset);
    void viewPortRatio(int w, int h);
    void setUniform(std::shared_ptr<GLProgram>);
private:
    void initializeLineLight();
    float scrollCoefficient(float);
    LineLight lightGroup[4];
	PerspectiveMat perspective;
	BoundingBox box;
	glm::mat4 camBaseWorld;
	glm::mat4 worldBaseCam;
};

