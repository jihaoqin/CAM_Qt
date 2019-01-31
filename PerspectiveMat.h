#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class PerspectiveMat {
public:
	PerspectiveMat();
	PerspectiveMat(float f, float r, float near, float far);
	~PerspectiveMat();
	glm::mat4 getMatrix();
	float getNearPlane();
	float getFov();
	void setFov(float);
    void setRatio(float);
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & fov & ratio & nearPlane & farPlane & matrix;
    }
	float fov;
	float ratio;
	float nearPlane;
	float farPlane;
	glm::mat4 matrix;
	void update();
};
