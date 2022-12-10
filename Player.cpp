//
// Created by harel on 27/11/2022.
//
#include "Player.h"


Player::Player(const Player& other){
    this -> playerId = other.playerId;
    this -> gamesPlayed = other.gamesPlayed;
    this -> goals = other.goals;
    this -> cardsReceived = other.cardsReceived;
    this -> goalKeeper = other.goalKeeper;
    this -> player_team = other.player_team;
    this -> closest_above = other.closest_above;
    this -> closest_below = other.closest_below;
}

int Player::getTeamId(){
    return this-> player_team.lock() -> getTeamId();
}
int Player::getGamesPlayed() {
    return this->gamesPlayed;
}
int Player::getPlayerId() {
    return (this->playerId);
}

int Player::getPlayerGoals() {
    return (this->goals);
}

int Player::getCards() {
    return (this->cardsReceived);
}

bool Player::getGoalKeeper() {
    return (this->goalKeeper);
}

void Player::setGoalKeeper(bool goalKeeper) {
    this->goalKeeper = goalKeeper;
}

void Player::update(int gamesPlayed, int scoredGoals, int cardsReceived) {
    this->gamesPlayed += gamesPlayed;
    this->goals += scoredGoals;
    this->cardsReceived += cardsReceived;

}

void Player::setPlayerTeam(const std::shared_ptr<Team>& t) {
    this->player_team = t;
}
void Player::updateTeam() {
    this->player_team.lock()->setSumCards(this->player_team.lock()->getSumCards() + this->cardsReceived);
    this->player_team.lock()->setSumGoals(this->player_team.lock()->getSumGoals() + this->goals);
}
int Player::compare_playerID(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    int player1_id = player1->getPlayerId();
    int player2_id = player2->getPlayerId();

    if (player1_id > player2_id)
        return -1;
    else if (player1_id == player2_id)
        return 0;
    else
        return 1;
}

int Player::compare_playerGoals(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    int player1_goals = player1->getPlayerGoals();
    int player2_goals = player2->getPlayerGoals();

    if (player1_goals > player2_goals)
        return -1;
    else if (player1_goals == player2_goals)
        return compare_playerCards(player1, player2);
    else
        return 1;
}

int Player::compare_playerCards(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    int player1_cards = player1->getCards();
    int player2_cards = player2->getCards();

    if (player1_cards > player2_cards)
        return 1;
    else if (player1_cards == player2_cards)
        return compare_playerID(player1, player2);
    else
        return -1;
}

std::shared_ptr<Team> Player::getTeam(){
    return player_team.lock();
}

std::shared_ptr<Player> Player::getClosestBelow(){
    return closest_below.lock();
}
std::shared_ptr<Player> Player::getClosestAbove(){
    return closest_above.lock();
}

void Player::setClosestBelow(const std::shared_ptr<Player>& player){
    this -> closest_below = player;
}
void Player::setClosestAbove(const std::shared_ptr<Player>& player){
    this -> closest_above = player;
}

void Player::setGamesPlayed(int games){
    this -> gamesPlayed = games;
}

void Player::addGamesPlayed(int games){
    this -> gamesPlayed += games;
}

void Player::setTeam(std::shared_ptr<Team> team){
    this -> player_team = team;
}

