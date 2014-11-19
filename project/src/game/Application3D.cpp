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
    DeferedLight()
{
    glCheckError(__FILE__,__LINE__);
}
