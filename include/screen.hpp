#ifndef screen_h
#define screen_h
#include "entity.hpp"
class Screen : public BackgroundScreen {
	int DataStart(std::vector<std::string> &data, int size);
	std::vector<std::string> ReadIntoVector();
	std::vector<Texture2D> GetTexts(int datastart, const std::vector<std::string> &data);
	void ParseEntity(const std::string in, const DoublePoint pos, int& n, const std::vector<Texture2D> &expectedTexts);
public:
	std::vector<Entity*> entities;
	std::vector<int> barriers;
	double tileSizeX = 1;
	double tileSizeY = 1;
	int size = 1;
	int width = 1;
	int height = 1;
	std::string fileName;
	Screen(void) {}
	Screen(Texture2D backin, std::vector<Entity*> entin);
	void Load(void);
	void ReadFromFile(std::string fileName, DoublePoint player = { -1,-1 });
	bool CheckPoint(const Point to);
	bool CheckEntity(const Entity& to);
	bool CheckHitbox(const EntityHitbox& to);
};

#endif
