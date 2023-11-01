#include "engine.hpp"
#include <cmath>
// Macros / global variables = SNAKE_CASE_CAPS
// Functions / classes = PascalCase
// Variables = camelCase
Point DoublePointToPoint(DoublePoint d){
	Point ret;
	ret.x = d.x;
	ret.y = d.y;
	return ret;
}
DoublePoint PointToDoublePoint(Point p){
	DoublePoint ret;
	ret.x = p.x;
	ret.y = p.y;
	return ret;
}
bool ColorsEqual(Color c1, Color c2){
	if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b) {
//	std::cout<<"colours are equal"<<std::endl;
	return true;
	}
	return false;
}
void BackgroundScreen::Draw(){
	if((*this).backgroundIsText){
		if (anim){
			time += GetFrameTime();
			if(time > (double)(frames) / (double)animFps){
				time = 0;
				currentFrame = 0;
				animDone = true;
			}
			currentFrame = std::floor(animFps * time);
		}
		ClearBackground(backgroundTint);
		DrawTextureRec(background, { 0,0,background.width,background.height }, 
		{(int)offset.x + (anim ? animOffset.x * currentFrame : 0 ),
		 (int)offset.y + (anim ? animOffset.y * currentFrame : 0 )},WHITE);
	}
	else{
		ClearBackground(backgroundTint);
	}
}
BackgroundScreen::BackgroundScreen(std::string backgroundImage) {
	backgroundIsText = false;// Texture, not literal text
	if (backgroundImage != "\n") {
		background = LoadTexture(backgroundImage.c_str());
		backgroundIsText = true;
	}
}
void BackgroundScreen::Init(std::string windowName, int fps, std::string backgroundImage) {
	std::cout << std::endl << "Starting Initialization" << std::endl;
	InitWindow(SCREENX, SCREENY, windowName.c_str());
	if (fps) {
		SetTargetFPS(fps);
	}
	backgroundIsText = false;// Texture, not literal text
	if (backgroundImage != "\n") {
		background = LoadTexture(backgroundImage.c_str());
		backgroundIsText = true;
	}
	std::cout << std::endl << "Initialization Finished" << std::endl;
}
