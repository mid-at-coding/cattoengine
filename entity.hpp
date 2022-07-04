#ifndef entity_h
#define entity_h
#include <vector>
#include <cmath>
class EntityHitbox{ // hitboxes, no drawing
public :
	double speedX = 0;
	double speedY = 0; 
	DoublePoint pos; // topleft corner
	int width = 1;
	int height = 1;
	double speed;
	double debugMoveCounter = 0;
	EntityHitbox(double posX, double posY, int widthin, int len ,  double speedin = 0){
		pos.x = posX;
		pos.y = posY;
		width = widthin;
		height = len;
		speed = speedin;
	}
	EntityHitbox(DoublePoint posIn, int widthin, int len ,  double speedin = 0){
		pos = posIn;
		width = widthin;
		height = len;
		speed = speedin;
	}
	EntityHitbox(Point posIn, int widthin, int len, double speedin = 0) {
		pos = PointToDoublePoint(posIn);
		width = widthin;
		height = len;
		speed = speedin;
	}
	void Move(){
		pos.x = pos.x + DT * speedX ;
		pos.y = pos.y + DT * speedY ;
		if(DEBUG){
			debugMoveCounter += DT;
			if(debugMoveCounter >= 1){
				debugMoveCounter = 0;
				std::cout<<pos.x<<" "<<pos.y<<std::endl;
			}
		}
	}
};
class Entity{ // container class for entities
private : 
	bool NumBetweenNums(double p1, double p2, double p3){
		if(p3 > std::min(p1,p2) && p3 < std::max(p1,p2)) return true;
		return false;
	}
public : 
	std::vector<EntityHitbox> hitboxes;
	std::vector<Texture2D> hitboxTexts;
	double debugDrawCounter = 0;
	void Draw(Color tint = WHITE){
		for(int i = 0; i < hitboxes.size(); i++){
			DrawTextureRec(hitboxTexts[i],
			{ 0 , 0 , hitboxes[i].width, hitboxes[i].height} ,
			{ hitboxes[i].pos.x , hitboxes[i].pos.y } , 
			tint);
			if(DEBUG){
				debugDrawCounter += DT;
				if(debugDrawCounter >= 1){ // so console doesnt get spammed
					debugDrawCounter = 0;
					std::cout<<"At Draw : Hitbox : "<<i<<" "<<hitboxes[i].pos.x<<" "<<hitboxes[i].pos.y<<std::endl;
				}
			}
		}
	}


	bool Colliding(DoublePoint p){ // point
		for(int i = 0; i < hitboxes.size(); i++){
			if (DEBUG) { std::cout << "Checking Point " << p.x << " , " << p.y << " for hitbox " << i; }
			if(p.x >= hitboxes[i].pos.x && p.x <= (hitboxes[i].pos.x + hitboxes[i].width)){ // check if x coord is inside
				if(p.y >= hitboxes[i].pos.y && p.y <= (hitboxes[i].pos.y + hitboxes[i].height)){ // check if y coord is inside
					std::cout << " True" << std::endl;
					return true;
				}
			}
		}
		std::cout << " False" << std::endl;
		return false;
	}
	bool Colliding(Point p1, Point p2){ // line
		return(Colliding(PointToDoublePoint(p1)) || Colliding(PointToDoublePoint(p2)));
	}
	bool Colliding(DoublePoint p1, DoublePoint p2){ // more lines
		return(Colliding(p1) || (Colliding(p2)));
	}
	bool Colliding(Point p){
		return(Colliding(PointToDoublePoint(p)));
	}
	bool Colliding(EntityHitbox ent){
		for(int i = 0; i < hitboxes.size(); i++){
			if (Colliding((DoublePoint){ ent.pos.x , ent.pos.y})) { // topleft
				return true;
			} 
			if(Colliding((DoublePoint){ (ent.pos.x + ent.width) , ent.pos.y })){ // topright
				return true;
			}
			if(Colliding((DoublePoint){ (ent.pos.x + ent.width) , (ent.pos.y + ent.height)})){ // bottomright
				return true;
			}
			if(Colliding((DoublePoint){ ent.pos.x  , (ent.pos.y + ent.height) })){ // bottomleft
				return true;
			}
		}
		return false;
	}
	bool Colliding(Entity ent){
		for (int i = 0; i < ent.hitboxes.size(); i++) {
			if (Colliding(ent.hitboxes[i])) {
				return true;
			}
		}
		return false;
	}
};
#endif
