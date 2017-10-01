#ifndef COLOR_H
#define COLOR_H


#include <string>
#include <Windows.h>

class Color
{
public:
	// This function return the color code associated with a given game object
	static int EntityColor(int EntityID);
};


#endif