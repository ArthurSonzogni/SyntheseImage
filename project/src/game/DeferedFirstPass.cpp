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

void LoadGroundCallback(void *clientData)
{
	ObjectAndString *oas = (ObjectAndString*)clientData;
	oas->instance->loadGround(oas->str.c_str());
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
    ground(NULL),
    sphereObj("obj/sphere.obj",ShaderProgram::loadFromFile(
        "shader/geometryPassColor.vert",
        "shader/geometryPassColor.frag"
    )),
	camera(NULL),
	menuBar(NULL),
    groundColorTexture(NULL),
    groundNormalTexture(NULL),
    groundSpecularTexture(NULL)
{
    loadGround("./ground/normal");

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

    /////////////
    // ground //
    ///////////


    ground->getShader().use();
    ground->getShader().setUniform("projection",projection);
    ground->getShader().setUniform("bumpEnable",bumpEnable);
    ground->getShader().setUniform("view",view);
    ground->getShader().setUniform("model",glm::mat4(1.f));
    ground->getShader().setUniform("screenInvDimensions",glm::vec2(1.0/getWidth(),1.0/getHeight()));
    groundColorTexture->bind(GL_TEXTURE0+0);
    ground->getShader().setUniform("colorTexture",0);
    groundNormalTexture->bind(GL_TEXTURE0+1);
    ground->getShader().setUniform("normalTexture",1);
    groundSpecularTexture->bind(GL_TEXTURE0+2);
    ground->getShader().setUniform("specularTexture",2);
    ground->draw();

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
        obj = new ModelObj(objName[objIndex],ShaderProgram::loadFromFile(
            "shader/geometryPass.vert",
            "shader/geometryPass.frag"
        ),textureName[objIndex]);
    }

    obj -> getShader().use();
    obj -> getShader().setUniform("projection",projection);
    obj -> getShader().setUniform("bumpEnable",bumpEnable);
    obj -> getShader().setUniform("view",view);
    obj -> getShader().setUniform("model",glm::mat4(1.f));
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
    
    if (lightObjEnable)
    {
        sphereObj.getShader().use();
        sphereObj.getShader().setUniform("projection",projection);
        sphereObj.getShader().setUniform("view",view);

        for(int i = 0; i<nbLight; ++i)
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
	TwAddVarRO(menuBar, "framerate", TW_TYPE_FLOAT, &m_framerate, "label='fps'");
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
    TwAddVarRW(menuBar,"animateLight",TW_TYPE_BOOLCPP,&animateLightEnable,"label=\"animatedLight\" group=\"light\"");
    animateLightEnable = true;
    TwAddVarRW(menuBar,"lightObjEnable",TW_TYPE_BOOLCPP,&lightObjEnable,"label=\"lightObjEnable\" group=\"light\"");
    lightObjEnable = false;
    TwAddVarRW(menuBar,"nbLight",TW_TYPE_UINT32,&nbLight,"label=\"Number of lights\" group=\"light\" min=0 max=6 step=1");
	nbLight = 6;
    shadowsEnable = false;
    TwAddVarRW(menuBar,"enableShadows",TW_TYPE_BOOLCPP,&shadowsEnable,"label=\"Enable Shadows\" group=\"light\"");
    shadowsOnFirstLight = false;
    TwAddVarRW(menuBar,"shadowsOnFirstLight",TW_TYPE_BOOLCPP,&shadowsOnFirstLight,"label=\"Shadows on the first light\" group=\"light\"");
    bumpEnable = false;
    TwAddVarRW(menuBar,"enableBump",TW_TYPE_BOOLCPP,&bumpEnable,"label=\"Enable Bump Mapping\" group=\"light\"");

    // bumpmapping
	std::vector<std::string> groundList = directoryListFiles("./ground");
	for(std::vector<std::string>::iterator it = groundList.begin() ; it != groundList.end() ; it++)
	{
		string name = string("./ground/")+it->c_str();
		const char *desc = (std::string(" label='")+*it+std::string("' group='ground'")).c_str();
		ObjectAndString *oas = new ObjectAndString;
		oas->instance = this;
		oas->str = name.c_str();
		TwAddButton(menuBar, it->c_str(), &LoadGroundCallback, (void*)oas, desc);
	}

}

void DeferedFirstPass::loadTexture(const char *fileName)
{
	std::string target = std::string("texture/")+std::string(fileName);
	obj->loadTexture(target.c_str());
}

void DeferedFirstPass::loadGround(std::string filename)
{
    ground = new ModelObj((filename+"/obj.obj").c_str(),ShaderProgram::loadFromFile(
        "shader/geometryPassBumpMapped.vert",
        "shader/geometryPassBumpMapped.frag"
    ));
    
    //groundColorTexture = &Texture::loadFromFile("ground/bumpMapped/color.jpg");
    groundColorTexture = new Texture(Texture::loadFromFile((filename+"/color.jpg").c_str()));
    groundNormalTexture = new Texture(Texture::loadFromFile((filename+"/normal.jpg").c_str()));
    groundSpecularTexture = new Texture(Texture::loadFromFile((filename+"/specular.jpg").c_str()));
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
        l.radius = 6.0f;
        l.sphereRadius = 0.4;
        l.color = glm::vec4(glm::rgbColor(glm::vec3(360.f*(i-2)/6,60.f*(i-1),60.0f*i+20)),1.0);
        lights.push_back(l);
    }
}

void DeferedFirstPass::animateLight()
{
    if (animateLightEnable)
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
}
