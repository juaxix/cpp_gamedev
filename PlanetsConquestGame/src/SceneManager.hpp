#ifndef __opengl_sample__SceneManager__
#define __opengl_sample__SceneManager__

class IScene;

class SceneManager
{
	IScene* _current;
	IScene* _nextScene;
    
public:
	SceneManager();
    ~SceneManager();
    
	void update();
	void render();
    
	void loadScene(IScene* scene);
    
	IScene* getCurrent();
};

#endif /* defined(__opengl_sample__SceneManager__) */
