#include "system/Application.hpp"
#include "graphic/FrameBuffer.hpp"
#include "graphic/ModelObj.hpp"

class DeferedBase : public Application
{
    public:
        DeferedBase();

    protected:

        virtual void loop();
        virtual void firstPass();
        virtual void secondPass();

        FrameBuffer framebuffer;
        ModelObj screenObj;

};
