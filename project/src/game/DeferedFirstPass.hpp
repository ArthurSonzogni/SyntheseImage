#ifndef DEFEREDFIRSTPASS_98QRG1M6
#define DEFEREDFIRSTPASS_98QRG1M6

#include "game/DeferedBase.hpp"
#include "utils/glm.hpp"
#include "game/Camera.hpp"

class ShaderProgram;

class DeferedFirstPass : public DeferedBase
{
    public:
        DeferedFirstPass();

    protected:
        int objIndex;
        ModelObj* obj;
        ModelObj ground;
        GLuint texture;
        void firstPass();

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
		Camera *camera;
};


#endif /* end of include guard: DEFEREDFIRSTPASS_98QRG1M6 */
