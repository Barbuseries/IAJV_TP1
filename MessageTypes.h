#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,

  Msg_BringItOn,
  Msg_ImOn,
  Msg_RefereeFight,

  Msg_ChooseMove,
  Msg_Win,
  Msg_Lose,

  Msg_AskForDrink,
  Msg_DrinkReady,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_BringItOn:

	  return "Bring it on !";

  case Msg_ImOn:
	  
	  return "Ah'm on!";

  case Msg_ChooseMove:

	  return "ChooseMove";

  case Msg_Win:

	  return "A winner is ya!";

  case Msg_Lose:

	  return "No luck, looser!";

  case Msg_AskForDrink:

	  return "Barman! Ah'm thirsty!";

  case Msg_DrinkReady:

	  return "Here's ya drink!";

  case Msg_RefereeFight:

	  return "Watch this!";

  default:

    return "Not recognized!";
  }
}

#endif