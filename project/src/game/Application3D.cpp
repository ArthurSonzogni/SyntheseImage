#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application3D.hpp"
#include <iostream>
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "system/Input.hpp"
#include "graphic/Shader.hpp"

using namespace std;


Application3D::Application3D():
    Application(),
    obj("obj/Charmander.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    )),
    screenObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/compose.vert",
        "shader/compose.frag"
    )),
    framebuffer(getWidth(),getHeight(),3),
	camera(NULL)
{
	camera = new Camera();
    glCheckError(__FILE__,__LINE__);
}

void Application3D::loop()
{
    Input::update(getWindow());
	camera->update();

    //===== First Pass ====//
    framebuffer.bindToWrite();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    static float t=0;
    t+=0.02;
    obj.getShader().use();
    obj.getShader().setUniform("projection", glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f));
    obj.getShader().setUniform("view", camera->lookAt(glm::vec3(0.0, 3.0, 0.0)));

    obj.draw();

    if (Input::isKeyHold(GLFW_KEY_SPACE))
    {
        //===== Draw the buffers of the first pass ==//
        framebuffer.drawToScreen();
    }
    else
    {
        //===== First Pass ====//
        screenObj.getShader().use();
        framebuffer.bindToRead();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        screenObj.getShader().setUniform("positionMap",0);
        screenObj.getShader().setUniform("colorMap",1);
        screenObj.getShader().setUniform("normalMap",2);
        screenObj.draw();


    }
}
