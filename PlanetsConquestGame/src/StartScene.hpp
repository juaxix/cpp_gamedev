#ifndef __opengl_sample__EndScene__
#define __opengl_sample__EndScene__

#include "IScene.hpp"
#include <vector>
#include <string>
#include "dbTweener/dbtweener.h"

#include <glm/glm.hpp>
#include "MathUtils.hpp"


using namespace glm;
class Plane;
class GUIText;
class Camera;

class StartScene : public IScene
{   
    Camera* _camera;
	Plane* _plane;
    GUIText* _text;
	
    float _loadTime;
    float _waittingTime;
	vec3 _textPos, _planeScale, _planeRotation;
	
	CDBTweener _tweens;
	float _frameTime;
public:
    ~StartScene();
    StartScene();
	void load();
	void update();
	void render(); 
};

#endif /* defined(__opengl_sample__EndScene__) */
