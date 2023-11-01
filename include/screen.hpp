#ifndef screen_h
#define screen_h
#include "entity.hpp"
#include <fstream>
#include <optional>
class Screen : public BackgroundScreen {
	int DataStart(std::vector<std::string> &data, int size);
	std::vector<std::string> ReadIntoVector();
	std::vector<Texture2D> GetTexts(int datastart, const std::vector<std::string> &data);
	void ParseEntity(const std::string in, const DoublePoint pos, int& n, const std::vector<Texture2D> &expectedTexts);
public:
	std::vector<EntityContainer> entities;
	std::vector<int> barriers;
	double tileSizeX = 1;
	double tileSizeY = 1;
	int size = 1;
	int width = 1;
	int height = 1;
	std::string fileName;
	Screen(void) {}
	Screen(Texture2D backin, std::vector<EntityContainer> entin);
	void Load(void);
	void ReadFromFile(std::string fileName, DoublePoint player = { -1,-1 });
	bool CheckMove(const Point to);
	bool CheckMove(const EntityContainer& to);
	bool CheckMove(const EntityHitbox& to);
};

std::optional<int> FindTrigger(int find);
std::optional<std::vector<int>> FindTriggerV(int find);
std::optional<std::vector<int>> TriggerCollision(const Entity& Ent);
std::optional<std::vector<int>> TriggerCollision(const EntityContainer& Player);
#endif
