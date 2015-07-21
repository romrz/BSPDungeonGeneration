#include "MapGenerator.h"

MapGenerator::MapGenerator(sf::Vector2i& size, sf::Vector2i& tileSize, Tile* tileset) {
	mLevel = new TileMap(size, tileSize, tileset);
}

TileMap* MapGenerator::generate() {
	mLevel->clear();
	delete mTree;
	mTree = new Node(Area(0, 0, mLevel->getSize().x, mLevel->getSize().y));
	bsp(mTree, mIterations);
	createCorridors(mTree);
	return mLevel;
}

void MapGenerator::bsp(Node* node, int i) {
	Area a1, a2;
	if (i != 0 && split(node->area, a1, a2)) {
		node->setLeft(new Node(a1));
		node->setRight(new Node(a2));
		bsp(node->left, --i);
		bsp(node->right, i);
	}
	else
		createRoom(node);
}

void MapGenerator::createRoom(Node* node) {
	int w = random(mMinSize.x, node->area.width - 2);
	int h = random(mMinSize.y, node->area.height - 2);
	int x = random(node->area.x + 1, node->area.x + node->area.width - w - 1);
	int y = random(node->area.y + 1, node->area.y + node->area.height - h - 1);
	node->area.setRoom(x, y, w, h);
	for (int i = y; i < y + h; i++)
		for (int j = x; j < x + w; j++)
			mLevel->setTile(0, j, i);
}

Area MapGenerator::createCorridors(Node* node) {
	if (node->isLeaf())
		return node->area.getRoom();
	Area a1 = createCorridors(node->left);
	Area a2 = createCorridors(node->right);
	createCorridor(
		a1.x + a1.width / 2,
		a1.y + a1.height / 2,
		a2.x + a2.width / 2,
		a2.y + a2.height / 2
	);
	return a1;
}

void MapGenerator::createCorridor(int x1, int y1, int x2, int y2) {
	int stepx = (x2 - x1) > 0 ? 1 : -1;
	int stepy = (y2 - y1) > 0 ? 1 : -1;
	int h = abs(x2 - x1), v = abs(y2 - y1), x = x1, y = y1;
	for (int i = 0; i < h; i++, x += stepx) {
		mLevel->setTile(0, x, y1);
		mLevel->setTile(0, x, y1 + 1);
	}
	for (int i = 0; i < v; i++, y += stepy) {
		mLevel->setTile(0, x2, y);
		mLevel->setTile(0, x2 + 1, y);
	}
}

bool MapGenerator::split(Area& a, Area &a1, Area& a2) {
	int splitSide = random(1, 2);
	if (splitSide == 1) {
		if (a.width < 2 * mMinSize.x + 4) return false;
		a1 = Area(a.x, a.y, random(mMinSize.x + 2, a.width - mMinSize.x - 2), a.height);
		a2 = Area(a1.x + a1.width, a.y, a.width - a1.width, a.height);
	}
	else if (splitSide == 2) {
		if (a.height < 2 * mMinSize.y + 4) return false;
		a1 = Area(a.x, a.y, a.width, random(mMinSize.y + 2, a.height - mMinSize.y - 2));
		a2 = Area(a.x, a1.y + a1.height, a.width, a.height - a1.height);
	}
	else return false;
	return true;
}