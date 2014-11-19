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
    ))
{
    glCheckError(__FILE__,__LINE__);
    populateLight();
}


void DeferedLight::populateLight()
{
    for(int i = 0; i < 20; ++i)
    {
        Light l;
        l.position = glm::vec3(i,0,i);
        l.radius = 10.f;
        float r,g,b;
        r = (i%7)/7.0;
        g = (i%4)/4.0;
        b = (i%3)/3.0;
        r = r*0.5 + 0.5;
        g = g*0.5 + 0.5;
        b = b*0.5 + 0.5;
        l.color = glm::vec4(r,g,b,1.0);
        lights.push_back(l);
    }
}

void DeferedLight::animateLight()
{
    float t = getTime()*0.1;
    for(int i = 0; i<lights.size(); ++i)
    {
        float ii = i%10;
        lights[i].position = glm::vec3(2.f*ii*cos(-i*t),1.f,2.f*ii*sin(-i*t));
    }

    static int i = 1;
    lights[0].position = glm::vec3(1.0,5.0,3.0);
    lights[0].radius = 20.f;

}

void DeferedLight::secondPass()
{
    animateLight();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
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
    sphere.hasTexture = false;


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
