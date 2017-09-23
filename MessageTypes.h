#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  
  Msg_BringItOn,

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

  case Msg_AskForDrink:

	  return "Barman! Ah'm thirsty!";

  case Msg_DrinkReady:

	  return "Here's ya drink!";

  default:

    return "Not recognized!";
  }
}

#endif