#include "CardData.h"


CardData::Suit CardData::getSuit() const
{
	return suit;
}

CardData::Rank CardData::getRank() const
{
	return rank;
}

uint32_t CardData::getId() const
{
	return id;
}