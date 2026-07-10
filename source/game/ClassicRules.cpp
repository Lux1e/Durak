#include "ClassicRules.h"
#include "../core/Constants.h"
#include "../core/EventSystem.h"
#include "../core/EventInitiator.h"
#include "GameLogic.h"
#include "events/Event.h"
#include "events/game/InvalidMoveEvent.h"
#include "events/game/TossEvent.h"
#include "events/game/BeatEvent.h"
#include "events/game/MainTrumpCardIdentifiedEvent.h"
#include "events/game/ExtraCardsToDealEvent.h"
#include "events/game/ShowPlayerTrumpCardEvent.h"
#include "events/game/IdentifyNewRolesPositionEvent.h"
#include "events/game/DealCardEvent.h"
#include "events/game/PassActionToPlayerEvent.h"
#include "events/game/NewPhaseEvent.h"
#include "events/game/ClearTableEvent.h"
#include "events/game/RewardPlayerEvent.h"


void ClassicRules::init()
{
	timer = 0.0f;

	allAttackerPlayersPassAction = false;
	allCardsAreBeaten = true;
	defended = false;
	isLimitAttackerCardsReached = false;

	maxCardsInPlayersHandsAtStart = Constants::Game::TotalCardsInHands;
	dealCardSeatPositionHelper = 0;
	newPhaseHelper = GameState::Phase::None;

	switchingPhaseTimerHelper = Constants::Timers::StandardSwitchingPhaseDelayTime;

	extraAttackerTossed = false;

	attackersOrder.reserve(gameState.getPlayers().size() - 1);
	winCandidatesSeatPosition.reserve(gameState.getPlayers().size());
	realWinnersSeatPosition.reserve(gameState.getPlayers().size() - 1);

	loserSeatPosition = -1;

	countWinnersAwarded = 0;


	defended = false;
	preparePhaseChangeDone = false;

	identifyTrumpCardDone = false;
	identifyFirstMoveDone = false;

	resetTurn();
}

void ClassicRules::subscribeAll()
{
	/*serverES.subscribe<TossRequestEvent, ClassicRules>(this, &ClassicRules::onTossRequestEvent);
	serverES.subscribe<BeatRequestEvent, ClassicRules>(this, &ClassicRules::onBeatRequestEvent);
	serverES.subscribe<PassActionFromPlayerEvent, ClassicRules>(this, &ClassicRules::onPassActionFromPlayerEvent);*/
}


void ClassicRules::update(float dt)
{
	timer += dt;

	switch (gameState.getCurrentPhase())
	{
		case GameState::Phase::StartingGame:
			startingGameUpdate(); //to do events
			break;

		case GameState::Phase::MainPlayerMove:
			mainPlayerMoveUpdate(gameState.getPlayers().size()); //to do events
			break;

		case GameState::Phase::EveryPlayerMove:
			everyPlayerMoveUpdate(); //to do events
			break;

		case GameState::Phase::RefillPhase:
			refillPhaseUpdate();
			break;

		case GameState::Phase::GameOver:
			//to do
			break;
	}
}


void ClassicRules::startingGameUpdate()
{
	//shuffle

	if (!dealCardsDone)
	{
		if (!dealCards(Constants::Timers::DealDelayTime))
			return;
		else
			dealCardsDone = true;
	}

	if (!identifyTrumpCardDone)
	{
		if (!gameState.getMainTrumpCardId())
		{
			uint32_t cardId = logic.identifyTrumpCard();
			serverES.publish<MainTrumpCardIdentifiedEvent>(cardId);

			resetTimer();
		}

		if (timer < Constants::Timers::ShowingCardDelayTime)
			return;

		identifyTrumpCardDone = true;

		resetTimer();

		return;
	}

	if (!identifyFirstMoveDone)
	{
		auto firstMove = logic.identifyFirstMove();
		if (!firstMove)
		{
			++maxCardsInPlayersHandsAtStart;
			serverES.publish<ExtraCardsToDealEvent>({});

			dealCardsDone = false;
		}

		else
		{
			maxCardsInPlayersHandsAtStart = Constants::Game::TotalCardsInHands;
			serverES.publish<ShowPlayerTrumpCardEvent>({firstMove->first, firstMove->second});

			int attackerSeatPosition = firstMove->first;
			int defenderSeatPosition = (attackerSeatPosition + 1) % gameState.getPlayers().size();

			setRoles(attackerSeatPosition, defenderSeatPosition);

			identifyFirstMoveDone = true;
		}

		resetTimer();

		return;
	}

	else
	{
		if (timer >= Constants::Timers::StandardDelayTime)
			setNewPhase(GameState::Phase::MainPlayerMove);
	}
}

void ClassicRules::mainPlayerMoveUpdate(int playersSize)
{
	if (preparePhaseChangeDone)
	{
		if (timer >= switchingPhaseTimerHelper)
			setNewPhase(newPhaseHelper);

		return;
	}


	bool mainAttackerPassAction = gameState.getCurrentMainAttackerPlayerData()->getPassAction();
	bool defenderPassAction = gameState.getCurrentDefenderPlayerData()->getPassAction();


	if (playersSize == 2)
	{
		if (defenderPassAction && (mainAttackerPassAction || isLimitAttackerCardsReached))
		{
			setDefended(false);

			preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);

			return;
		}

		if (mainAttackerPassAction && allCardsAreBeaten)
		{
			setDefended(true);

			preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);

			return;
		}
	}

	else
	{
		if (allAttackerPlayersPassAction)
		{
			if (allCardsAreBeaten)
			{
				setDefended(true);

				preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);

				return;
			}

			if (defenderPassAction)
			{
				setDefended(false);

				preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);

				return;
			}
		}

		if (mainAttackerPassAction && !allAttackerPlayersPassAction && !isLimitAttackerCardsReached)
		{
			setPassAction(*gameState.getCurrentMainAttackerPlayerData(), false);

			preparePhaseChange(GameState::Phase::EveryPlayerMove, Constants::Timers::FastSwitchingPhaseDelayTime);

			return;
		}
	}
}

void ClassicRules::everyPlayerMoveUpdate()
{
	if (preparePhaseChangeDone)
	{
		if (timer >= switchingPhaseTimerHelper)
			setNewPhase(newPhaseHelper);

		return;
	}

	auto currentMainAttackerPlayer = gameState.getCurrentMainAttackerPlayerData();
	auto currentDefenderPlayer = gameState.getCurrentDefenderPlayerData();

	bool mainAttackerPassAction = currentMainAttackerPlayer->getPassAction();
	bool defenderPassAction = currentDefenderPlayer->getPassAction();


	if (allAttackerPlayersPassAction && allCardsAreBeaten)
	{
		setDefended(true);

		preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);

		return;
	}

	if (defenderPassAction)
	{
		if (allAttackerPlayersPassAction || isLimitAttackerCardsReached)
		{
			setDefended(false);

			preparePhaseChange(GameState::Phase::RefillPhase, Constants::Timers::StandardSwitchingPhaseDelayTime);
		}

		return;
	}

	if (extraAttackerTossed && !currentMainAttackerPlayer->getHand().empty() && !isLimitAttackerCardsReached)
	{
		extraAttackerTossed = false;

		for (auto& p : gameState.getPlayers())
		{
			if (p.getId() == currentDefenderPlayer->getId() || p.getHand().empty())
				continue;

			setPassAction(*gameState.getPlayerDataByPlayerId(p.getId()), false);
		}

		preparePhaseChange(GameState::Phase::MainPlayerMove, Constants::Timers::FastSwitchingPhaseDelayTime);

		return;
	}
}

void ClassicRules::refillPhaseUpdate()
{
	if (preparePhaseChangeDone)
	{
		if (timer >= switchingPhaseTimerHelper)
			setNewPhase(newPhaseHelper);

		return;
	}

	if (!clearTableDone)
	{
		clearTable();

		clearTableDone = true;
		resetTimer();
	}

	if (!clearTableEventDone)
	{
		if (timer >= Constants::Timers::ClearTableDelayTime)
		{
			clearTableEventDone = true;
			resetTimer();
		}

		return;
	}

	if (!dealCardsDone)
	{
		if (!dealCards(Constants::Timers::DealDelayTime))
			return;
		else
			dealCardsDone = true;
	}

	if (!checkForWinnersAndLoserDone)
		checkForWinnersAndLoser();

	if (!winAndLoseEventsDone)
	{
		if (!winAndLoseEvents(Constants::Timers::AwardingDelayTime))
			return;

		winAndLoseEventsDone = true;

		resetTimer();
	}


	if (loserSeatPosition == -1)
	{
		determineNextTurnRoles();
		preparePhaseChange(GameState::Phase::MainPlayerMove, Constants::Timers::StandardSwitchingPhaseDelayTime);

		resetTurn();
		resetTimer();

		return;
	}

	else
	{
		//to do GameOverEvent
	}
}


void ClassicRules::recalculateRuleState()
{
	allCardsAreBeaten = (gameState.attackerCardsOnTableSize() == gameState.defenderCardsOnTableSize());
	isLimitAttackerCardsReached = (gameState.attackerCardsOnTableSize() == Constants::Game::MaxTablePositions);

	auto currentDefenderPlayer = gameState.getCurrentDefenderPlayerData();

	allAttackerPlayersPassAction = true;
	for (const auto& player : gameState.getPlayers())
	{
		if (player.getId() == currentDefenderPlayer->getId())
			continue;

		if (!player.getPassAction())
		{
			allAttackerPlayersPassAction = false;
			break;
		}
	}
}


void ClassicRules::preparePhaseChange(GameState::Phase newPhase, float timeDelayBeforeChange)
{
	preparePhaseChangeDone = true;
	newPhaseHelper = newPhase;
	switchingPhaseTimerHelper = timeDelayBeforeChange;
	resetTimer();
}


void ClassicRules::resetTimer()
{
	timer = 0.0f;
}


void ClassicRules::setPassAction(PlayerData& player, bool value)
{
	player.setPassAction(value);
	serverES.publish<PassActionToPlayerEvent>({ player.getPassAction(), value });
}

bool ClassicRules::dealCards(float dealDelayTime)
{
	if (timer < dealDelayTime)
		return false;

	if (gameState.getDeck().empty())
		return true;

	bool needToDeal = false;
	for (const auto& player : gameState.getPlayers())
	{
		if (player.getHand().size() < maxCardsInPlayersHandsAtStart)
		{
			needToDeal = true;
			break;
		}
	}

	if (!needToDeal)
		return true;

	int eventSeatPosition = dealCardSeatPositionHelper;
	std::optional<uint32_t> cardIdOpt = std::nullopt;

	if (gameState.getCurrentPhase() == GameState::Phase::StartingGame)
	{
		cardIdOpt = logic.dealOneCard(dealCardSeatPositionHelper);


		if (!cardIdOpt)
		{
			DEBUG_LOG("Logical rule mistake");
			throw std::runtime_error("Logical mistake in classic Rules");
		}

		dealCardSeatPositionHelper = (dealCardSeatPositionHelper + 1) % gameState.getPlayers().size();
	}


	if (gameState.getCurrentPhase() == GameState::Phase::RefillPhase)
	{
		int mainAttackerSeatPosition = gameState.getCurrentAttackerSeatPosition();
		int defenderSeatPosition = gameState.getCurrentDefenderSeatPosition();

		dealCardSeatPositionHelper = -1;


		for (auto& attackerOrder : attackersOrder)
		{
			if (gameState.getPlayerDataBySeatPosition(attackerOrder)->getHand().size() >= Constants::Game::TotalCardsInHands)
				continue;
			else
				dealCardSeatPositionHelper = attackerOrder;
		}

		if (dealCardSeatPositionHelper == -1)
			dealCardSeatPositionHelper = defenderSeatPosition;


		cardIdOpt = logic.dealOneCard(dealCardSeatPositionHelper);


		if (!cardIdOpt)
		{
			DEBUG_LOG("Logical rule mistake");
			throw std::runtime_error("Logical mistake in classic Rules");
		}

		eventSeatPosition = dealCardSeatPositionHelper;
	}

	serverES.publish<DealCardEvent>({ eventSeatPosition, *cardIdOpt });

	resetTimer();

	return false;
}

void ClassicRules::setRoles(int attackerSeatPosition, int defenderSeatPosition)
{
	gameState.setCurrentAttackerSeatPosition(attackerSeatPosition);
	gameState.setCurrentDefenderSeatPosition(defenderSeatPosition);
	
	serverES.publish<IdentifyNewRolesPositionEvent>({ attackerSeatPosition, defenderSeatPosition });
}

void ClassicRules::setNewPhase(GameState::Phase phase)
{
	gameState.setCurrentPhase(phase);
	serverES.publish<NewPhaseEvent>(phase);

	preparePhaseChangeDone = false;
	resetTimer();
}

void ClassicRules::setDefended(bool value) //to do event
{
	defended = value;
	//to do Defended event;
}

void ClassicRules::clearTable()
{
	if (defended)
	{
		logic.moveTableCardsToBoneyard();
		serverES.publish<ClearTableEvent>(Constants::Game::BoneyardId);
	}

	else
	{
		int seatPosition = gameState.getCurrentDefenderSeatPosition();

		logic.moveTableCardsToHand(seatPosition);
		serverES.publish<ClearTableEvent>(seatPosition);
	}
}


void ClassicRules::checkForWinnersAndLoser()
{
	if (loserSeatPosition != -1 || winCandidatesSeatPosition.empty() || !gameState.getDeck().empty())
		return;

	for (auto& pos : winCandidatesSeatPosition)
	{
		const auto& player = gameState.getPlayerDataBySeatPosition(pos);
		if (player->getHand().empty())
		{
			player->setWinner(true);

			realWinnersSeatPosition.emplace_back(pos);

			if (realWinnersSeatPosition.size() == gameState.getPlayers().size() - 1)
			{
				for (const auto& p : gameState.getPlayers())
				{
					if (!p.isWinner())
					{
						int seatPosition = gameState.getSeatPositionByPlayerData(p);
						loserSeatPosition = seatPosition;

						break;
					}
				}
			}
		}
	}

	winCandidatesSeatPosition.clear();

	checkForWinnersAndLoserDone = true;
	resetTimer();
}

bool ClassicRules::winAndLoseEvents(float awardingDelayTime)
{
	if (timer < awardingDelayTime)
		return false;

	if (countWinnersAwarded == realWinnersSeatPosition.size())
		return true;


	int rewardSeatPosition = realWinnersSeatPosition[countWinnersAwarded];
	serverES.publish<RewardPlayerEvent>(rewardSeatPosition);

	++countWinnersAwarded;

	resetTimer();

	return false;
}

void ClassicRules::determineNextTurnRoles()
{
	int attackerCandidateSeatPosition = -1;
	int defenderCandidateSeatPosition = -1;

	size_t playersSize = gameState.getPlayers().size();

	if (defended)
	{
		attackerCandidateSeatPosition = gameState.getCurrentDefenderSeatPosition();

		if (gameState.getPlayerDataBySeatPosition(attackerCandidateSeatPosition)->isWinner())
		{
			do
			{
				attackerCandidateSeatPosition = (attackerCandidateSeatPosition + 1) % playersSize;
			} while (gameState.getPlayerDataBySeatPosition(attackerCandidateSeatPosition)->isWinner());
		}

		defenderCandidateSeatPosition = (attackerCandidateSeatPosition + 1) % playersSize;

		if (gameState.getPlayerDataBySeatPosition(defenderCandidateSeatPosition)->isWinner())
		{
			do
			{
				defenderCandidateSeatPosition = (defenderCandidateSeatPosition + 1) % playersSize;
			} while (gameState.getPlayerDataBySeatPosition(defenderCandidateSeatPosition)->isWinner());
		}

		setRoles(attackerCandidateSeatPosition, defenderCandidateSeatPosition);

		defended = false;
	}

	else
	{
		attackerCandidateSeatPosition = (gameState.getCurrentDefenderSeatPosition() + 1) % playersSize;

		if (gameState.getPlayerDataBySeatPosition(attackerCandidateSeatPosition)->isWinner())
		{
			do
			{
				attackerCandidateSeatPosition = (attackerCandidateSeatPosition + 1) % playersSize;
			} while (gameState.getPlayerDataBySeatPosition(attackerCandidateSeatPosition)->isWinner());
		}

		defenderCandidateSeatPosition = (attackerCandidateSeatPosition + 1) % playersSize;

		if (gameState.getPlayerDataBySeatPosition(defenderCandidateSeatPosition)->isWinner())
		{
			do
			{
				defenderCandidateSeatPosition = (defenderCandidateSeatPosition + 1) % playersSize;
			} while (gameState.getPlayerDataBySeatPosition(defenderCandidateSeatPosition)->isWinner());
		}

		setRoles(attackerCandidateSeatPosition, defenderCandidateSeatPosition);
	}

	if (attackerCandidateSeatPosition == defenderCandidateSeatPosition)
	{
		DEBUG_LOG("logic mistake, only one player left");
		throw std::runtime_error("logic mistake");
	}
}

void ClassicRules::resetTurn()
{
	clearTableDone = false;
	clearTableEventDone = false;
	dealCardsDone = false;
	checkForWinnersAndLoserDone = false;
	winAndLoseEventsDone = false;

	extraAttackerTossed = false;

	for (const auto& player : gameState.getPlayers())
	{
		if (player.isWinner())
			continue;

		setPassAction(*gameState.getPlayerDataByPlayerId(player.getId()), false);
	}
}


bool ClassicRules::canToss(const GameState::Phase& phase, int playerSeatPosition, uint32_t cardId) const
{
	if ((phase != GameState::Phase::EveryPlayerMove && phase != GameState::Phase::MainPlayerMove) ||
		gameState.attackerCardsOnTableSize() >= Constants::Game::MaxTablePositions ||
		gameState.getPlayerDataBySeatPosition(gameState.getCurrentDefenderSeatPosition())->getHand().size() - (gameState.attackerCardsOnTableSize() - gameState.defenderCardsOnTableSize()) <= 0)
		return false;

	if (gameState.cardsOnTableSize() > 0)
	{
		const auto& card = gameState.getPlayerDataBySeatPosition(playerSeatPosition)->getCardById(cardId);
		if (!card)
			return false;

		for (auto& tableCard : gameState.getTableCards())
		{
			if (card->getRank() == tableCard->getRank())
				return true;
		}

		return false;
	}

	return true;
}

bool ClassicRules::canBeat(const GameState::Phase& phase, int playerSeatPosition, int tablePosition, uint32_t cardId) const
{
	if ((phase != GameState::Phase::EveryPlayerMove && phase != GameState::Phase::MainPlayerMove) || (gameState.cardsOnTablePositionSize(tablePosition) != 1) ||
		playerSeatPosition != gameState.getCurrentDefenderSeatPosition())
		return false;

	const auto* card = gameState.getPlayerDataBySeatPosition(gameState.getCurrentDefenderSeatPosition())->getCardById(cardId);
	if (!card)
		return false;

	const auto& cardSuit = card->getSuit();
	const auto& cardRank = card->getRank();

	const auto& tableCard = gameState.getTableCardsAtPosition(tablePosition).front();
	const auto& tableCardSuit = tableCard.getSuit();
	const auto& tableCardRank = tableCard.getRank();

	const auto& trumpSuit = gameState.getTrumpSuit();
	if (!trumpSuit)
		return false;

	if (cardSuit == *trumpSuit)
	{
		if (tableCardSuit != *trumpSuit)
			return true;

		else if (cardRank > tableCardRank)
			return true;

		else
			return false;
	}

	else
	{
		if (cardSuit != tableCardSuit || cardRank < tableCardRank)
			return false;

		else
			return true;
	}
}


//void ClassicRules::onTossRequestEvent(TossRequestEvent& event)
//{
//	if (!canToss(gameState.getCurrentPhase(), event.playerSeatPosition, event.cardId))
//	{
//		serverES.publish<InvalidMoveEvent>(event.playerSeatPosition);
//		return;
//	}
//
//	int firstEmptyTablePosition = gameState.attackerCardsOnTableSize();
//
//
//	bool success = logic.toss(event.playerSeatPosition, firstEmptyTablePosition, event.cardId);
//
//
//	if (success)
//	{
//		recalculateRuleState();
//
//		if (std::find(attackersOrder.begin(), attackersOrder.end(), event.playerSeatPosition) != attackersOrder.end())
//			attackersOrder.push_back(event.playerSeatPosition);
//
//		if (event.playerSeatPosition != gameState.getCurrentAttackerSeatPosition())
//			extraAttackerTossed = true;
//
//		if (gameState.getPlayerDataBySeatPosition(event.playerSeatPosition)->getHand().empty())
//		{
//			setPassAction(*gameState.getPlayerDataBySeatPosition(event.playerSeatPosition), true);
//
//			winCandidatesSeatPosition.emplace_back(event.playerSeatPosition);
//		}
//
//		serverES.publish<TossEvent>({ event.playerSeatPosition, firstEmptyTablePosition, event.cardId });
//	}
//
//	else
//		serverES.publish<InvalidMoveEvent>(event.playerSeatPosition);
//}
//
//void ClassicRules::onBeatRequestEvent(BeatRequestEvent& event)
//{
//	if (!canBeat(gameState.getCurrentPhase(), event.playerSeatPosition, event.tablePosition, event.cardId))
//	{
//		serverES.publish<InvalidMoveEvent>(event.playerSeatPosition);
//		return;
//	}
//
//
//	bool success = logic.beat(event.playerSeatPosition, event.tablePosition, event.cardId);
//
//
//	if (success)
//	{
//		recalculateRuleState();
//
//		if (gameState.getPlayerDataBySeatPosition(event.playerSeatPosition)->getHand().empty())
//			winCandidatesSeatPosition.emplace_back(event.playerSeatPosition);
//
//		serverES.publish<BeatEvent>({ event.playerSeatPosition, event.tablePosition, event.cardId });
//	}
//
//	else
//		serverES.publish<InvalidMoveEvent>(event.playerSeatPosition);
//}
//
//void ClassicRules::onPassActionFromPlayerEvent(PassActionFromPlayerEvent& event)
//{
//	setPassAction(*gameState.getPlayerDataBySeatPosition(event.playerSeatPosition), true);
//}