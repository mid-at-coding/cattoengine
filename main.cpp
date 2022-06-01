#include <iostream>
#include "raylib.h"
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
	std::cout<<"test ";
	BackgroundScreen test;
	Entity testEntity;
	test.Init("test", 144);
	std::cout<<"1"<<std::endl;
	testEntity.hitboxes.push_back(EntityHitbox(50,50,1,50));
	std::cout<<"2"<<std::endl;
	testEntity.hitboxTexts.push_back(LoadTexture("galaxy.png"));
	std::cout<<"3"<<std::endl;
	testEntity.hitboxes[0].speedX = testEntity.hitboxes[0].speedY = 2;
	std::cout<<"4"<<std::endl;
	double counter;
	Color current = RED;
	counter = 0;
	while(!WindowShouldClose()){
		DT = GetFrameTime();
		test.Draw(WHITE);
		testEntity.Draw(current);
		testEntity.hitboxes[0].Move();
		counter += DT;
		if(counter >= 0.5){
			counter = 0;
			current = CycleRainbow(current);
		}
	}
}
