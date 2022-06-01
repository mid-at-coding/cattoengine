#ifndef engine_h
#define engine_h
#include <string>
// Defines main functions, object classes & variables
// Macros / global variables = SNAKE_CASE_CAPS
// Functions / classes = PascalCase
// Variables = camelCase
double DT = 0;
#define CELLSX 100
#define CELLSY 100
#define SCREENX 800
#define SCREENY 800
#define CELLSIZEX (SCREENX/CELLSX)
#define CELLSIZEY (SCREENY/CELLSY)
#define ENTITY_MAX 20
#define DEBUG true
class Point{
public :
	int x = 0;
	int y = 0;
};
bool ColorsEqual(Color c1, Color c2){
	if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b) {
//	std::cout<<"colours are equal"<<std::endl;
	return true;
	}
	return false;
}
class BackgroundScreen{
	bool barrier[CELLSX][CELLSY];
	Texture2D background;
	bool backgroundIsText;
public :
	void Init(std::string windowName, int fps = 0, std::string backgroundImage = "none"){
		std::cout<<std::endl<<"Starting Initialization"<<std::endl;
		InitWindow(SCREENX, SCREENY, windowName.c_str());
		if(fps){
			SetTargetFPS(fps);
		}
		backgroundIsText = false;// Texture, not literal text
		if (backgroundImage != "none"){
			background = LoadTexture(backgroundImage.c_str());
			backgroundIsText = true;
		}
		std::cout<<std::endl<<"Initialization Finished"<<std::endl;
	}
	void Draw(Color backgroundTint = WHITE){
		BeginDrawing();
			if(backgroundIsText){
				DrawTexture(background, 0, 0, backgroundTint);  
			}
			else{
				ClearBackground(backgroundTint);
			}
		EndDrawing();
	}
};

#endif
	
