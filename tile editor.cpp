#include <iostream>	
#include <fstream>	
#include "include/raylib.h"
#include "include/engine.hpp"
#include "include/entity.hpp"
#include "include/screen.hpp"
const int SIZE = 25;
struct text {
	Texture2D texture;
	std::string name;
};
void changeBrush(int x, std::vector<text>& textures,bool& Delete,bool& PlayerBrush, bool& InteractBrush,bool& BarrierBrush,Texture2D& Brush ) {
	Brush = textures[x].texture;
	Delete = false;
	PlayerBrush = false;
	InteractBrush = false;
	BarrierBrush = false;
	std::cout << textures[x].name << std::endl;
}
int main(){

	Screen Blank; // creates screen
	initEntityArr(); // empties array; clears garbage
	Blank.init("Shit Game"); // initializes screen
	Blank.backgroundTint = RED;
	std::ofstream file;
	std::ifstream fileRead;
	std::vector<text> textures;
	textures.push_back({LoadTexture("assets/white_top.png"),"assets/white_top.png"});
	textures.push_back({LoadTexture("assets/wood.png"),"assets/wood.png"});
	textures.push_back({LoadTexture("assets/stonebrick.png"),"assets/stonebrick.png"});
	textures.push_back({LoadTexture("assets/doorbottom.png"),"assets/doorbottom.png"});
	textures.push_back({LoadTexture("assets/doortop.png"),"assets/doortop.png"});
	textures.push_back({LoadTexture("assets/bedleft.png"),"assets/bedleft.png"});
	textures.push_back({LoadTexture("assets/bedright.png"),"assets/bedright.png"});
	Texture2D Character = LoadTexture("assets/character.png");
	Texture2D Brush = textures[0].texture;
	int playerx, playery;
	playerx = playery = 0;
	bool PlayerBrush = false;
	bool BarrierBrush = false;
	bool InteractBrush = false;
	int InteractCounter = 0;
	bool Delete = false;
	std::vector<Entity> Arr;
	Entity TileGrid[SIZE][SIZE];
	bool BarrierGrid[SIZE][SIZE];
	int InteractGrid[SIZE][SIZE];
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			TileGrid[x][y].addBox(EntityHitbox((DoublePoint) { x * SCREENX / SIZE , y * SCREENY / SIZE}, SCREENX / SIZE, SCREENY / SIZE));
			TileGrid[x][y].isTrigger(true);
			TileGrid[x][y].addTexture(textures[0].texture);
			BarrierGrid[x][y] = 0;
			InteractGrid[x][y] = 0;
		}
	}
	Entity Player;
	Player.addBox(EntityHitbox(0,0,16,16,0));
	Player.addTexture(Character);
	Player.isTrigger(false);
	//read from file
	fileRead.open("save.ce");
	if (fileRead.is_open()) {
		std::vector<std::string> data;
		bool flag = false;
		std::string currentLine;
		std::string currentNumber;
		int xcounter;
		EntityContainer empty;
		currentNumber = "0";
		if (fileRead.is_open()) {
			while (getline(fileRead, currentLine)) {
				data.push_back(currentLine);
			}
		}
		fileRead.close();
		// first line is the size of the grid
		if (SIZE == std::stoi(data[0])) {
			// 1 to (data.size() - (size + 1)) is all textures
			//(data.size() - (size + 1)) to data.size() - 2 is all tile data
			for (int y = (data.size() - (SIZE)); y < data.size() - 1; y++) {
				xcounter = 0;
				for (int x = 0; x < data[y].size(); x++) {
					if (data[y][x] == 'p') {
						playerx = xcounter;
						playery = (y - (data.size() - (SIZE)));
						Player.hitboxes(0)->pos = { TileGrid[xcounter][(y - (data.size() - (SIZE)))].hitboxes(0)->pos.x + (SCREENX / (2 * SIZE) - 8), TileGrid[xcounter][(y - (data.size() - (SIZE)))].hitboxes(0)->pos.y + (SCREENY / (2 * SIZE) - 8) }; // dont ask
					}
					else if (data[y][x] == '[') { // dont question it
						x++;
						while (data[y][x] != ']') {
							x++;
						}
					}
					else if (data[y][x] == 'b') {
						BarrierGrid[xcounter][(y - (data.size() - (SIZE)))] = true;
						TileGrid[xcounter][(y - (data.size() - (SIZE)))].tint(RED);
					}
					else if (data[y][x] == ',') {
						if (stoi(currentNumber)) {
							Brush = textures[stoi(currentNumber)].texture;
							TileGrid[xcounter][(y - (data.size() - (SIZE)))].isTrigger(false);
							(*TileGrid[xcounter][(y - (data.size() - (SIZE)))].hitboxTexts(0)) = Brush;
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
	}
	while (!WindowShouldClose()) {
		BeginDrawing();
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				for (int x = 0; x < SIZE; x++) {
					for (int y = 0; y < SIZE; y++) {
						if (TileGrid[x][y].Colliding((DoublePoint) { GetMouseX(), GetMouseY() })) {
							if (BarrierBrush && Delete) {
								BarrierGrid[x][y] = false;
								if (!InteractGrid[x][y]) {
									TileGrid[x][y].tint(WHITE);
								}
							}
							else if (InteractBrush && Delete) {
								if (InteractGrid[x][y]) {
									--InteractCounter;
								}
								InteractGrid[x][y] = 0;
								if (!BarrierGrid[x][y]) {
									TileGrid[x][y].tint(WHITE);
								}
							}
							else if (Delete) {
								TileGrid[x][y].isTrigger(true);
							}
							else if (PlayerBrush) {
								Player.hitboxes(0)->pos = { TileGrid[x][y].hitboxes(0)->pos.x + (SCREENX / (2 * SIZE) - 8), TileGrid[x][y].hitboxes(0)->pos.y + (SCREENY / (2 * SIZE) - 8) }; // dont ask
								playerx = x;
								playery = y;
							}
							else if (BarrierBrush) {
								InteractGrid[x][y] = 0;
								BarrierGrid[x][y] = true;
								TileGrid[x][y].tint(RED);
							}
							else if (InteractBrush) {
								BarrierGrid[x][y] = false;
								if (!InteractGrid[x][y]) {
									++InteractCounter;
								}
								std::cout << InteractCounter << std::endl;
								InteractGrid[x][y] = InteractCounter;
								TileGrid[x][y].tint(BLUE);
							}
							else {
								TileGrid[x][y].isTrigger(false);
								(*TileGrid[x][y].hitboxTexts(0)) = Brush;
							}
						}
					}
				}
			}
			if (IsKeyPressed(KEY_ONE)) {
				changeBrush(0, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_TWO)) {
				changeBrush(1, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_THREE)) {
				changeBrush(2, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_FOUR)) {
				changeBrush(3, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_FIVE)) {
				changeBrush(4, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_SIX)) {
				changeBrush(5, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_SEVEN)) {
				changeBrush(6, textures, Delete, PlayerBrush, InteractBrush, BarrierBrush, Brush);
			}
			if (IsKeyPressed(KEY_I)) {
				InteractBrush = !InteractBrush;
				PlayerBrush = false;
				std::cout << "Interact Brush " << InteractBrush << std::endl;
			}
			if (IsKeyPressed(KEY_P)) {
				PlayerBrush = !PlayerBrush;
				std::cout << "Player Selection " << PlayerBrush << std::endl;
			}
			if (IsKeyPressed(KEY_D)) {
				Delete = !Delete;
				std::cout << "Deletion " << Delete << std::endl;
			}
			if (IsKeyPressed(KEY_B)) {
				BarrierBrush = !BarrierBrush;
				std::cout << "Barrier Brush " << BarrierBrush << std::endl;
			}
			if (IsKeyPressed(KEY_S)) {
				// save file
				remove("save.ce");
				file.open("save.ce");
				file << SIZE << std::endl;
				for (int i = 0; i < textures.size(); i++) {
					file << textures[i].name << std::endl;
				}
				for (int y = 0; y < SIZE; y++) {
					for (int x = 0; x < SIZE; x++) {
						if (x == playerx && y == playery) {
							file << "p";
						}
						if (BarrierGrid[x][y]) {
							file << "b";
						}
						if (InteractGrid[x][y]) {
							file << "[" << InteractGrid[x][y] << "]";
						}
						if (TileGrid[x][y].isTrigger() == true) {
							file << "0,";
						}
						else {
							for (int i = 0; i < textures.size(); i++) {
								if ((*TileGrid[x][y].hitboxTexts(0)).id == textures[i].texture.id) {
									file << i << ",";
								}
							}
						}
					}
					file << "\n";
				}
				file.close();
			}
			Blank.Draw();
			DrawEntities();
			for (int x = 0; x < SIZE; x++) {
				for (int y = 0; y < SIZE; y++) {
					if (TileGrid[x][y].Colliding((DoublePoint) { GetMouseX(), GetMouseY() })) {
						DrawText((std::to_string(x) + " , " + std::to_string(y)).c_str(), 0, 0, 50, WHITE);
					}
				}
			}
		EndDrawing();
	}
}
