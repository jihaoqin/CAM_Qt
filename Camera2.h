#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"
#include "PerspectiveMat.h"
class Camera2
{
public:
	Camera2(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
    Camera2(BoundingBox b);
	~Camera2();
public:
    void setBoundingBox(BoundingBox);
	void rotateScene(glm::vec3 pos, float rad, glm::vec3 axis);
	glm::vec3 getPos();
    glm::mat4 viewMatrix();
    glm::mat4 perspectiveMatrix();
	void bindBoundingBox(BoundingBox b);
	void processMouseMove(float deltaX, float deltaY);
	void processScroll(double yOffset);
    void viewPortRatio(int w, int h);
private:
	PerspectiveMat perspective;
	BoundingBox box;
	glm::mat4 camBaseWorld;
	glm::mat4 worldBaseCam;
};

