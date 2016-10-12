#ifndef opengl_sample_IScene_hpp
#define opengl_sample_IScene_hpp

class GameEntity;

class  IScene
{
	
public:
    virtual ~IScene(){};
    
    virtual void load(){};
    virtual void addEntity(GameEntity* e){};
    virtual void removeEntity(GameEntity* e){};
    
    virtual void update(){};
    virtual void render(){};
};

#endif
