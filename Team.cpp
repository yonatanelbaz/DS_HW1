//
// Created by harel on 27/11/2022.
//
#ifndef TEAM_CPP
#define TEAM_CPP
#include "Team.h"

Team::Team(int teamId, int points): teamId(teamId), points(points),numPlayers(0), teamValid(false), goalKeepers(0),
            sumGoals(0), sumCards(0), gamesPlayed(0),
            playersById(new AVLTree<std::shared_ptr<Player>>(Player::compare_playerID)),
            playersByGoals(new AVLTree<std::shared_ptr<Player>>(Player::compare_playerGoals)){}

void Team::incNumPlayers() {
    this->numPlayers++;
}
//Getters
AVLTree<std::shared_ptr<Player>>* Team::getPlayersById() {
    return this->playersById;
}

AVLTree<std::shared_ptr<Player>>* Team::getPlayersByGoals(){
    return this->playersByGoals;
}
std::shared_ptr<Player> Team::getTopScorer() {
    if(!this->topScorer.lock()) {
        return nullptr;
    }
    return this->topScorer.lock();
}

int Team::getSumCards() {
    return this->sumCards;
}

int Team::getSumGoals() {
    return this->sumGoals;
}

int Team::getTeamId() {
    return this->teamId;
}
int Team::getPoints() {
    return this->points;
}
int Team::getNumPlayers() {
    return this->numPlayers;
}
bool Team::getTeamValid() {
    return this->teamValid;
}
int Team::getGoalkeepers() {
    return this->goalKeepers;
}
void Team::setGoalkeepers(int num){
    this->goalKeepers = num;
}

void Team::incGoalKeepers(){
    this->goalKeepers++;
}
int Team::getGamesPlayed() {
    return this -> gamesPlayed;
}
void Team::incGamesPlayed() {
    this -> gamesPlayed++;
}

int Team::getMatchScore(){
    return this->points + this->sumGoals - this->sumCards;
}

//Setters
void Team::setNumPlayers(int numPlayers) {
    this->numPlayers = numPlayers;
}
void Team::setSumCards(int sumCards) {
    this->sumCards = sumCards;
}

void Team::setSumGoals(int sumGoals) {
    this->sumGoals = sumGoals;
}
void Team::setNumPoints(int points) {
    this->points = points;
}
void Team::setTeamId(int teamId) {
    this->teamId = teamId;
}
void Team::setTeamValid(bool teamVaild) {
    this->teamValid = teamVaild;
}

void Team::setTopScorer(const std::shared_ptr<Player>& player){
    this -> topScorer = player;
}

void Team::setGamesPlayed(int games) {
    this -> gamesPlayed = games;
}

int Team::compare_TeamID(const std::shared_ptr<Team> &team1, const std::shared_ptr<Team> &team2) {
    int team1_id = team1->getTeamId();
    int team2_id = team2->getTeamId();

    if (team1_id > team2_id)
        return -1;
    else if (team1_id == team2_id)
        return 0;
    else
        return 1;
}
void Team::decNumPlayers() {
    this->numPlayers--;
}
void Team::decGoalKeepers() {
    this->goalKeepers--;
}

void Team::setPlayersByGoals(AVLTree<std::shared_ptr<Player>>* playersByGoals) {
    if (this->playersByGoals != nullptr) {
        delete this->playersByGoals;
    }
    this->playersByGoals = playersByGoals;
}

void Team::setPlayersById(AVLTree<std::shared_ptr<Player>>* playersById) {
    if(this->playersById != nullptr){
        delete this->playersById;
    }
    this->playersById = playersById;
}

Team::~Team() {
    delete this->playersById;
    delete this->playersByGoals;
}
#endif