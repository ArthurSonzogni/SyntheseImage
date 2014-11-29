#ifndef DeferedLIGHT_DEF
#define DeferedLIGHT_DEF
#include "game/DeferedFirstPass.hpp"
#include "utils/glm.hpp"

class ShaderProgram;

class DeferedLight : public DeferedFirstPass
{
    public:

        DeferedLight();


        
    protected:
        virtual void secondPass();
        ModelObj sphere;
        ModelObj ambientObj;
        ModelObj occlusionObj;
        ModelObj reflectionObj;
        ModelObj contourObj;

        // pass activation
        virtual void initTwBar();

        bool ambientPassEnable;
        bool occlusionPassEnable;
        bool reflectionPassEnable;
        bool contourPassEnable;

        glm::vec3 ambientColor;
        
        float occlusionIntensity;
        float occlusionDistance;

        float contourShadowIntensity;

};

#endif /* end of include guard: DeferedLIGHT_DEF*/
