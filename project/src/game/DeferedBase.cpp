#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DeferedBase.hpp"
#include <iostream>
#include "utils/glError.hpp"
#include "system/Input.hpp"
#include "graphic/Shader.hpp"

using namespace std;

DeferedBase::DeferedBase():
	Application(),
	framebuffer(getWidth(),getHeight(),4),
	screenObj("obj/screen.obj",ShaderProgram::loadFromFile(
				"shader/compose.vert",
				"shader/compose.frag"
				)),
	m_framerate(0.0)
{
	glCheckError(__FILE__,__LINE__);
}

void DeferedBase::loop()
{
    static int ok = 0;
    static float delta = 0.0;
    delta = 0.9 * delta + 0.1 * getFrameDeltaTime();
    if (ok++%30 == 0)
    {
        m_framerate = 1.0/delta;
    }



    Input::update(getWindow());
    framebuffer.recreate(getWidth(),getHeight());

	//===== First Pass ====//
	framebuffer.bindToWrite();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	firstPass();

	if (Input::isKeyHold(GLFW_KEY_SPACE))
	{
		//===== Draw the buffers of the first pass ==//
		framebuffer.drawToScreen();
	}
	else
	{
		//===== Second Pass ====//
		framebuffer.bindToRead();

		secondPass();

	}
	if(Input::isKeyHold(GLFW_KEY_ESCAPE))
	{
		exit();
	}
}

void DeferedBase::firstPass()
{
	cout << "first pass" << endl;
}

void DeferedBase::secondPass()
{
	cout << "second pass" << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	screenObj.getShader().use();
	screenObj.getShader().setUniform("positionMap",0);
	screenObj.getShader().setUniform("colorMap",1);
	screenObj.getShader().setUniform("normalMap",2);
	screenObj.draw();
}
