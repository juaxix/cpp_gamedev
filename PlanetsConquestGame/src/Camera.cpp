#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

const glm::mat4& Camera::getViewProjection() const
{
	return _vp;
}

void Camera::setOrthoParams(float left, float right, float bottom, float top, float znear, float zfar)
{
    _proj = glm::ortho(left, right, bottom, top, znear, zfar);
    _vp = _proj * _view;
}

void Camera::setLookAt(const glm::vec3& origin, const glm::vec3& center, const glm::vec3& up)
{
    _view = glm::lookAt(origin, center, up);
    _vp = _proj * _view;
}
