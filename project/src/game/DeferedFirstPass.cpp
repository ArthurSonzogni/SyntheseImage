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
    )),
	camera(NULL)
{
	camera = new Camera();
    glCheckError(__FILE__,__LINE__);
}


void DeferedFirstPass::firstPass()
{
	camera->update();
	view = camera->lookAt(glm::vec3(0.0,3.0,0.0));

    projection = glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
    obj.getShader().use();
    obj.getShader().setUniform("projection",projection);
    obj.getShader().setUniform("view",view);

    obj.draw();
    ground.draw();
}
