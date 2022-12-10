//
// Created by harel on 27/11/2022.
//
#ifndef DATA_STRUCTURES_1_PLAYER_H
#define DATA_STRUCTURES_1_PLAYER_H
#include "AVLTtree.h"
#include "Team.h"

class Team;

class Player{
private:
    int playerId;
    int gamesPlayed;
    int goals;
    int cardsReceived;
    bool goalKeeper;
    std::weak_ptr<Team> player_team;
    std::weak_ptr<Player> closest_above;
    std::weak_ptr<Player> closest_below;

public:
    static int compare_playerID(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    static int compare_playerGoals(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2);
    static int compare_playerCards(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2);

    Player(int playerId, int gamesPlayed, int goals, int cardsReceived, bool goalKeeper): playerId(playerId),
    gamesPlayed(gamesPlayed), goals(goals), cardsReceived(cardsReceived), goalKeeper(goalKeeper){};
    Player(const Player& other);
    void update(int gamesPlayed, int scoredGoals, int cardsReceived);
    void updateTeam();
    int getGamesPlayed();
    int getPlayerGoals();
    int getCards();
    int getPlayerId();
    bool getGoalKeeper();
    int getTeamId();
    void setTeam(std::shared_ptr<Team> team);
    std::shared_ptr<Team> getTeam();
    std::shared_ptr<Player> getClosestBelow();
    std::shared_ptr<Player> getClosestAbove();
    void setClosestBelow(const std::shared_ptr<Player>& player);
    void setClosestAbove(const std::shared_ptr<Player>& player);
    void setGamesPlayed(int games);
    void setGoalKeeper(bool goalKeeper);
    void addGamesPlayed(int games);
    Player()=default;
    void setPlayerTeam(const std::shared_ptr<Team>& t);
    ////add play dtor
     ~Player()=default;
};

#endif //DATA_STRUCTURES_1_PLAYER_H
