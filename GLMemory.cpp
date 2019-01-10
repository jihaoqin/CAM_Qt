#include "GLMemory.h"

GLMemory::GLMemory()
{

}

GLMemory::~GLMemory(){
    core->glDeleteVertexArrays(1, &VAO);
    core->glDeleteBuffers(1, &VBO);
    core->glDeleteBuffers(1, &EBO);
}
