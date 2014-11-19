#ifndef APPLICATION3D_3KYVM1W4
#define APPLICATION3D_3KYVM1W4
#include "system/DeferedBase.hpp"
#include "graphic/FrameBuffer.hpp"

class ShaderProgram;

class Application3D : public DeferedBase
{
    public:
        Application3D();

    protected:
        void loop();
        ModelObj obj;
        GLuint texture;
};

#endif /* end of include guard: APPLICATION3D_3KYVM1W4 */
