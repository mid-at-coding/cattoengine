#ifndef entity_h
#define entity_h
#include <vector>
#include <map>
#include <set>
#include "engine.hpp"
class EntityHitbox {
public:
	DoublePoint speed = { 0 , 0 }; // speed, in px/s
	DoublePoint pos = { 0 , 0 }; // topleft corner
	int width = 1;
	int height = 1;
	double debugMoveCounter = 0;
	EntityHitbox(double posX, double posY, int widthin, int len, double speedin = 0) {
		pos.x = posX;
		pos.y = posY;
		width = widthin;
		height = len;
		speed = DoublePoint{ speedin, speedin };
	}
	EntityHitbox(DoublePoint posIn, int widthin, int len, double speedin = 0) {
		pos = posIn;
		width = widthin;
		height = len;
		speed = DoublePoint{ speedin, speedin };
	}
	EntityHitbox(Point posIn, int widthin, int len, double speedin = 0) {
		pos = PointToDoublePoint(posIn);
		width = widthin;
		height = len;
		speed = DoublePoint{ speedin, speedin };
	}
	EntityHitbox(){}
	inline void Move() {
		pos.x += speed.x;
		pos.y += speed.y;
	}
}; // hitboxes, no drawing
class Entity { // entities
	static std::set<Entity*> screenEntities; // entities we can safely delete while clearing the screen
	static std::map<int, std::vector<Entity*>> renderingPriority; // renderingPriority[0] -> map of all entities that are rendered on the bottom, etc.
																  // also update priority mostly cause i have no good reason to change it
	static std::set<int> registeredLayers; // all the layers
	int layer = 0;
public:
	std::string id;
	std::string signText;

	bool anim = false;
	Point animOffset = {64,0}; // how much the animation is offset by per animation frame (not necessarily every actual frame) 
	int fps = 5;
	int frames = 4;
	int currentFrame = 0;
	double time = 0;
	bool animDone = false; // anim run one time at least (doesn't affect looping)
	Color tint = WHITE;
	Point offset = { 0 , 0 }; // offset of the texture, in px
	bool dontDraw = false;
	
	std::vector<EntityHitbox> hitboxes;
	std::vector<Texture2D> hitboxTexts;

	virtual void Update(void);
	virtual void OnTouch(Entity&);
	virtual void OnUse(Entity&);

	void SetScreenEntity(bool reset = false); // make this entity a "screen entity" -- specify that it can be deleted when clearing the screen	
	static void ClearEntities(void);

	void SetLayer(int newLayer);
	int GetLayer();
	static void DrawEntities(void);

	bool Colliding(const DoublePoint& p);
	inline bool Colliding(const Point& p) {
		return(Colliding(PointToDoublePoint(p)));
	}
	inline bool Colliding(const EntityHitbox& ent) {
		for (int i = 0; i < hitboxes.size(); i++) {
			if(CheckCollisionRecs({(float)hitboxes[i].pos.x, (float)hitboxes[i].pos.y
								  ,(float)hitboxes[i].width,(float)hitboxes[i].height},
							  	  {(float)ent.pos.x, (float)ent.pos.y, 
								   (float)ent.width, (float)ent.height}))
				return true;
		}
		return false;
	}
	inline bool Colliding(const Entity& ent) {
		for (int i = 0; i < ent.hitboxes.size(); i++) {
			if (Colliding(ent.hitboxes[i])) {
				return true;
			}
		}
		return false;
	}

	Entity();
	Entity(std::string);
	~Entity();
};
void DrawEntity(Entity& ent);
#endif
