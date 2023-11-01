#include "entity.hpp"
bool EntityContainer::Colliding(const DoublePoint& p) {
	for (int i = 0; i < hitboxes.size(); i++) {
		if (p.x >= hitboxes[i].pos.x && p.x <= (hitboxes[i].pos.x + hitboxes[i].width)) { // check if x coord is inside
			if (p.y >= hitboxes[i].pos.y && p.y <= (hitboxes[i].pos.y + hitboxes[i].height)) { // check if y coord is inside
				return true;
			}
		}
	}
	return false;
}
void Entity::AddToGArry(bool constructed) {
	{
		if (DEBUG && constructed) {
			std::cout << "Entity Constructer Called" << std::endl;
		}
		if (LOADED_ENTITIES_HEAD > ENTITY_MAX) {
			// fuck
			// easy way out = crash
			if (EMAX_CRASH) {
				std::cerr << "EMAX_CRASH";
				exit(1);
			}
			else { // uh um hm help
				ent = &LOADED_ENTITIES[ENTITY_MAX - 1]; // bad solution
				k = LOADED_ENTITIES_HEAD - 1;
				if (DEBUG) {
					std::cout << "[VERY VERY BAD] : Entity Overflow! Entity replaced end of array" << std::endl;
				}
			}
		}
		else {
			ent = &LOADED_ENTITIES[LOADED_ENTITIES_HEAD++];
			k = LOADED_ENTITIES_HEAD - 1;
			if(!constructed){
				hitboxes = ent->hitboxes.data();
				hitboxTexts = ent->hitboxTexts.data();
			}
			if (DEBUG) {
				std::cout << "Entity Loaded Succesfully" << std::endl;
			}
		}
		return;
	}
}
Entity::Entity() {
	AddToGArry(true);
}
Entity::Entity(EntityContainer in) {
	ent = &in;
}
void DrawEntity(int k){
	DrawEntity(LOADED_ENTITIES[k], k);
}
void DrawEntity(Entity& ent){
	DrawEntity(*ent.ent);
}
void DrawEntity(EntityContainer& ent, int k){
	if(ent.anim){ // use LOADED_ENTITIES[k] bc entity container is passed by value (maybe fix? because that's a lot of data)
		ent.time += GetFrameTime();
		if(ent.time > (double)(ent.frames) / (double)ent.fps){
			ent.time = 0;
			ent.currentFrame = 0;
			ent.animDone = true;
		}
		ent.currentFrame = std::floor(ent.fps * ent.time);
	}
	if(!ent.hitboxes.size() && !DEBUG){
		return;
	}
	if(ent.dontDraw){
		if(!DEBUG)
			return;
		for (int i = 0; i < ent.hitboxes.size(); i++) {
			std::string name;
			name = std::to_string(k) + "." + std::to_string(i) + "\n" + std::to_string(ent.triggerID) + "\nnodraw"; 
			DrawRectangleLines(ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, ent.hitboxes[i].width, ent.hitboxes[i].height, GREEN);
			DrawText(name.c_str(), ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, 20, BLACK);
		}
		return;
	}
	for (int i = 0; i < ent.hitboxes.size(); i++) {
		DrawTextureRec(ent.hitboxTexts[i],
			{ ent.offset.x + (ent.anim ? ent.animOffset.x * ent.currentFrame : 0 ),
			  ent.offset.y + (ent.anim ? ent.animOffset.y * ent.currentFrame : 0 ), 
			  ent.hitboxes[i].width, ent.hitboxes[i].height },
			{ ent.hitboxes[i].pos.x , ent.hitboxes[i].pos.y },
			ent.tint);

			if (DRAW_DEBUG) {
				ent.debugDrawCounter += GetFrameTime();
				if (ent.debugDrawCounter >= 1) { // so console doesnt get spammed
					ent.debugDrawCounter = 0;
					std::cout << "At Draw : Entity : " << k << " Hitbox : " << i << " " << ent.hitboxes[i].pos.x << " " << ent.hitboxes[i].pos.y << std::endl;
				}
			}
			if (DEBUG && k > 0){
				std::string name;
				name = std::to_string(k) + "." + std::to_string(i) + "\n" + std::to_string(ent.triggerID); 
				DrawRectangleLines(ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, ent.hitboxes[i].width, ent.hitboxes[i].height, GREEN);
				DrawText(name.c_str(), ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, 20, GREEN);
			}
	}
}
void DrawEntities() {
	for (int k = 1; k < LOADED_ENTITIES_HEAD; k++) {
		DrawEntity(k);
	}
	// draw entity 0 last, because it's typically the player
	DrawEntity(0);
}
void MoveEntities() {
	for (int i = 0; i < LOADED_ENTITIES_HEAD; i++) {
		for (int k = 0; k < LOADED_ENTITIES[k].hitboxes.size(); k++) {
			LOADED_ENTITIES[i].hitboxes[k].Move();
		}
	}
}
EntityContainer::EntityContainer(bool t, bool an, Point animoff, int fs, int framenum, int trig, std::vector<EntityHitbox> hbs, std::vector<Texture2D> texts, Color tnt, Point off, std::string st, bool dd){
	trigger = t;
	anim = an;
	animOffset = animoff;
	fps = fs;
	frames = framenum;
	triggerID = trig;
	hitboxes = hbs;
	hitboxTexts = texts;
	tint = tnt;
	offset = off;
	signText = st;
	dontDraw = dd;
}
EntityContainer::EntityContainer(){
	// left blank to get the compiler to stop complaining
}
