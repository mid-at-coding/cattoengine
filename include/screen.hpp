#ifndef screen_h
#define screen_h
#include "engine.hpp"
#include "entity.hpp"
class Screen : public BackgroundScreen {
public : 
	std::vector<EntityContainer> entities;
	Screen() {}
	Screen(Texture2D backin, std::vector<EntityContainer> entin) {
		background = backin;
		entities = entin;
	}
	void load() {
		clearEntities();
		if (DEBUG)
			std::cout << "Screen Cleared \n";

		for (int i = 0; i < entities.size(); i++) {
			LOADED_ENTITIES[i] = entities[i];
		}
		LOADED_ENTITIES_HEAD = entities.size();
		if (DEBUG) {
			std::cout << "Entities Added to Entity Array \n";
			std::cout << "EArr Head : " << LOADED_ENTITIES_HEAD << std::endl;
		}
	}
};
#endif