#ifndef OUTPUT_H
#define OUTPUT_H

#include "misc\ConsoleUtils.h"
#include "Color.h"

#include <mutex> 

class Output {
private:
	static std::mutex mtx;

public:
	Output(int id = -1) { 
		mtx.lock();
		SetTextColor(Color::EntityColor(id)); 
	};

	~Output() { mtx.unlock(); }

	void ChangeColor(int color) { SetTextColor(color); };

	void ChangeEntity(int id) { SetTextColor(Color::EntityColor(id)); };

};

#endif