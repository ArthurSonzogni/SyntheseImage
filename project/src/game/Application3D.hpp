#ifndef APPLICATION3D_3KYVM1W4
#define APPLICATION3D_3KYVM1W4
#include "system/Application.hpp"
#include "graphic/FrameBuffer.hpp"
#include "graphic/ModelObj.hpp"
#include "Camera.hpp"

class ShaderProgram;

class Application3D : public Application
{
    public:
        Application3D();

    protected:
        void loop();
        FrameBuffer framebuffer;
        ModelObj obj;
        ModelObj screenObj;
        GLuint texture;
		Camera *camera;
};

#endif /* end of include guard: APPLICATION3D_3KYVM1W4 */
