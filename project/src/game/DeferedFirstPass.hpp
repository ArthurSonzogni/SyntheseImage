#ifndef DEFEREDFIRSTPASS_98QRG1M6
#define DEFEREDFIRSTPASS_98QRG1M6

#include "game/DeferedBase.hpp"

class ShaderProgram;

class DeferedFirstPass : public DeferedBase
{
    public:
        DeferedFirstPass();

    protected:
        ModelObj obj;
        ModelObj ground;
        GLuint texture;
        void firstPass();
};


#endif /* end of include guard: DEFEREDFIRSTPASS_98QRG1M6 */
