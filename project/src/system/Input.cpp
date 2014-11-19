#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "utils/glm.hpp"

#include <map>
#include <vector>
#include <cmath>

using namespace std;

map<int,pair<int,int> > keyState;
map<int,pair<int,int> > mouseState;
double m_mouseX;
double m_mouseY;
double m_deltaX = 0.0;
double m_deltaY = 0.0;

void Input::update(GLFWwindow* window)
{
    // sticky mouse and keyboard to be sure every press/release is recorded.
    glfwSetInputMode(window,GLFW_STICKY_KEYS,true);
    glfwSetInputMode(window,GLFW_STICKY_MOUSE_BUTTONS,true);

    // get screen dimension
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // update key
    for(auto it = keyState.begin(); it!=keyState.end(); ++it)
    {
        it->second.second = it->second.first;
        it->second.first  = glfwGetKey(window,it->first);
    }

    // update mouse buttons
    for(auto it = mouseState.begin(); it!=mouseState.end(); ++it)
    {
        it->second.second = it->second.first;
        it->second.first  = glfwGetMouseButton(window,it->first);
    }

	double oldX = m_mouseX;
	double oldY = m_mouseY;
    // update mouse coords
    glfwGetCursorPos(window, &m_mouseX, &m_mouseY);
	m_deltaX = m_mouseX - oldX;
	m_deltaY = m_mouseY - oldY;
}


bool Input::isKeyPressed(int key)
{
    auto p = keyState[key];
    return (( p.first  == GLFW_PRESS ) and
            ( p.second == GLFW_RELEASE ));
}

bool Input::isKeyReleased(int key)
{
    auto p = keyState[key];
    return (( p.first  == GLFW_RELEASE ) and
            ( p.second == GLFW_PRESS ));
}

bool Input::isKeyHold(int key)
{
    auto p = keyState[key];
    return ( p.first  == GLFW_PRESS );
}


bool Input::isMousePressed(int key)
{
    auto p = mouseState[key];
    return (( p.first  == GLFW_PRESS ) and
            ( p.second == GLFW_RELEASE ));
}

bool Input::isMouseReleased(int key)
{
    auto p = mouseState[key];
    return (( p.first  == GLFW_RELEASE ) and
            ( p.second == GLFW_PRESS ));
}

bool Input::isMouseHold(int key)
{
    auto p = mouseState[key];
    return ( p.first  == GLFW_PRESS);
}


double Input::mouseX()
{
    return m_mouseX;
}

double Input::mouseY()
{
    return m_mouseY;
}

double Input::deltaX()
{
    return m_deltaX;
}

double Input::deltaY()
{
    return m_deltaY;
}
