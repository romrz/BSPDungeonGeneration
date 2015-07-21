#include "TileMap.h"

TileMap::TileMap(sf::Vector2i& size, sf::Vector2i& tileSize, char* map, Tile* tileset) {
	mSize = size;
	mTileSize = tileSize;
	mMap = map;
	mTileset = tileset;
	clear();
}

TileMap::TileMap(sf::Vector2i& size, sf::Vector2i& tileSize, Tile* tileset) {
	mSize = size;
	mTileSize = tileSize;
	mMap = new char[mSize.x*mSize.y];
	mTileset = tileset;
	clear();
}

void TileMap::clear() {
	int n = mSize.x*mSize.y;
	for (int i = 0; i < n; i++)
		mMap[i] = 1;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Tile* tile;
	for (int y = 0; y < mSize.y; y++)
		for (int x = 0; x < mSize.x; x++) {
			tile = &mTileset[getTile(x, y)];
			tile->setPosition(x*mTileSize.x, y*mTileSize.y);
			target.draw(*tile);
		}
}