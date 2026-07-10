#pragma once
#include <cstdint>


class CardData
{
public:
	enum class Suit { Hearts, Diamonds, Clubs, Spades };
	enum class Rank {Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

	CardData(Suit s, Rank r, uint32_t i) : suit(s), rank(r), id(i) {}

	Suit getSuit() const;
	Rank getRank() const;
	uint32_t getId() const;

private:
	Suit suit;
	Rank rank;
	uint32_t id;
};