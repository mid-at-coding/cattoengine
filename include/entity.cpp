#include "entity.hpp"
#include <iostream>
#include <cmath>
std::set<Entity*> Entity::screenEntities; // entities we can safely delete while clearing the screen
std::map<int, std::vector<Entity*>> Entity::renderingPriority; // renderingPriority[0] -> map of all entities that are rendered on the bottom, etc.
std::set<int> Entity::registeredLayers; // all the layers
bool Entity::Colliding(const DoublePoint& p) {
	for (int i = 0; i < hitboxes.size(); i++) {
		if (p.x >= hitboxes[i].pos.x && p.x <= (hitboxes[i].pos.x + hitboxes[i].width)) { // check if x coord is inside
			if (p.y >= hitboxes[i].pos.y && p.y <= (hitboxes[i].pos.y + hitboxes[i].height)) { // check if y coord is inside
				return true;
			}
		}
	}
	return false;
}
void DrawEntity(Entity& ent){
	DrawEntity(ent);
}
void DrawEntity(Entity& ent, int k){
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
			name = std::to_string(k) + "." + std::to_string(i) + "\n" + ent.id + "\nnodraw"; 
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

			if (DEBUG && k > 0){
				std::string name;
				name = std::to_string(k) + "." + std::to_string(i) + "\n" + ent.id; 
				DrawRectangleLines(ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, ent.hitboxes[i].width, ent.hitboxes[i].height, GREEN);
				DrawText(name.c_str(), ent.hitboxes[i].pos.x, ent.hitboxes[i].pos.y, 20, GREEN);
			}
	}
}
void Entity::DrawEntities() {
	for(auto& currentLayer : registeredLayers){
		for(Entity*& currentEntity : renderingPriority.at(currentLayer)){
			DrawEntity(*currentEntity);
		}
	}
}

Entity::Entity(){
	SetLayer(layer);
}
Entity::Entity(std::string idIn){
	id = idIn;
	SetLayer(layer);
}

void Entity::SetLayer(int newLayer){
	// linearly search for ourself in our old layer
	for(auto it = renderingPriority.at(layer).begin();;it++){
		if(*it == this){ // that's us!
			renderingPriority.at(layer).erase(it);
			break;
		}
	}

	// make sure our new layer is registered
	registeredLayers.insert(newLayer);

	// check if we already exist in the new layer
	for(auto it = renderingPriority.at(newLayer).begin();;it++){
		if(*it == this){ // kind of odd, but i guess we're done
			layer = newLayer; // make sure to set our layer
			return;
		}
	}
	// add ourself to the rendering priorities
	renderingPriority[newLayer].push_back(this);

	// set our layer
	layer = newLayer;
}

int Entity::GetLayer(){
	return layer;
}
Entity::~Entity(){
	// remove our spot in the rendering priorities
	for(auto it = renderingPriority.at(layer).begin();;it++){
		if(*it == this){
			renderingPriority.at(layer).erase(it);
			break;
		}
	}

	// if we were the only one in that layer, remove the layer too
	if(renderingPriority.at(layer).empty())
		renderingPriority.erase(layer);
}

void Entity::Update(){ }
void Entity::OnTouch(Entity&){ }
void Entity::OnUse(Entity&){ }

void Entity::SetScreenEntity(bool reset){
	if(reset){
		screenEntities.erase(this);
		return;
	}
	screenEntities.insert(this);
}

void Entity::ClearEntities(){
	for(Entity* currentEntity : screenEntities){
		delete currentEntity;
	}
	screenEntities.clear();
}
