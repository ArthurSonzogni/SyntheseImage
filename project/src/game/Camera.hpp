#ifndef __CAMERA__
#define __CAMERA_
#include "utils/glm.hpp"

class Camera
{
	protected:
		static float THETA_MULTIPLIER;
		static float PHI_MULTIPLIER;
		static float ZOOM_MULTIPLIER;
		static float TRANSLATION_MULTIPLIER;
		float r;
		float theta;
		float phi;
		glm::vec3 target;
	public:
		Camera();
		void update();
		glm::mat4 lookAt();
		void setTarget(glm::vec3 target);
		void setTheta(float theta);
		void setPhi(float phi);
};

#endif
