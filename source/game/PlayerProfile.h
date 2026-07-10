#pragma once
#include <memory>
#include <string>
#include "PlayerData.h"
#include "../protocol/ClientRole.h"
#include "../protocol/SyncState.h"


class PlayerProfile
{
public:
	PlayerProfile(uint64_t token, std::string nickname) : m_token(token), m_nickname(nickname), m_id(idCounter++)
	{
		m_role = ClientRole::Regular;
	}

	uint64_t getToken() const{ return m_token; }
	uint32_t getId() const { return m_id; }

	void setNickname(std::string newNickname) { m_nickname = newNickname; }
	void setNickname(std::string& newNickname) { m_nickname = newNickname; }
	const std::string& getNickname() const { return m_nickname; }

	void setRole(ClientRole value) { m_role = value; }
	ClientRole getRole() const { return m_role; }
	bool isHost() const { return m_role == ClientRole::Host; }

private:
	inline static uint32_t idCounter = 0;

	const uint64_t m_token;
	const uint32_t m_id;
	std::string m_nickname;

	ClientRole m_role;
};