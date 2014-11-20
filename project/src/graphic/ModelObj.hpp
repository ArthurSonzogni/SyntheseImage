// created by Arthur Sonzogni

#ifndef MODELOBJ_IZ6KZI5P
#define MODELOBJ_IZ6KZI5P

#include "utils/Obj.hpp"
#include "utils/Texture.hpp"

class ShaderProgram;

class ModelObj
{
    public:
        ModelObj(const char * filename, ShaderProgram& shader);
        ModelObj(const char * filename, ShaderProgram& shader, const char * textureName);
        ~ModelObj();
        void draw();
        ShaderProgram& getShader();
        bool hasTexture;
        Obj::Dimension getDimension();
    private:
        Obj obj;
        GLuint vao;
        ShaderProgram& shader;
        Texture& texture;
};

#endif /* end of include guard: MODELOBJ_IZ6KZI5P */
