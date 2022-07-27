#include <iostream>	
#include <fstream>	
#include "include/raylib.h"
#include "include/engine.hpp"
#include "include/entity.hpp"
#include "include/screen.hpp"
#define SIZE 32
int main(){

	Screen Blank; // creates screen
	initEntityArr(); // empties array; clears garbage
	Blank.init("Shit Game"); // initializes screen
	Blank.backgroundTint = RED;
	std::ofstream file;
	Texture2D Block = LoadTexture("assets/white_top.png");
	Texture2D Wood = LoadTexture("assets/wood.png");
	Texture2D Brush = Wood;
	bool Delete = false;
	std::vector<Entity> Arr;
	Entity TileGrid[SIZE][SIZE];
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			TileGrid[x][y].addBox(EntityHitbox((DoublePoint) { x * SCREENX / SIZE , y * SCREENY / SIZE}, SCREENX / SIZE, SCREENY / SIZE));
			TileGrid[x][y].isTrigger(true);
			TileGrid[x][y].addTexture(Wood);
		}
	}
	while (!WindowShouldClose()) {
		BeginDrawing();
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				for (int x = 0; x < SIZE; x++) {
					for (int y = 0; y < SIZE; y++) {
						if (TileGrid[x][y].Colliding((DoublePoint) { GetMouseX(), GetMouseY() })) {
							if (Delete) {
								TileGrid[x][y].isTrigger(true);
							}
							else {
								TileGrid[x][y].isTrigger(false);
								(*TileGrid[x][y].hitboxTexts(0)) = Brush;
							}
						}
					}
				}
			}
			if (IsKeyDown(KEY_ONE)) {
				Brush = Block;
			}
			if (IsKeyDown(KEY_TWO)) {
				Brush = Wood;
			}
			if (IsKeyDown(KEY_D)) {
				Delete = !Delete;
			}
			if (IsKeyDown(KEY_S)) {
				// save file
				remove("save.ce");
				file.open("save.ce");
				file << SIZE << std::endl;
				file << "assets/wood.png" << std::endl;
				file << "assets/white_top.png" << std::endl;
				for (int y = 0; y < SIZE; y++) {
					for (int x = 0; x < SIZE; x++) {
						if (TileGrid[x][y].isTrigger() == true) {
							file << "0,";
						}
						else if (TileGrid[x][y].hitboxTexts(0)->id == Wood.id) {
							file << "1,";
						}
						else if (TileGrid[x][y].hitboxTexts(0)->id == Block.id) {
							file << "2,";
						}
					}
					file << std::endl;
				}
				file.close();
			}
			Blank.Draw();
			DrawEntities();
		EndDrawing();
	}
}
