#include "Display.h"
#include "EntityManager.h"

#include <windows.h>


locationView* createWindow() {
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "error with SDL initialisation : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return NULL;
	}
	SDL_Window* window = NULL;
	window = SDL_CreateWindow("westWorld", 100, 100, SCREEN_HEIGHT, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	// Setup renderer
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//display the location
	locationView* locationViewList = createLocationViewList(renderer);

	return locationViewList;
}

void runDisplay(int sleepTime) {
	locationView *locationViewList = createWindow();

	int run = true;

	while (run && EntityMgr->HasEntities()) {
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: { run = false; } break;

			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: { run = false; } break;

				default: break;
				}
			} break;

			default: break;
			}
		}

		for (int id = 0; id < MAX_LOCATION_INDEX; id++) {
			locationViewList[id].updateDisplay();
		}

		if (locationViewList) {
			SDL_RenderPresent(locationViewList[0].getRenderer());
		}

		SDL_Delay(sleepTime);
	}
}

void updateCharacterAtLocation(SDL_Renderer* renderer, position positionLocation, enum location_type location) {

	std::vector<int> entities = EntityMgr->GetAllEntitiesAtLocation(location);

	for (int i = 0; i < entities.size(); i++){
		displayCharacter(renderer, entities[i], positionLocation, i);
	}
}

void displayCharacter(SDL_Renderer* renderer, int entityId, position positionLocation, int positionOrder) {

	position headPos;
	headPos.x = positionLocation.x + CHARACTER_WIDTH * positionOrder;// add space for the other caractere
	if (positionOrder > 0)
		headPos.x += CHARACTER_SPACING;
	headPos.y = positionLocation.y + (SIZE_LOCATION_WH - CHARACTER_HEIGHT);
	position bodyPos = { headPos.x, headPos.y + HEAD_HEIGHT };

	sizeObject headSize = { HEAD_HEIGHT , CHARACTER_WIDTH };
	sizeObject bodySize = { BODY_HEIGHT, CHARACTER_WIDTH };

	SDL_Color headColor = { 255, 229, 204 };//belge
	SDL_Color bodyColor;
	char* nameDisplay;
	switch (entityId)
	{
	case(ent_Nobody):
		return;
	case(ent_Miner_Bob):
		nameDisplay = "BOB";
		bodyColor = { 255, 0, 0 };
		break;
	case(ent_Elsa):
		nameDisplay = "ELSA";
		bodyColor = { 0, 255, 0 };
		break;
	case(ent_Drunkard_Clay):
		nameDisplay = "CLAY";
		bodyColor = { 0, 0, 255 };
		break;
	case(ent_Barman):
		nameDisplay = "BARMAN";
		bodyColor = { 255, 255, 0 };
		break;
	default:
		return;
	}

	//create the head
	createRectangle(renderer, headPos, headSize, headColor);
	//create the body under the head
	createRectangle(renderer, bodyPos, bodySize, bodyColor);
	dispayText(renderer, bodyPos, nameDisplay, 10);
}

locationView* createLocationViewList(SDL_Renderer* renderer) {

	locationView* locationViewList = (locationView *)malloc((MAX_LOCATION_INDEX) * sizeof(locationView));

	for (int id = 0; id < MAX_LOCATION_INDEX; id++) {

		position positionLocation;
		SDL_Color colorLocation;
		char* name;
		enum location_type location;

		switch (id) {
		case in_the_middle_of_nowhere:
			positionLocation = { SCREEN_WIDTH, 
								SCREEN_HEIGHT};
			colorLocation = { 255, 255, 255 };
			name = "Nowhere";
			location = in_the_middle_of_nowhere;
			break;
		case shack:
			positionLocation = { SCREEN_WIDTH - SIZE_LOCATION_WH, 
								0 };
			colorLocation = { 255, 0, 0 };
			name = "bob Shack";
			location = shack;
			break;
		case bank:
			positionLocation = { 0, 
								SCREEN_HEIGHT - SIZE_LOCATION_WH };
			colorLocation = { 255, 255, 0 };
			name = "bank";
			location = bank;
			break;
		case goldmine:
			positionLocation = { SCREEN_WIDTH/2 - SIZE_LOCATION_WH/2,
							   SCREEN_WIDTH / 2 - SIZE_LOCATION_WH/2 };
			colorLocation = { 0, 255, 255 };
			name = "Goldmine";
			location = goldmine;
			break;
		case saloon:
			positionLocation = { 0,
								0 };
			colorLocation = { 0, 255, 0 };
			name = "Saloon";
			location = saloon;
			break;
		case drunkard_shack:
			positionLocation = { SCREEN_WIDTH - SIZE_LOCATION_WH, 
								SCREEN_HEIGHT - SIZE_LOCATION_WH };
			colorLocation = { 0, 0, 255 };
			name = "drunkard Shack";
			location = drunkard_shack;
			break;
		default:
			std::cout << "error with Location initialisation : ";
			return NULL;
		}
		
		locationViewList[id] = locationView(renderer, positionLocation, location, name, colorLocation);
	}
	return locationViewList;
}

void createRectangle(SDL_Renderer* renderer, position pos, sizeObject size, SDL_Color color) {
	// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
	SDL_Rect rectangle;
	rectangle.x = pos.x;
	rectangle.y = pos.y;
	rectangle.w = size.w;
	rectangle.h = size.h;
	// Set render color to blue ( rect will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	// Render rect
	SDL_RenderFillRect(renderer, &rectangle);
}

void dispayText(SDL_Renderer* renderer, position pos, char* text, int fontSize) {

	TTF_Init();
	TTF_Font* Sans = TTF_OpenFont("arial.ttf", fontSize);
	SDL_Color White = { 0, 0, 0 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	TTF_CloseFont(Sans);
	SDL_FreeSurface(surfaceMessage);
	TTF_Quit();

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(Message, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 0, 0, texW, texH };

	dstrect.x = pos.x;  //controls the rect's x coordinate 
	dstrect.y = pos.y; // controls the rect's y coordinte

	SDL_RenderCopy(renderer, Message, NULL, &dstrect);

	SDL_DestroyTexture(Message);
}