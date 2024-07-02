#include "screen.hpp"
#include <string>
#include <fstream>
#include <iostream>
static bool bracesMatched(std::string st){
	int total = 0;
	for(int i = 0 ; i < st.size(); i++){
		if(st[i] == '{') {
			total += 1;
		}
		if(st[i] == '}') {
			total -= 1;
		}
	}
	if(total == 0)
		return true;
	return false;
}
static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}
Screen::Screen(Texture2D backin, std::vector<Entity*> entin) {
		background = backin;
		entities = entin;
}
void Screen::Load() {
	Entity::ClearEntities();
	if (DEBUG)
		std::cout << "Screen Cleared \n";
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->SetScreenEntity();
	}
	if (DEBUG) {
		std::cout << "Entities Added to Entity Array \n";
	}
}
int Screen::DataStart(std::vector<std::string> &data, int size){
		// find the start of the data
		for(int i = 0; i < data.size(); i++){
			if(data[i] == "DATA"){
				std::cout << "DATA :" << i - 1 << std::endl;
				return i;
			}
		}
		// if it wasn't found default to assuming the map's height = size
		return (data.size() - (size + 1));
}
void Screen::ParseEntity(const std::string in, const DoublePoint pos, int& n, const std::vector<Texture2D> &expectedTexts){
	std::string local_in = in;
	bool flag_player = false;
	bool flag_barrier = false;
	bool flag_invis = false;
	bool flag_anim = false;
	bool flag_sign = false;
	bool flag_id = false;
	bool flag_behind = false;
	std::string id = "entity_generic";
	int behind_pos = 0;
	std::string behind_str = "";
	std::string signText = "";
	std::string textureNum = "";
	for(int i= 0; i < in.size(); i++){
		if(flag_behind)
			break;
		switch (in[i]){
			case 'p' :
				flag_player = true;
				break;
			case 'b' : 
				flag_barrier = true;
				break;
			case 'i' :
				flag_invis = true;
				break;
			case 'a' :
				flag_anim = true;
				break;
			case '{' :
			    i++;
				for(std::string num; in[i] != '}'; i++){
					num += in[i];
					signText = num;
				}
				flag_sign = true; 
				break;
			case '[' :
				i++;
				id = "";
				for(std::string num; in[i] != ']'; i++){
					id += in[i];
				}
	 			flag_id = true;
				break;
			case 'B' :
				flag_behind = true;
				i++;
				for(; i < in.size(); i++)
					behind_str += in[i];
				break;
			default : 
				textureNum += in[i];
				break;
		}
	}
	if(flag_player){
		// TODO
	}
	if(flag_sign)
		ReplaceStringInPlace(signText,"\\n","\n");
	if(!flag_barrier && !flag_invis && !flag_anim && !flag_sign && !flag_id && (textureNum == "0" || textureNum == "00")){
		n--;
		return; // no reason to make an entity
	}

	// make an entity
	Entity* ent = new Entity;
	ent->dontDraw = flag_invis;
	ent->anim = flag_anim;
	ent->signText = signText;
	ent->id = id;
	ent->hitboxes.push_back(EntityHitbox(pos, tileSizeX, tileSizeY));
	ent->hitboxTexts.push_back(expectedTexts[std::stoi(textureNum)]);

	if(flag_barrier)
		barriers.push_back(n);

	entities.push_back(ent);
	if(flag_behind){
		ParseEntity(behind_str, pos, ++n ,expectedTexts);
	}
}
std::vector<std::string> Screen::ReadIntoVector(){
	std::ifstream file;
	file.open(fileName);
	std::vector<std::string> data;
	std::string currentLine;
	if (file.is_open()) {
		while (getline(file, currentLine)) {
			while (currentLine.find("\r") != std::string::npos) {
				currentLine.erase(currentLine.find("\r"), 1);
			}
			data.push_back(currentLine);
		}
	}
	file.close();
	return data;
}
std::vector<Texture2D> Screen::GetTexts(int datastart, const std::vector<std::string> &data){
		static std::vector<Texture2D> texts;
		static std::vector<std::string> loadedTexts;
		std::vector<Texture2D> expectedTexts;
		bool textureCached = false;
		for (size_t i = 1; i <= datastart; i++) {
			for (size_t k = 0; k < loadedTexts.size(); k++) {
				if (loadedTexts[k] == data[i]) {
					textureCached = true;
					break;
				}
			}
			if (!textureCached) {
				loadedTexts.push_back(data[i]);
				texts.push_back(LoadTexture(data[i].c_str()));
			}
			textureCached = false;
		}
		// add cached textures
		for (size_t i = 1; i <= (data.size() - (size + 1)); i++) {
			for (size_t k = 0; k < loadedTexts.size(); k++) {
				if (loadedTexts[k] == data[i]) {
					expectedTexts.push_back(texts[k]);
				}
			}
		}
		return expectedTexts;
}
void Screen::ReadFromFile(std::string fileName, DoublePoint player) {
		(*this).fileName = fileName;
		std::vector<std::string> data = ReadIntoVector();
		std::vector<Texture2D> expectedTexts;
		int entityCounter = entities.size() - 1;
		int xcounter = 0;
		barriers.clear();
		Entity::ClearEntities();
		int datastart = -1;
		std::string currentPiece = "";
		int size;

		size = std::stoi(data[0]); // first line is the size of the grid
		datastart = DataStart(data, size);
		this->height = (data.size() - 1) - (datastart + 1);
		this->size = size;
		tileSizeX = (float)SCREENY / size;
		tileSizeY = (float)SCREENY / size;
		if(SQUARE_TILES){
			tileSizeX = tileSizeY;
		}
		expectedTexts = GetTexts(datastart, data);		
		// datastart to data.size() - 1 is all tile data
		for (size_t y = datastart + 1; y < data.size(); y++) {
			xcounter = 0;
			for (size_t x = 0; x < data[y].size(); x++) {
				if(data[y][x] == ',' && bracesMatched(currentPiece)){
					ParseEntity(currentPiece,{
							static_cast<double>(xcounter++ * SCREENY / size), 
							static_cast<double>((y - datastart - 1)* SCREENY / size) }
							, ++entityCounter, expectedTexts);
					currentPiece = "";
				}
				else
					currentPiece += data[y][x];
			}
			if (y > datastart + 1 && xcounter != width && DEBUG){
				std::cout << "width mismatch on line " << y << "!\n";
			}
			width = xcounter;
		}
		if(DEBUG){
			std::cout << "at end of reading : " << std::endl;
			std::cout << "		barriers : ";
			for(int i = 0; i < barriers.size(); i++){
				std::cout << barriers[i] << " , ";
			}
		}
}
bool Screen::CheckPoint(const Point to) {
		for (size_t i = 0; i < barriers.size(); i++) {
			if (entities[barriers[i]]->Colliding(to)) {
				return false;
			}
		}
		return true;
	}
bool Screen::CheckEntity(const Entity& to) {
		for (size_t i = 0; i < barriers.size(); i++) {
			if (entities[barriers[i]]->Colliding(to)) {
				return false;
			}
		}
		return true;
	}
bool Screen::CheckHitbox(const EntityHitbox& to) {
	for (size_t i = 0; i < barriers.size(); i++) {
		if (entities[barriers[i]]->Colliding(to)) {
			return false;
		}
	}
	return true;
}
