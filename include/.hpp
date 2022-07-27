#ifndef screen_h
#define screen_h
#include "engine.hpp"
#include "entity.hpp"
#include <fstream>
class Screen : public BackgroundScreen {
public : 
	std::vector<EntityContainer> entities;
	Screen() {}
	Screen(Texture2D backin, std::vector<EntityContainer> entin) {
		background = backin;
		entities = entin;
	}
	void Load() {
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
	void ReadFromFile(std::string fileName) {
		std::ifstream file;
		file.open(fileName);
		std::vector<std::string> data;
		std::vector<Texture2D> texts;
		std::string currentLine;
		std::string currentNumber;
		int xcounter;
		EntityContainer empty;
		currentNumber = "0";
		int size;
		if (file.is_open()) {
			while (getline(file, currentLine)) {
				data.push_back(currentLine);
			}
		}
		file.close();
		size = std::stoi(data[0]); // first line is the size of the grid
		// 1 to (data.size() - (size + 1)) is all textures
		for (int i = 1; i <= (data.size() - (size + 1)); i++) {
			texts.push_back(LoadTexture(data[i].c_str()));
		}
		//(data.size() - (size + 1)) to data.size() - 2 is all tile data
		for (int y = (data.size() - (size)); y < data.size() - 1; y++) {
			xcounter = 0;
			for (int x = 0; x < data[y].size(); x++) {
				if (data[y][x] == ',') {
					if (stoi(currentNumber)) {
						empty.hitboxTexts.clear();
						empty.hitboxes.clear();
						empty.hitboxTexts.push_back(texts[stoi(currentNumber) - 1]);
						empty.hitboxes.push_back(EntityHitbox((DoublePoint) { xcounter * SCREENX / size, y * SCREENY / size }, SCREENX / size, SCREENY / size));
						entities.push_back(empty);
					}
					currentNumber = " ";
					xcounter++;
				}
				else {
					currentNumber += data[y][x];
				}
			}
		}
	}
};
#endif
