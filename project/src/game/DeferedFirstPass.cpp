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

void LoadTexCallback(void *clientData)
{
	ObjectAndString *oas = (ObjectAndString*)clientData;
	oas->instance->loadTexture(oas->str.c_str());
}


DeferedFirstPass::DeferedFirstPass():
    DeferedBase(),
    //obj("obj/ArmadilloTex.obj",ShaderProgram::loadFromFile(
    //obj("obj/Charmander.obj",ShaderProgram::loadFromFile(
    objIndex(0),
    obj(NULL),
    ground("obj/ground.obj",ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ),
        "texture/texture.png"
    ),
    sphereObj("obj/sphere.obj",ShaderProgram::loadFromFile(
        "shader/geometryPassColor.vert",
        "shader/geometryPassColor.frag"
    )),
	camera(NULL),
	menuBar(NULL)
{
	obj = new ModelObj("obj/Charmander.obj",ShaderProgram::loadFromFile(
				"shader/geometryPass.vert",
				"shader/geometryPass.frag"
				),
			"texture/brick.jpg"
			);

	camera = new Camera();
	camera->setTarget(glm::vec3(0.0, 3.0, 0.0));
    glCheckError(__FILE__,__LINE__);

	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(getWidth(), getHeight());
	menuBar = TwNewBar("Coucou");

	initTwBar();

	glfwSetWindowSizeCallback(this->getWindow(), (GLFWwindowposfun)TwWindowSizeGLFW3);
	glfwSetMouseButtonCallback(this->getWindow(), (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(this->getWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(this->getWindow(), (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(this->getWindow(), (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(this->getWindow(), (GLFWcharfun)TwEventCharGLFW3);

    populateLight();
}

void DeferedFirstPass::loop()
{
	DeferedBase::loop();
	TwDraw();
}


void DeferedFirstPass::firstPass()
{
    animateLight();

	camera->update();
	view = camera->lookAt();

    projection = glm::perspective(70.0f, float(getWidth())/float(getHeight()), 0.1f, 100.0f);
    ground.getShader().use();
    ground.getShader().setUniform("projection",projection);
    ground.getShader().setUniform("view",view);
    ground.getShader().setUniform("model",glm::mat4(1.f));
    ground.draw();
    ////////////
    //  Obj ///
    //////////

	//loadingMutex.lock();
    if (!obj || Input::isKeyPressed(GLFW_KEY_M))
    {
        const char* objName[]=
        {
            "obj/Charmander.obj",
            "obj/ArmadilloTex.obj",
            "obj/mew.obj",
        };
        const char* textureName[]=
        {
            "texture/Charmander.tga",
            "texture/pavement.jpg",
            "texture/mew.tga",
        };
        objIndex = (objIndex+1)%3;
        if (obj) delete obj;
        obj = new ModelObj(objName[objIndex],ground.getShader(),textureName[objIndex]);
    }

    obj -> getShader().use();
    glm::mat4 objTransform(1.0);
    float size = 4.0;
    {
        auto d = obj -> getDimension();
        float dx = d.xmax-d.xmin;
        float dy = d.ymax-d.ymin;
        float dz = d.zmax-d.zmin;
        float dg = size/sqrt(sqrt(dx*dy)*dz);
        objTransform = glm::translate(objTransform,glm::vec3(0.0,+dy/2.f*dg,0.0));
        objTransform = glm::scale(objTransform,glm::vec3(dg,dg,dg));
        objTransform = glm::translate(objTransform,glm::vec3(-d.xmin-dx/2,-d.ymin-dy/2,-d.zmin-dz/2));
    }
    obj->getShader().setUniform("model",objTransform);
    obj->draw();

    /////////////
    //  Light //
    ///////////
    
    if (lightPassEnable)
    {
        sphereObj.getShader().use();
        sphereObj.getShader().setUniform("projection",projection);
        sphereObj.getShader().setUniform("view",view);

        for(int i = 0; i<lights.size(); ++i)
        {
            sphereObj.getShader().use();
            sphereObj.getShader().setUniform("color",lights[i].color);
            sphereObj.getShader().setUniform("view",glm::scale(glm::translate(view,lights[i].position),glm::vec3(lights[i].sphereRadius)));
            sphereObj.draw();
        }

        // culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

	//loadingMutex.unlock();
}

void DeferedFirstPass::initTwBar()
{
	std::vector<std::string> objList = directoryListFiles("./obj");
	for(std::vector<std::string>::iterator it = objList.begin() ; it != objList.end() ; it++)
	{
		const char *name = it->c_str();
		const char *desc = (std::string(" label='")+*it+std::string("' group='model'")).c_str();
		ObjectAndString *oas = new ObjectAndString;
		oas->instance = this;
		oas->str = *it;
		TwAddButton(menuBar, name, &LoadObjCallback, (void*)oas, desc);
	}

	std::vector<std::string> texList = directoryListFiles("./texture");
	for(std::vector<std::string>::iterator it = texList.begin() ; it != texList.end() ; it++)
	{
		const char *name = it->c_str();
		const char *desc = (std::string(" label='")+*it+std::string("' group='Model texture'")).c_str();
		ObjectAndString *oas = new ObjectAndString;
		oas->instance = this;
		oas->str = *it;
		TwAddButton(menuBar, name, &LoadTexCallback, (void*)oas, desc);
	}
    lightPassEnable = false;
}

void DeferedFirstPass::loadTexture(const char *fileName)
{
	std::string target = std::string("texture/")+std::string(fileName);
	obj->loadTexture(target.c_str());
}

void DeferedFirstPass::loadModel(const char *fileName)
{
	std::string target = std::string("obj/")+std::string(fileName);
	ModelObj *newObj = NULL;

	if(obj->hasTexture)
	{
		newObj = new ModelObj(target.c_str(), obj->getShader(), obj->getTexture());
	}
	else
	{
		newObj = new ModelObj(target.c_str(), obj->getShader());
	}
	//loadingMutex.lock();
	if(newObj)
	{
		if(obj) delete obj;
		obj = newObj;
	}
	//loadingMutex.unlock();
}

DeferedFirstPass::~DeferedFirstPass()
{
	TwTerminate();
}

void DeferedFirstPass::populateLight()
{
    for(int i = 0; i < 6; ++i)
    {
        Light l;
        l.position = glm::vec3(i,0,i);
        l.radius = 10.f;
        l.sphereRadius = 0.4;
        l.color = glm::vec4(glm::rgbColor(glm::vec3(360.f*(i-2)/6,10.0f,10.0f)),1.0);
        lights.push_back(l);
    }
}

void DeferedFirstPass::animateLight()
{
    static float t = 0.0;
    t += getFrameDeltaTime() * 0.5;
    //cout << t << endl;
    for(int i = 0; i<lights.size(); ++i)
    {
        float ii = i+2;
        lights[i].position.x = 5*sin(ii*t);
        lights[i].position.y = (4*(1+sin(i*t))+lights[i].sphereRadius)*lights[i].sphereRadius;
        lights[i].position.z = 5*cos(ii*t);
    }

    static int i = 1;
    //lights[0].position = glm::vec3(1.0,5.0,3.0);
    //lights[0].radius = 10.f;
    //lights[0].color = glm::vec4(1.0);
}
