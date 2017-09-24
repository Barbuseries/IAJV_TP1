#include "Barman.h"

bool Barman::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Barman::Update() {
	m_pStateMachine->Update();
}