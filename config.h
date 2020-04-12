#ifndef CONFIG_H
#define CONFIG_H

//#define LAB
#define ROOM

#ifdef LAB
#define VERTEX_PATH "E:\\0qin\\Qt\\CAM_Qt\\vertexShader.vs"
#define MESH_FRAGMENT_PATH "E:\\0qin\\Qt\\CAM_Qt\\meshFragShader.fs"
#define POINT_FRAGMENT_PATH "E:\\0qin\\Qt\\CAM_Qt\\pointFragShader.fs"
#define MODEL_PATH "E:\\0qin\\Qt\\CAM_Qt\\tee.STL"
#endif

#ifdef ROOM
#define VERTEX_PATH "F:\\0qin\\Qt\\CAM_Qt\\vertexShader.vs"
#define MESH_FRAGMENT_PATH "F:\\0qin\\Qt\\CAM_Qt\\meshFragShader.fs"
#define POINT_FRAGMENT_PATH "F:\\0qin\\Qt\\CAM_Qt\\pointFragShader.fs"
#define MODEL_PATH "F:\\0qin\\Qt\\CAM_Qt\\3.STL"
#endif

#endif // CONFIG_H
