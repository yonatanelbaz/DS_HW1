// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "AVLTtree.h"
#include "Player.h"
#include "Team.h"
#include "stdio.h"
#include <memory>
using namespace std;
class world_cup_t {
private:
	//
	// Here you may add anything you want
	//
    AVLTree<std::shared_ptr<Team>> teamsTree;
    AVLTree<std::shared_ptr<Team>> validTeamsTree;
    AVLTree<std::shared_ptr<Player>> playersIdTree;
    AVLTree<std::shared_ptr<Player>> playerGoalsTree;
    int sumPlayers;
    std::weak_ptr<Player> topScorer;
public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId, int points);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	
	StatusType remove_player(int playerId);
	
	StatusType update_player_stats(int playerId, int gamesPlayed,
	                                int scoredGoals, int cardsReceived);
	
	StatusType play_match(int teamId1, int teamId2);
	
	output_t<int> get_num_played_games(int playerId);
	
	output_t<int> get_team_points(int teamId);
    void mergeArr(std::shared_ptr<Player> arr1[], int arr1Len, std::shared_ptr<Player> arr2[], int arr2Len, std::shared_ptr<Player> arr3[], int (*comp)(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b));

    void makePlayersList(AVLNode<std::shared_ptr<Player>>* curr, int* const output, int index);

    StatusType mergeTeam(std::shared_ptr<Player> team1[], std::shared_ptr<Player> team2[]);

    void playersToArr(AVLNode<std::shared_ptr<Player>>* curr, std::shared_ptr<Player> arr[], int index);

    void get_all_players_by_goals(std::shared_ptr<Team> t1, std::shared_ptr<Team> arr[]);

	StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

	output_t<int> get_top_scorer(int teamId);
	
	output_t<int> get_all_players_count(int teamId);
	
	StatusType get_all_players(int teamId, int *const output);
	
	output_t<int> get_closest_player(int playerId, int teamId);
	
	output_t<int> knockout_winner(int minTeamId, int maxTeamId);

    StatusType add_to_player_trees(const std::shared_ptr<Player>& newPlayer);

    StatusType add_to_team_trees(const std::shared_ptr<Team>& team, const std::shared_ptr<Player>& player);

    void remove_all_players(const AVLNode<shared_ptr<Player>>& node);

    StatusType remove_all_teams(const AVLNode<shared_ptr<Team>>& node);

    int ranged_teams_count(int minTeamId, int maxTeamId, AVLNode<std::shared_ptr<Team>>* curr);

    void ranged_teams_to_arr(int minTeamId, int maxTeamId, AVLNode<std::shared_ptr<Team>>* curr, std::shared_ptr<Team>* arr, int* index);

    std::shared_ptr<Player> getTopScorer();


    int dist(int num1, int num2);
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
