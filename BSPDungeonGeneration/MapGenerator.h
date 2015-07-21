#include <cstdlib>
#include "TileMap.h"

inline int random(int min, int max) { return rand() % (max + 1 - min) + min; }

class Area {
public:
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	Area() {}
	Area(int ax, int ay, int aw, int ah) : x(ax), y(ay), width(aw), height(ah) {}
	~Area() { delete room; }

	void setRoom(int rx, int ry, int rw, int rh) { room = new Area(rx, ry, rw, rh); }
	Area getRoom() { return *room; }
private:
	Area* room = nullptr;
};

class Node {
public:
	Node* left = nullptr;
	Node* right = nullptr;
	Area area;

	Node() {}
	Node(Area a) { area = a; }
	~Node() { delete left; delete right; }

	void setLeft(Node* node) { delete left; left = node; }
	void setRight(Node* node) { delete right; right = node; }
	bool isLeaf() { return left == nullptr && right == nullptr; }
};

class MapGenerator {
public:
	MapGenerator(sf::Vector2i& size, sf::Vector2i& tileSize, Tile* tileset);
	~MapGenerator() { delete mTree; }
	
	void setMinRoomSize(sf::Vector2i& size) { mMinSize = size; }
	void setIterations(int i) { mIterations = i; }
	void setSeed(int s) { srand(s); }

	TileMap* generate();
protected:
	TileMap* mLevel = nullptr;
	Node* mTree = nullptr;
	sf::Vector2i mMinSize = sf::Vector2i(1, 1);
	int mIterations = -1;

	void bsp(Node* node, int i);
	void createRoom(Node* node);
	Area createCorridors(Node* node);
	void createCorridor(int x1, int y1, int x2, int y2);
	bool split(Area& area, Area &a1, Area& a2);
};