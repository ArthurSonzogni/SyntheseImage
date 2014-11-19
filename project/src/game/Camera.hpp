#ifndef __CAMERA__
#define __CAMERA_
#include "utils/glm.hpp"

class Camera
{
	protected:
		static float THETA_MULTIPLIER;
		static float PHI_MULTIPLIER;
		static float ZOOM_MULTIPLIER;
		float r;
		float theta;
		float phi;
	public:
		Camera();
		void update();
		glm::mat4 lookAt(glm::vec3 target = glm::vec3(0.0,0.0,0.0));
};

#endif
