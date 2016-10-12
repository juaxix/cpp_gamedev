#ifndef __opengl_sample__Camera__
#define __opengl_sample__Camera__

#include <glm/glm.hpp>

class Camera
{
    glm::mat4 _view;
    glm::mat4 _proj;
    
	glm::mat4 _vp;
    
public:
    
    void setOrthoParams(float left, float right, float bottom, float top, float znear, float zfar);
    void setLookAt(const glm::vec3& origin, const glm::vec3& center, const glm::vec3& up);
    
	const glm::mat4& getViewProjection() const;
};

#endif /* defined(__opengl_sample__Camera__) */
