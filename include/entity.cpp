#include "entity.hpp"
bool EntityContainer::Colliding(DoublePoint p) {
	for (int i = 0; i < hitboxes.size(); i++) {
		if (p.x >= hitboxes[i].pos.x && p.x <= (hitboxes[i].pos.x + hitboxes[i].width)) { // check if x coord is inside
			if (p.y >= hitboxes[i].pos.y && p.y <= (hitboxes[i].pos.y + hitboxes[i].height)) { // check if y coord is inside
				return true;
			}
		}
	}
	return false;
}
Entity::Entity() {
	if (DEBUG) {
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
			ent = &LOADED_ENTITIES[ENTITY_MAX]; // bad solution
			if (DEBUG) {
				std::cout << "Entity Overflow! Entity replaced end of array" << std::endl;
			}
		}
	}
	else {
		ent = &LOADED_ENTITIES[LOADED_ENTITIES_HEAD++];
		if (DEBUG) {
			std::cout << "Entity Loaded Succesfully" << std::endl;
		}
	}
	return;
}
void Entity::AddToGArry() {
	{
		if (DEBUG) {
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
				ent = &LOADED_ENTITIES[ENTITY_MAX]; // bad solution
				if (DEBUG) {
					std::cout << "Entity Overflow! Entity replaced end of array" << std::endl;
				}
			}
		}
		else {
			ent = &LOADED_ENTITIES[LOADED_ENTITIES_HEAD++];
			if (DEBUG) {
				std::cout << "Entity Loaded Succesfully" << std::endl;
			}
		}
		return;
	}
}
Entity::Entity(EntityContainer in) {
	ent = &in;
}
void DrawEntities() { // dont question it im begging you
	for (int k = 1; k < LOADED_ENTITIES_HEAD; k++) {
		if (!LOADED_ENTITIES[k].isTrigger) {
			for (int i = 0; i < LOADED_ENTITIES[k].hitboxes.size(); i++) {
				DrawTextureRec(LOADED_ENTITIES[k].hitboxTexts[i],
					{ 0 , 0 , (LOADED_ENTITIES[k].hitboxes[i].width), (LOADED_ENTITIES[k].hitboxes[i].height) },
					{ (LOADED_ENTITIES[k].hitboxes[i].pos.x) , (LOADED_ENTITIES[k].hitboxes[i].pos.y) },
					LOADED_ENTITIES[k].tint);
				if (DEBUG) {
					LOADED_ENTITIES[k].debugDrawCounter += GetFrameTime();
					if (LOADED_ENTITIES[k].debugDrawCounter >= 1) { // so console doesnt get spammed
						LOADED_ENTITIES[k].debugDrawCounter = 0;
						std::cout << "At Draw : Entity : " << k << " Hitbox : " << i << " " << LOADED_ENTITIES[k].hitboxes[i].pos.x << " " << LOADED_ENTITIES[k].hitboxes[i].pos.y << std::endl;
					}
				}
			}
		}
	}
	// draw entity 0 last, because it's typically the player
	if (!LOADED_ENTITIES[0].isTrigger) {
		for (int i = 0; i < LOADED_ENTITIES[0].hitboxes.size(); i++) {
			DrawTextureRec(LOADED_ENTITIES[0].hitboxTexts[i],
				{ 0 , 0 , (LOADED_ENTITIES[0].hitboxes[i].width), (LOADED_ENTITIES[0].hitboxes[i].height) },
				{ (LOADED_ENTITIES[0].hitboxes[i].pos.x) , (LOADED_ENTITIES[0].hitboxes[i].pos.y) },
				LOADED_ENTITIES[0].tint);
			if (DEBUG) {
				LOADED_ENTITIES[0].debugDrawCounter += GetFrameTime();
				if (LOADED_ENTITIES[0].debugDrawCounter >= 1) { // so console doesnt get spammed
					LOADED_ENTITIES[0].debugDrawCounter = 0;
					std::cout << "At Draw : Entity : " << 0 << " Hitbox : " << i << " " << LOADED_ENTITIES[0].hitboxes[i].pos.x << " " << LOADED_ENTITIES[0].hitboxes[i].pos.y << std::endl;
				}
			}
		}
	}
}
void MoveEntities() {
	for (int i = 0; i < LOADED_ENTITIES_HEAD; i++) {
		if (!LOADED_ENTITIES[i].isTrigger) {
			for (int k = 0; k < LOADED_ENTITIES[k].hitboxes.size(); k++) {
				LOADED_ENTITIES[i].hitboxes[k].Move();
			}
		}
	}
}
