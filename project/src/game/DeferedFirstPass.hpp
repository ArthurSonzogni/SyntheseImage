#ifndef DEFEREDFIRSTPASS_98QRG1M6
#define DEFEREDFIRSTPASS_98QRG1M6

#include "game/DeferedBase.hpp"
#include "utils/glm.hpp"
#include "game/Camera.hpp"
#include <AntTweakBar.h>

class ShaderProgram;

class DeferedFirstPass : public DeferedBase
{
    public:
        DeferedFirstPass();
		void loadModel(const char *fileName);

    protected:
		void initTwBar();

        ModelObj *obj;
        ModelObj ground;
        GLuint texture;
        void firstPass();
		void loop();

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
		Camera *camera;

		TwBar *menuBar;
};


#endif /* end of include guard: DEFEREDFIRSTPASS_98QRG1M6 */
