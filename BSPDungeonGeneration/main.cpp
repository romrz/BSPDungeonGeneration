#include "MapGenerator.h"
#include <iostream>

void processInput(sf::RenderWindow& window);

bool generateLevel = true;

int main() {
	sf::Vector2i levelSize(30, 20);
	sf::Vector2i tileSize(32, 32);
	sf::RenderWindow window(sf::VideoMode(levelSize.x * tileSize.x, levelSize.y * tileSize.y), "Dungeon Generation using BSP");
	window.setVerticalSyncEnabled(true);

	sf::Texture texture;
	if (!texture.loadFromFile("tileset.png")) {
		std::cout << "Unable to load the texture" << std::endl;
		return 0;
	}
	Tile tileset[] = {
		Tile(texture, sf::IntRect(0, 0, tileSize.x, tileSize.y)),
		Tile(texture, sf::IntRect(tileSize.x, 0, tileSize.x, tileSize.y))
	};

	MapGenerator generator(levelSize, tileSize, tileset);
	generator.setMinRoomSize(sf::Vector2i(4, 4));
	
	sf::Font font;
	if (!font.loadFromFile("LCD_Solid.TTF")) {
		std::cout << "Unable to load the font" << std::endl;
		return 0;
	}
	sf::String str(" FPS\n\nPRESS 'G' TO GENERATE A NEW DUNGEON");
	sf::Text text(str, font, 10);
	text.setPosition(10, 10);
	text.setColor(sf::Color::White);

	TileMap* tileMap = nullptr;
	int frameCount = 0;
	sf::Clock clock;
	while (window.isOpen()) {
		if (clock.getElapsedTime().asMilliseconds() >= 1000) {
			clock.restart();
			text.setString(std::to_string(frameCount) + str);
			frameCount = 0;
		}
		frameCount++;
		if (generateLevel) {
			tileMap = generator.generate();
			generateLevel = false;
		}
		processInput(window);
		window.draw(*tileMap);
		window.draw(text);
		window.display();
	}

	delete tileMap;
	return 0;
}

void processInput(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event))
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed)
			if(event.key.code == sf::Keyboard::G)
				generateLevel = true;	
}