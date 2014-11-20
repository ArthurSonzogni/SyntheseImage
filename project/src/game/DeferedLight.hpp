#ifndef DeferedLIGHT_DEF
#define DeferedLIGHT_DEF
#include "game/DeferedFirstPass.hpp"
#include "utils/glm.hpp"

class ShaderProgram;

class DeferedLight : public DeferedFirstPass
{
    public:

        DeferedLight();

        struct Light
        {
            glm::vec3 position;
            glm::vec4 color;
            float radius;
        };

        
    protected:
        std::vector<Light> lights;
        void populateLight();
        void animateLight();
        virtual void secondPass();
        ModelObj sphere;
        ModelObj ambientObj;
        ModelObj occlusionObj;
};

#endif /* end of include guard: DeferedLIGHT_DEF*/
