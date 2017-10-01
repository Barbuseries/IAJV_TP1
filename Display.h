#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "SDL_ttf.h"
#undef main
#include <iostream>
#include <stdlib.h>

#include "Locations.h"
#include <vector>
#include "EntityNames.h"


struct position {
	int x, y;
};

struct sizeObject {
	int h, w;
};

class locationView;
class EntityManager;

locationView* createWindow();
void createRectangle(SDL_Renderer* renderer, position pos, sizeObject size, SDL_Color color);
void displayCharacter(SDL_Renderer* renderer, int entityId, position positionLocation, int positionOrder = 0);
void dispayText(SDL_Renderer* renderer, position pos, char* text, int fontSize);
void updateView(locationView* locationViewList, int count, int sleepTime);
locationView* createLocationViewList(SDL_Renderer* renderer);
void updateCharacterAtLocation(SDL_Renderer* renderer, position positionLocation, enum location_type location);

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

const int SIZE_LOCATION_WH = 150;

const int HEAD_HEIGHT = 20;
const int BODY_HEIGHT = 35;
const int CHARACTER_HEIGHT = HEAD_HEIGHT + BODY_HEIGHT;
const int CHARACTER_WIDTH = 40;

const int CHARACTER_SPACING = 10;

	

class locationView {
	private:
		position pos;
		enum location_type  location;
		char* name;
		SDL_Color colorLocation;
		sizeObject size;
		SDL_Renderer* renderer;

	public:
		locationView(SDL_Renderer* renderer, position pos, enum location_type location, char* name, SDL_Color colorLocation, sizeObject size = { SIZE_LOCATION_WH ,SIZE_LOCATION_WH }) {
			this->renderer = renderer;
			this->pos = pos;
			this->location = location;
			this->name = name;
			this->colorLocation = colorLocation;
			this->size = size;
		}
		position getPosition() {
			return pos;
		}

		void updateDisplay() {
			createRectangle(renderer, pos, size, colorLocation);
			dispayText(renderer, pos, name, 20);
			updateCharacterAtLocation(renderer, pos, location);
		}
};

#endif


