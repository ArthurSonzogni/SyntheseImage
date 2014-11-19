#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DeferedFirstPass.hpp"
#include <iostream>
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "system/Input.hpp"
#include "graphic/Shader.hpp"

using namespace std;


DeferedFirstPass::DeferedFirstPass():
    DeferedBase(),
    obj("obj/Charmander.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    )),
    ground("obj/ground.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ))
{
    glCheckError(__FILE__,__LINE__);
}


void DeferedFirstPass::firstPass()
{

    static float t=0;
    t+=0.02;
    projection = glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
    view = glm::lookAt(
            glm::vec3(10.0*sin(t),5.0,10.0*cos(t)),
            glm::vec3(0.0,3,0.0),
            glm::vec3(0.0,1.0,0.0)
    );
    obj.getShader().use();
    obj.getShader().setUniform("projection",projection);
    obj.getShader().setUniform("view",view);

    obj.draw();
    ground.draw();
}
