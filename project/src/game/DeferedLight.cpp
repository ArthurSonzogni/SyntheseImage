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
    )),
    contourObj("obj/screen.obj",ShaderProgram::loadFromFile(
        "shader/contour.vert",
        "shader/contour.frag"
    ))
{
    glCheckError(__FILE__,__LINE__);

    initTwBar();
}



void DeferedLight::secondPass()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    if (contourPassEnable)
    {
        contourObj.getShader().use();
        contourObj.getShader().setUniform("positionMap",0);
        contourObj.getShader().setUniform("colorMap",1);
        contourObj.getShader().setUniform("normalMap",2);
        contourObj.getShader().setUniform("screenInvDimensions",glm::vec2(1.0/getWidth(),1.0/getHeight()));
        contourObj.getShader().setUniform("projection",projection);
        contourObj.getShader().setUniform("view",view);
        contourObj.getShader().setUniform("contourShadowIntensity",contourShadowIntensity);
        contourObj.draw();
    }
    /////////////
    // ambient//
    ///////////
    
    ambientPassEnable^= Input::isKeyPressed(GLFW_KEY_Q);
    if (ambientPassEnable and not contourPassEnable)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        ambientObj.getShader().use();
        ambientObj.getShader().setUniform("positionMap",0);
        ambientObj.getShader().setUniform("colorMap",1);
        ambientObj.getShader().setUniform("normalMap",2);
        ambientObj.getShader().setUniform("ambientColor",glm::vec4(ambientColor,1.0));
        ambientObj.draw();
    }

    ////////////////
    // reflection//
    //////////////
    
    reflectionPassEnable ^= Input::isKeyPressed(GLFW_KEY_R);
    if (reflectionPassEnable and not contourPassEnable)
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
        reflectionObj.getShader().setUniform("specularMap",3);
        reflectionObj.getShader().setUniform("inverseProjection",glm::inverse(projection));
        reflectionObj.getShader().setUniform("projection",projection);
        reflectionObj.getShader().setUniform("view",view);
        reflectionObj.draw();
    }
    ///////////
    // local//
    /////////
    
    lightPassEnable ^= Input::isKeyPressed(GLFW_KEY_L);
    if (lightPassEnable and not contourPassEnable)
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
        sphere.getShader().setUniform("specularMap",3);
        sphere.getShader().setUniform("screenInvDimensions",glm::vec2(1.0/getWidth(),1.0/getHeight()));


        for(unsigned int i = 0; i<nbLight; ++i)
        {
            sphere.getShader().use();
            sphere.getShader().setUniform("lightRadius",lights[i].radius);
            sphere.getShader().setUniform("view",glm::scale(glm::translate(view,lights[i].position),glm::vec3(lights[i].radius)));
            sphere.getShader().setUniform("lightPosition",glm::vec3(0.0));
            sphere.getShader().setUniform("lightColor",lights[i].color);
			sphere.getShader().setUniform("projection",projection);
			sphere.getShader().setUniform("shadowsEnable",shadowsEnable && (!shadowsOnFirstLight || i==0));
            sphere.draw();
        }

        // culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    ////////////////
    // occlusion //
    //////////////
    
    occlusionPassEnable ^= Input::isKeyPressed(GLFW_KEY_O);
    if (occlusionPassEnable)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
        glBlendFunc(GL_DST_COLOR,GL_ONE);
        
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
        occlusionObj.getShader().setUniform("occlusionIntensity",occlusionIntensity);
        occlusionObj.getShader().setUniform("occlusionDistance",occlusionDistance);
        occlusionObj.draw();
    }


    glDisable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE,GL_ONE);
}

void DeferedLight::initTwBar()
{
    ambientPassEnable = true;
    occlusionPassEnable= false;
    reflectionPassEnable = false;
    ambientColor = glm::vec3(0.3,0.3,0.3);
    occlusionIntensity = 0.9;
    occlusionDistance = 0.1;
    //TwAddSeparator(menuBar, NULL, " group='shader pass' ");
    TwAddVarRW(menuBar,"AmbientPass",TW_TYPE_BOOLCPP,&ambientPassEnable,"label=\"ambient pass\" group=\"ambient\"");
    TwAddVarRW(menuBar,"LightPass",TW_TYPE_BOOLCPP,&lightPassEnable,"label=\"light pass\" group=\"light\"");
    TwAddVarRW(menuBar,"OcclusionPass",TW_TYPE_BOOLCPP,&occlusionPassEnable,"label=\"occlusion pass\" group=\"occlusion\"");
    TwAddVarRW(menuBar,"ReflectionPass",TW_TYPE_BOOLCPP,&reflectionPassEnable,"label=\"reflection pass\" group=\"reflection\"");
    TwAddVarRW(menuBar,"ContourPass",TW_TYPE_BOOLCPP,&contourPassEnable,"label=\"contourPass\" group=\"contour\"");
    TwAddVarRW(menuBar,"contourShadowIntensity",TW_TYPE_FLOAT,&contourShadowIntensity,"label=\"contourShadowIntensity\" group=\"contour\" min=0.0 max=10.0 step=0.01");
    TwAddVarRW(menuBar,"ambientColor",TW_TYPE_COLOR3F,&ambientColor,"label=\"ambientColor\" group=\"ambient\"");


    TwAddVarRW(menuBar,"occlusionIntensity",TW_TYPE_FLOAT,&occlusionIntensity,"label=\"occlusionIntensity\" group=\"occlusion\" min=0.0 max=10.0 step=0.02");
    TwAddVarRW(menuBar,"occlusionDistance",TW_TYPE_FLOAT,&occlusionDistance,"label=\"occlusionDistance\" group=\"occlusion\" min=0.0 max=0.7 step=0.001");

}
