#ifndef FIGHT_MOVE_H
#define FIGHT_MOVE_H

#include <string>

enum FightMove {
	None,

	Rock,
	Paper,
	Cisors,
};

inline std::string FightMoveToStr(int move) {
	switch (move) {
		case None:	 return "nothing";
		case Rock:	 return "rock";
		case Paper:	 return "paper";
		case Cisors: return "cisors";
		default:	 return "weird";
	}
}

#endif
