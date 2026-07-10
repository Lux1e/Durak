#include "ClientPlayerData.h"



void ClientPlayerData::init()
{
	handSize = 0;
	passAction = false;

	winner = false;
	loser = false;

	ready = false;
}


void ClientPlayerData::setId(uint32_t value)
{
	id = value;
}

uint32_t ClientPlayerData::getId() const
{
	return id;
}


void ClientPlayerData::setNickname(std::string newNickname)
{
	nickname = newNickname;
}

const std::string& ClientPlayerData::getNickname() const
{
	return nickname;
}


void  ClientPlayerData::setRole(ClientRole role)
{
	m_role = role;
}

ClientRole ClientPlayerData::getRole() const
{
	return m_role;
}

void ClientPlayerData::setHandSize(int value)
{
	handSize = value;
}

int ClientPlayerData::getHandSize() const
{
	return handSize;
}


void ClientPlayerData::setPassAction(bool value)
{
	passAction = value;
}

bool ClientPlayerData::getPassAction() const
{
	return passAction;
}


void ClientPlayerData::setWinner(bool value)
{
	winner = value;
}

bool ClientPlayerData::getWinner() const
{
	return winner;
}


void ClientPlayerData::setLoser(bool value)
{
	loser = value;
}

bool ClientPlayerData::getLoser() const
{
	return loser;
}


void ClientPlayerData::setReady(bool value)
{
	ready = value;
}

bool ClientPlayerData::getReady() const
{
	return ready;
}