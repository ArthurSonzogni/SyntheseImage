#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DeferedFirstPass.hpp"
#include <iostream>
#include "utils/glError.hpp"
#include "utils/Texture.hpp"
#include "utils/file.hpp"
#include "system/Input.hpp"
#include "graphic/Shader.hpp"

using namespace std;

inline void TwEventMouseButtonGLFW3(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	TwEventMouseButtonGLFW(button, action);
}
inline void TwEventMousePosGLFW3(GLFWwindow* /*window*/, double xpos, double ypos)
{
	TwMouseMotion(int(xpos), int(ypos));
}
inline void TwEventMouseWheelGLFW3(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	TwEventMouseWheelGLFW(yoffset);
}
inline void TwEventKeyGLFW3(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	TwEventKeyGLFW(key, action);
}
inline void TwEventCharGLFW3(GLFWwindow* /*window*/, int codepoint)
{
	TwEventCharGLFW(codepoint, GLFW_PRESS);
}
inline void TwWindowSizeGLFW3(GLFWwindow* /*window*/, int width, int height)
{
	TwWindowSize(width, height);
}

struct ObjectAndString
{
	DeferedFirstPass *instance;
	std::string str;
};

void LoadObjCallback(void *clientData)
{
	ObjectAndString *oas = (ObjectAndString*)clientData;
	oas->instance->loadModel(oas->str.c_str());
}


DeferedFirstPass::DeferedFirstPass():
    DeferedBase(),
	obj(NULL),
    ground("obj/ground.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ),
        "texture/texture.png"
    ),
	camera(NULL),
	menuBar(NULL)
{
	obj = new ModelObj("obj/Charmander.obj",ShaderProgram::loadFromFile(
				"shader/geometryPass.vert",
				"shader/geometryPass.frag"
				),
			"texture/brick.jpg"
			);

	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(getWidth(), getHeight());
	menuBar = TwNewBar("Coucou");
	camera = new Camera();
	camera->setTarget(glm::vec3(0.0, 3.0, 0.0));
    glCheckError(__FILE__,__LINE__);

	initTwBar();

	glfwSetWindowSizeCallback(this->getWindow(), (GLFWwindowposfun)TwWindowSizeGLFW3);
	glfwSetMouseButtonCallback(this->getWindow(), (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(this->getWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(this->getWindow(), (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(this->getWindow(), (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(this->getWindow(), (GLFWcharfun)TwEventCharGLFW3);
}

void DeferedFirstPass::loop()
{
	DeferedBase::loop();
	TwDraw();
}


void DeferedFirstPass::firstPass()
{
	camera->update();
	view = camera->lookAt();

	projection = glm::perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
	obj->getShader().use();
	obj->getShader().setUniform("projection",projection);
	obj->getShader().setUniform("view",view);

	//obj.getShader().setUniform("model",glm::mat4(1.0));
	ground.draw();

	obj->getShader().use();
	//obj.getShader().setUniform("model",glm::scale(glm::mat4(1.0),glm::vec3(0.04)));
	obj->draw();
}

void DeferedFirstPass::initTwBar()
{
	TwAddButton(menuBar, "Model list", NULL, NULL, " label='Models'");
	std::vector<std::string> objList = directoryListFiles("./obj");
	for(std::vector<std::string>::iterator it = objList.begin() ; it != objList.end() ; it++)
	{
		const char *name = it->c_str();
		const char *desc = (std::string(" label='")+*it+std::string("'")).c_str();
		ObjectAndString *oas = new ObjectAndString;
		oas->instance = this;
		oas->str = *it;
		TwAddButton(menuBar, name, &LoadObjCallback, (void*)oas, desc);
	}
}

void DeferedFirstPass::loadModel(const char *fileName)
{
//	if(obj) delete obj;
//	std::string target = std::string("obj/")+std::string(fileName);
//
//	 if(obj->hasTexture)
//	 {
//		 obj = new ModelObj(target.c_str(), obj->getShader(), obj->getTexture());
//	 }
//	 else
//	 {
//		 obj = new ModelObj(target.c_str(), obj->getShader());
//	 }
}
