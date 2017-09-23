#ifndef DRINK_TYPES_H
#define DRINK_TYPES_H

#include <string>

enum DrinkType {
	Milk,
	Beer,
	Whisky
};

inline std::string DrinkToStr(int drink)
{
	switch (drink)
	{
	case Milk:   return "milk";
	case Beer:   return "beer";
	case Whisky: return "whisky";
	default:     return "beverage";
	}
}

#endif

