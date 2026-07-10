#pragma once
#include <string>
#include "../protocol/ClientRole.h"


class ClientPlayerData
{
public:
	explicit ClientPlayerData(uint32_t playerId, std::string playerNickname, ClientRole role) : id(playerId), nickname(std::move(playerNickname)), m_role(role)
	{
		init();
	}


	void setId(uint32_t value);
	uint32_t getId() const;

	void setNickname(std::string newNickname);
	const std::string& getNickname() const;

	void setRole(ClientRole role);
	ClientRole getRole() const;

	void setHandSize(int value);
	int getHandSize() const;

	void setPassAction(bool value);
	bool getPassAction() const;

	void setWinner(bool value);
	bool getWinner() const;

	void setLoser(bool value);
	bool getLoser() const;

	void setReady(bool value);
	bool getReady() const;

private:
	void init();


	uint32_t id;
	std::string nickname;
	ClientRole m_role;

	int handSize;
	bool passAction;

	bool winner;
	bool loser;

	bool ready;
};