#include "Barman.h"

bool Barman::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Barman::Update() {
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN| FOREGROUND_INTENSITY);

	m_pStateMachine->Update();
}