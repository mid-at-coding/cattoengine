#ifndef engine_h
#define engine_h
#include <string>
#include <iostream>
#include "raylib.h"
// Macros / global variables = SNAKE_CASE_CAPS
// Functions / classes = PascalCase
// Variables = camelCase
#define SCREENX 800
#define SCREENY 800
#define ENTITY_MAX 2000
#define DEBUG false
#define EMAX_CRASH false
struct Point { int x; int y; };
struct DoublePoint { double x; double y; };
class BackgroundScreen {
public:
	bool backgroundIsText;
	Texture2D background;
	Color backgroundTint = WHITE;
	void Draw();
	BackgroundScreen(std::string backgroundImage = "\n");
	void Init(std::string windowName, int fps = 0, std::string backgroundImage = "\n");
};

Point DoublePointToPoint(DoublePoint d);
DoublePoint PointToDoublePoint(Point p);
bool ColorsEqual(Color c1, Color c2);

#endif
	
