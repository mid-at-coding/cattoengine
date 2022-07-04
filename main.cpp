#include <iostream>
#include "include/raylib.h"
#include "engine.hpp"
#include "entity.hpp"
Color CycleRainbow(Color currentc){
	Color current = RED;
	if(ColorsEqual(currentc, RED)){ return ORANGE; }
	else if(ColorsEqual(currentc, ORANGE)) { return YELLOW; }
	else if(ColorsEqual(currentc, YELLOW)) { return GREEN; }
	else if(ColorsEqual(currentc, GREEN)) { return PURPLE; }
	else if(ColorsEqual(currentc, PURPLE)) { return RED; }
	return current;
}
int main(){
	BackgroundScreen test;
	Entity testEntity , testEntity2;
	double counter;
	Color current = RED;
	test.Init("test", 144);
	testEntity.hitboxes.push_back(EntityHitbox(0,0,400,400));
	testEntity2.hitboxes.push_back(EntityHitbox(50,50,400,400));
	testEntity.hitboxTexts.push_back(LoadTexture("galaxy.png"));
	testEntity2.hitboxTexts.push_back(LoadTexture("galaxy.png"));
	testEntity.hitboxes[0].speedX = testEntity.hitboxes[0].speedY = 40;
	testEntity2.hitboxes[0].speedX = testEntity2.hitboxes[0].speedY = 0;
	counter = 0;
	while (!WindowShouldClose()) {
		DT = GetFrameTime(); // keep this here
		test.Draw(testEntity.Colliding(testEntity2.hitboxes[0]) ? RED : WHITE);
		testEntity.Draw(current);
		testEntity2.Draw(CycleRainbow(CycleRainbow(current)));
		testEntity.hitboxes[0].Move();
		counter += DT;
		if(counter >= 0.5){
			counter = 0;
			current = CycleRainbow(current);
		}
	}
}
