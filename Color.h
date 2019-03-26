#ifndef COLOR_H
#define COLOR_H
#include <glm/glm.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
class Color
{
public:
    static glm::vec3 RED;
    static glm::vec3 YELLOW;
    static glm::vec3 GREEN;
    static glm::vec3 BLACK;
    static glm::vec3 BLUE;
    Color();
    Color(glm::vec3);
    glm::vec3 rgb;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & rgb;
    }
};

#endif // COLOR_H
