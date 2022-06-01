#ifndef entity_h
#define entity_h
#include <vector>
#include <cmath>
class EntityHitbox{ // hitboxes, no drawing
public :
	double speedX = 0;
	double speedY = 0; 
	double x = 0; // top left x
	double y = 0; // top left y
	int width = 1;
	int height = 1;
	double speed;
	double debugMoveCounter = 0;
	EntityHitbox(double posX, double posY, int widthin, int len ,  double speedin = 20){
		x = posX;
		y = posY;
		width = widthin;
		height = len;
		speed = speedin;
	}
	void Move(){
		x = x + DT * speedX * CELLSIZEX;
		y = y + DT * speedY * CELLSIZEY;
		if(DEBUG){
			debugMoveCounter += DT;
			if(debugMoveCounter >= 1){
				debugMoveCounter = 0;
				std::cout<<x<<" "<<y<<std::endl;
			}
		}
	}
};
class Entity{ // container class for entities
private : 
	bool NumBetweenNums(double p1, double p2, double p3){
		if(p3 > min(p1,p2) && p3 < max(p1,p2)) return true;
		return false;
	}
public : 
	std::vector<EntityHitbox> hitboxes;
	std::vector<Texture2D> hitboxTexts;
	double debugDrawCounter = 0;
	void Draw(Color tint = WHITE){
		for(int i = 0; i < hitboxes.size(); i++){
			DrawTextureRec(hitboxTexts[i],
			{ 0 , 0 , hitboxes[i].width * CELLSIZEX, hitboxes[i].height * CELLSIZEY } ,
			{ hitboxes[i].x , hitboxes[i].y } , 
			tint);	2q
			if(DEBUG){
				debugDrawCounter += DT;
				if(debugDrawCounter >= 1){
					debugDrawCounter = 0;
					std::cout<<"At Draw : Hitbox : "<<i<<" "<<hitboxes[i].x<<" "<<hitboxes[i].y<<std::endl;
				}
			}
		}
		bool Colliding(Entity ent)
		bool Colliding(Point p)
		bool Colliding(Point p1, Point p2)
		bool Colliding(EntityHitbox ent){
			for(int i = 0; i < hitboxes.size(); i++){
				if(ent.x > hitboxes[i].x + size || )
				if()
			}
		}
	}
};
#endif
