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
    //obj("obj/ArmadilloTex.obj",ShaderProgram::loadFromFile(
    obj("obj/Charmander.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ),
        "texture/Charmander.tga"
        //"texture/brick.jpg"
    ),
    ground("obj/ground.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ),
        "texture/texture.png"
    ),
	camera(NULL)
{
	camera = new Camera();
	camera->setTarget(glm::vec3(0.0, 3.0, 0.0));
    glCheckError(__FILE__,__LINE__);
}


void DeferedFirstPass::firstPass()
{
	camera->update();
	view = camera->lookAt();

    projection = glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
    obj.getShader().use();
    obj.getShader().setUniform("projection",projection);
    obj.getShader().setUniform("view",view);

    //obj.getShader().setUniform("model",glm::mat4(1.0));
    ground.draw();

    obj.getShader().use();
    //obj.getShader().setUniform("model",glm::scale(glm::mat4(1.0),glm::vec3(0.04)));
    obj.draw();
}
