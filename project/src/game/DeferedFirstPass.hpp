#ifndef DEFEREDFIRSTPASS_98QRG1M6
#define DEFEREDFIRSTPASS_98QRG1M6

#include "game/DeferedBase.hpp"
#include "utils/glm.hpp"
#include "game/Camera.hpp"
#include <AntTweakBar.h>
#include <SFML/System.hpp>

class ShaderProgram;

class DeferedFirstPass : public DeferedBase
{
    public:
        DeferedFirstPass();
		~DeferedFirstPass();
		void loadModel(const char *fileName);
		void loadTexture(const char *fileName);

        struct Light
        {
            glm::vec3 position;
            glm::vec4 color;
            float radius;
            float sphereRadius;
        };

        void loadGround(std::string filename);

    protected:

        std::vector<Light> lights;
        void populateLight();
        void animateLight();

		virtual void initTwBar();

        int objIndex;
        ModelObj* obj;
        ModelObj* ground;
        ModelObj sphereObj;
        GLuint texture;
        void firstPass();
		void loop();

        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
		Camera *camera;

		TwBar *menuBar;

		sf::Mutex loadingMutex;

        bool lightPassEnable;
        bool animateLightEnable;
        bool lightObjEnable;
		bool shadowsEnable;
		unsigned int nbLight;

        Texture* groundColorTexture;
        Texture* groundNormalTexture;
        Texture* groundSpecularTexture;

};


#endif /* end of include guard: DEFEREDFIRSTPASS_98QRG1M6 */
