#include "glwidget.h"

#include <QDebug>
#include <iostream>
const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";


GLWidget::GLWidget(QWidget *parent):QOpenGLWidget (0), point{0.5,0.5,0.5, 0.2,0.1,0.4, -0.3,0.0,0.1}
{
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    setFormat(fmt);
}

void GLWidget::initializeGL(){
    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    QOpenGLShaderProgram program;
    if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)){
        qDebug()<<"Failed to load vertexShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)){
        qDebug()<<"Failed to load fragmentShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    program.link();
    program.bind();

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void *)0);
}

void GLWidget::paintGL(){
    program.bind();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLWidget::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}
