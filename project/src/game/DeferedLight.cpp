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
    for(int i = 0; i < 10; ++i)
    {
        Light l;
        l.position = glm::vec3(i,0,i);
        l.radius = 3.f;
        lights.push_back(l);
    }
}

void DeferedLight::animateLight()
{
    float t = getTime();
    for(int i = 0; i<lights.size(); ++i)
    {
        lights[i].position = glm::vec3(3.f*i*cos(-i*t),1.f,3.f*i*sin(-i*t));
    }

    lights[0].position = glm::vec3(1.0,5.0,3.0);
    lights[0].radius = 5.f;

}

void DeferedLight::secondPass()
{
    animateLight();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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
        sphere.draw();
    }
}
