#ifndef __opengl_sample__Game__
#define __opengl_sample__Game__

#include "IScene.hpp"
#include <iostream>
#include <vector>


class World;
class Camera;
class Plane;
class GUIText;

class GameScene : public IScene
{
	World* _world;
    Camera* _camera;
	Plane* _background;
	Plane* _pauseBackground;
	GUIText* _pauseText;
public:
	GameScene();
    ~GameScene();
    
    void load();
    
    void update();
    void render();
	void requestGameFinish(bool Win);
	void addEntity(GameEntity* entity);
	void removeEntity(GameEntity* entity);
};

#endif /* defined(__opengl_sample__Game__) */
