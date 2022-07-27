#include <iostream>
#include "raylib.h"
#include "engine.hpp"
#include "entity.hpp"
#include "screen.hpp"
#define SPEED 500
void CheckMovement(Entity & Player) {
	bool moveUp = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
	bool moveDown = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
	bool moveLeft = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
	bool moveRight = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
	if (moveUp) {
		if (moveRight || moveLeft) {
			Player.hitboxes(0)->pos.y -= DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes(0)->pos.y -= DT * SPEED;
		}
	}
	if (moveDown) {
		if (moveRight || moveLeft) {
			Player.hitboxes(0)->pos.y += DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes(0)->pos.y += DT * SPEED;
		}
	}
	if (moveLeft) {
		if (moveUp || moveDown) {
			Player.hitboxes(0)->pos.x -= DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes(0)->pos.x -= DT * SPEED;
		}
	}
	if (moveRight) {
		if (moveUp || moveDown) {
			Player.hitboxes(0)->pos.x += DT * SPEED * (sin(45));
		}
		else {
			Player.hitboxes(0)->pos.x += DT * SPEED;
		}
	}
}
void Bullet(DoublePoint to, DoublePoint from, Texture2D text, int width = 32, int height = 32, double speed = 1) {
	static std::vector<Entity> bullets;
	bool flag = false;
	EntityContainer* current;
	for (int i = 0; i < bullets.size(); i++) { // check if we can reuse any OoB bullets for performance
		if (bullets[i].OutOfBounds()) {
			current = bullets[i].ent;
			flag = true;
			break;
		}
	}
	if (!flag) {
		Entity bulletEnt;
		current = bulletEnt.ent;
		bullets.push_back(bulletEnt);
		current->hitboxes.push_back(EntityHitbox(from, width, height));
		current->hitboxTexts.push_back(text);
	}
	current->isTrigger = false;
	current->hitboxTexts[0] = text;
	current->hitboxes[0].width = width;
	current->hitboxes[0].height = height;
	current->tint = RED;
	current->hitboxes[0].speedX = (to.x - from.x) * speed;
	current->hitboxes[0].speedY = (to.y - from.y) * speed;
	current->hitboxes[0].pos = from;
}
bool isInsideCircle(int circle_x, int circle_y,double rad, int x, int y)
{
	// Compare radius of circle with distance
	// of its center from given point
	if ((x - circle_x) * (x - circle_x) +
		(y - circle_y) * (y - circle_y) <= rad * rad)
		return true;
	else
		return false;
} // adapted from https://www.geeksforgeeks.org/find-if-a-point-lies-inside-or-on-circle/ because im too lazy to do it myself
Point GenRandomPointNotOnPlayer(Entity& Player, double radius) {
	int x, y;
	x = y = 0;
	x = rand() % SCREENX;
	y = rand() % SCREENY;
	while (isInsideCircle(Player.hitboxes(0)->pos.x, Player.hitboxes(0)->pos.y, radius, x, y)) {
		x = rand() % SCREENX;
		y = rand() % SCREENY;
	}
	std::cout<<" Generated random points : " << x << " " << y << "\n";
	return ((Point) { x, y });
}
bool CheckLoseState(Entity& Player) {
	for (int i = 1; i < LOADED_ENTITIES_HEAD; i++) {
		if (Player.Colliding(LOADED_ENTITIES[i])) {
			return true;
		}
	}
	return false;
}
void Sweep(Texture2D text, int n = 5,int size = 64) {
	for (int i = 1; i <= n; i++) {
		Bullet(
			{SCREENX,SCREENY * (i) / (n+1) - (size / 2) },
			{1, SCREENY * (i)/(n+1) - (size / 2) },
			text,
			size,size,
			0.5
		);
		std::cout << "Sweep at " << SCREENY * (i) / (n + 1) - ( size / 2) << std::endl;
	}
}
int main(){

	Screen test; // creates screen
	initEntityArr(); // empties array; clears garbage
	Entity Player; // creates two entities; Player, and Copy. Constructer is run, all data is (ideally) in the array
	test.Init("i am so good at programming"); // initializes screen with refresh rate of current monitor
	Texture2D smallBall = LoadTexture("assets/small_ball.png"); // Loads texture
	Texture2D ball = LoadTexture("assets/ball.png"); // ^^
	double time, homingCounter, sweepCounter;
	bool cleared = false;
	bool started = false;
	Player.addTexture(ball); // sets the texture for hitbox one(zero) of the player entity
	Player.tint(BLUE); // sets the tint for the player as blue
	Player.addBox(EntityHitbox(368, 368, 64, 64)); // makes a new hitbox at 368,368 with width and height of 64
	Player.isTrigger(false); // ensures player is drawn
	Player.hitboxes(0)->speedX;
	test.entities.push_back(*Player.ent);
	test.background = LoadTexture("assets/background.png");
	test.backgroundIsText = true;
	InitAudioDevice();
	while (!IsAudioDeviceReady()) {}
	Sound backgroundMusic = LoadSound("assets/soundtrack.mp3");
	DT = 0;
	PlaySound(backgroundMusic);
	while (!WindowShouldClose()) {
		BeginDrawing();
			DT = GetFrameTime(); // keep this here
			time += DT;
			homingCounter += DT;
			sweepCounter += DT;
			if (Player.OutOfBounds()|| CheckLoseState(Player)) {
				test.Load(); // resets screen
				StopSound(backgroundMusic);
				PlaySound(backgroundMusic);
				time = 0;
				homingCounter = 0;
				sweepCounter = 0;
			}
			if (homingCounter >= 0.5 && time <= 14) {
				Bullet(Player.hitboxes(0)->pos, PointToDoublePoint(GenRandomPointNotOnPlayer(Player, 200)), smallBall,32,32,0.5);
				// ^^ creates a bullet that goes towards the player at a random point 200 units or more from the player
				homingCounter = 0;
			}
			if (time >= 28 && homingCounter >= 0.5 && time <= 42) {
				Bullet(Player.hitboxes(0)->pos, PointToDoublePoint(GenRandomPointNotOnPlayer(Player, 200)), smallBall, 32, 32, 0.5);
				homingCounter = 0;
			}
			if (time >= 43 && homingCounter >= 0.5 && time <= 58) {
				Bullet(Player.hitboxes(0)->pos, PointToDoublePoint(GenRandomPointNotOnPlayer(Player, 200)), smallBall, 32, 32, 0.7);
				homingCounter = 0;
			}
			if (time >= 4 && sweepCounter >= 0.5 && time <= 12) {
				Sweep(ball,1);
				sweepCounter = 0;
			}
			if (time >= 12 && sweepCounter >= 0.1 && time <= 14) {
				Sweep(ball, (3));
				sweepCounter = 0;
			}
			if (time >= 14 && sweepCounter >= 2 && time <= 20) {
				Sweep(ball, (3));
				sweepCounter = 0;
			}
			if (time >= 20 && sweepCounter >= 2 && time <= 42) {
				Sweep(ball, (1));
				sweepCounter = 0;
			}
			if (time >= 42 && time <= 43) {
				if (!cleared) {
					clearEntitiesExceptFirst();
					cleared = true;
				}
			}
			test.Draw(); // draws background
			MoveEntities();
			DrawEntities();
			CheckMovement(Player);
		EndDrawing();
	}
}
