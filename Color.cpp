#include "Color.h"
#include "EntityNames.h"


int Color::EntityColor(int EntityID) {
	switch (EntityID)
	{
	case ent_Miner_Bob:
		return (FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case ent_Elsa:
		return (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	default:
		return (BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
}