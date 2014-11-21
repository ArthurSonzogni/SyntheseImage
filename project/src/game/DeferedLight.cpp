#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DeferedLight.hpp"
#include <iostream>
#include "utils/glError.hpp"
#include "system/Input.hpp"
#include "graphic/Shader.hpp"

using namespace std;


DeferedLight::DeferedLight():
    DeferedFirstPass(),
    sphere("obj/sphere.obj",ShaderProgram::loadFromFile(
        "shader/lightPass.vert",
        "shader/lightPass.frag"
    )),
    ambientObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/ambientPass.vert",
        "shader/ambientPass.frag"
    )),
    occlusionObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/ambientOcclusion.vert",
        "shader/ambientOcclusion.frag"
    )),
    reflectionObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/ambientReflection.vert",
        "shader/ambientReflection.frag"
    ))
{
    glCheckError(__FILE__,__LINE__);
    populateLight();
}


void DeferedLight::populateLight()
{
    for(int i = 0; i < 6; ++i)
    {
        Light l;
        l.position = glm::vec3(i,0,i);
        l.radius = 10.f;
        l.color = glm::vec4(glm::rgbColor(glm::vec3(360.f*(i-2)/6,10.0f,10.0f)),1.0);
        lights.push_back(l);
    }
}

void DeferedLight::animateLight()
{
    static float t = 0.0;
    t += getFrameDeltaTime() * 0.5;
    //cout << t << endl;
    for(int i = 0; i<lights.size(); ++i)
    {
        float ii = 2+i%10;
        lights[i].position.x = 6*sin(i*t);
        lights[i].position.y = lights[i].radius * 0.3;
        lights[i].position.z = 6*cos(i*t);
    }

    static int i = 1;
    lights[0].position = glm::vec3(1.0,5.0,3.0);
    lights[0].radius = 10.f;
    lights[0].color = glm::vec4(1.0);
}

void DeferedLight::secondPass()
{
    animateLight();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    /////////////
    // ambient//
    ///////////
    
    if (Input::isKeyHold(GLFW_KEY_Q))
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        ambientObj.getShader().use();
        ambientObj.getShader().setUniform("positionMap",0);
        ambientObj.getShader().setUniform("colorMap",1);
        ambientObj.getShader().setUniform("normalMap",2);
        ambientObj.getShader().setUniform("ambientColor",glm::vec4(1.0)*0.6f);
        ambientObj.draw();
    }

    ////////////////
    // reflection//
    //////////////
    
    if (Input::isKeyHold(GLFW_KEY_R))
    {

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_ONE,GL_ONE);
        
        static float param0 = 1.0;
        if (Input::isKeyHold(GLFW_KEY_T))
            param0 *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_G))
            param0 /= 1.01;
        //cout << "param0 = " << param0 << endl;

        static float param1 = 0.6;
        if (Input::isKeyHold(GLFW_KEY_Y))
            param1 *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_H))
            param1 /= 1.01;
        //cout << "param1 = " << param1 << endl;
        
        reflectionObj.getShader().use();
        reflectionObj.getShader().setUniform("positionMap",0);
        reflectionObj.getShader().setUniform("colorMap",1);
        reflectionObj.getShader().setUniform("normalMap",2);
        reflectionObj.getShader().setUniform("projection",projection);
        reflectionObj.draw();
    }
    ///////////
    // local//
    /////////
    
    if (!Input::isKeyHold(GLFW_KEY_L))
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        sphere.getShader().use();
        sphere.getShader().setUniform("projection",projection);
        sphere.getShader().setUniform("view",view);

        sphere.getShader().setUniform("positionMap",0);
        sphere.getShader().setUniform("colorMap",1);
        sphere.getShader().setUniform("normalMap",2);


        for(int i = 0; i<lights.size(); ++i)
        {
            sphere.getShader().use();
            sphere.getShader().setUniform("lightRadius",lights[i].radius);
            sphere.getShader().setUniform("view",glm::scale(glm::translate(view,lights[i].position),glm::vec3(lights[i].radius)));
            sphere.getShader().setUniform("lightPosition",glm::vec3(0.0));
            sphere.getShader().setUniform("lightColor",lights[i].color);
            sphere.draw();
        }

        // culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    ////////////////
    // occlusion //
    //////////////
    
    if (!Input::isKeyHold(GLFW_KEY_O))
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
        glBlendFunc(GL_ONE,GL_ONE);
        
        static float param0 = 1.0;
        if (Input::isKeyHold(GLFW_KEY_T))
            param0 *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_G))
            param0 /= 1.01;
        //cout << "param0 = " << param0 << endl;

        static float param1 = 0.6;
        if (Input::isKeyHold(GLFW_KEY_Y))
            param1 *= 1.01;
        if (Input::isKeyHold(GLFW_KEY_H))
            param1 /= 1.01;
        //cout << "param1 = " << param1 << endl;
        
        occlusionObj.getShader().use();
        occlusionObj.getShader().setUniform("positionMap",0);
        occlusionObj.getShader().setUniform("colorMap",1);
        occlusionObj.getShader().setUniform("normalMap",2);
        occlusionObj.getShader().setUniform("projection",projection);
        occlusionObj.getShader().setUniform("param0",param0);
        occlusionObj.getShader().setUniform("param1",param1);
        occlusionObj.draw();
    }


    glDisable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE,GL_ONE);
}
