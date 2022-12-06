#ifndef WORLDCUP
#define WORLDCUP
#include "worldcup23a1.h"
#define WIN 3
#define PLAYERS 11
#define TIE 1
using namespace std;
world_cup_t::world_cup_t():
    playersIdTree(Player::compare_playerID),
    teamsTree(Team::compare_TeamID),
    playerGoalsTree(Player::compare_playerGoals),
    validTeamsTree(Team::compare_TeamID),
    sumPlayers(0),
    topScorer(nullptr){}


world_cup_t::~world_cup_t()
{
    remove_all_players(*playersIdTree.GetRoot());

    remove_all_teams(*teamsTree.GetRoot());

    remove_all_teams(*validTeamsTree.GetRoot());

    delete playersIdTree.GetRoot();
    delete playerGoalsTree.GetRoot();
    delete teamsTree.GetRoot();
    delete validTeamsTree.GetRoot();

}

StatusType world_cup_t::remove_all_teams(const AVLNode<shared_ptr<Team>>& node){
    if (node.GetValue() && node.GetLeft()) {
        remove_all_teams(*node.GetLeft());
    }
    StatusType result = remove_team(node.GetValue() -> getTeamId());
    if (result != StatusType::SUCCESS) {return result;}

    if (node.GetValue() && node.GetRight()) {
        remove_all_teams(*node.GetRight());
    }
    return result;
}

StatusType world_cup_t::remove_all_players(const AVLNode<shared_ptr<Player>>& node){


        if (node.GetValue() && node.GetLeft()) {
            remove_all_players(*node.GetLeft());
        }
        StatusType result = remove_player(node.GetValue() -> getPlayerId());
        if (result != StatusType::SUCCESS) {return result;}

        if (node.GetValue() && node.GetRight()) {
            remove_all_players(*node.GetRight());
        }
        return result;

}



// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId<0 || points<0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> newTeam(new Team(teamId, points));

        if (this->teamsTree.Find(newTeam) != nullptr) {
            return StatusType::FAILURE;
        }

        StatusType ret_status = this->teamsTree.Insert(newTeam);
        return ret_status;
    }
    catch(const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!????????????

//StatusType world_cup_t::add_player_to_team(){}

StatusType world_cup_t::add_to_player_trees(const std::shared_ptr<Player>& player){

    std::shared_ptr<Player> temp_above;
    std::shared_ptr<Player> temp_below;

    auto newPlayerNode = this -> playersIdTree.Find(player);
    if (newPlayerNode != nullptr) {
        return StatusType::FAILURE;
    }
    auto dadNode = this->playerGoalsTree.FindDad(player);

    if(Player::compare_playerGoals(dadNode -> GetValue(), player) == 1){
        temp_above = dadNode -> GetValue();
        temp_below = dadNode -> GetValue()-> getClosestBelow();
    }
    else{
        temp_above = dadNode -> GetValue() -> getClosestAbove();
        temp_below = dadNode -> GetValue();
    }
    if(temp_above != nullptr) {temp_above -> setClosestBelow(player);}
    if(temp_below != nullptr) {temp_below -> setClosestAbove(player);}
    player -> setClosestAbove(temp_above);
    player -> setClosestBelow(temp_below);

    StatusType ret_status = this->playersIdTree.Insert(player);
    if (ret_status != StatusType::SUCCESS) {
        return ret_status;
    }
    ret_status = this->playerGoalsTree.Insert(player);
    if (ret_status != StatusType::SUCCESS) {
        return ret_status;
    }

    return StatusType::SUCCESS;

}

StatusType world_cup_t::add_to_team_trees(const std::shared_ptr<Team>& team, const std::shared_ptr<Player>& player){

    auto ret_status = team -> getPlayersById() -> Insert(player);
    if (ret_status != StatusType::SUCCESS) {
        return ret_status;
    }
    ret_status = team -> getPlayersByGoals() -> Insert(player);
    if (ret_status != StatusType::SUCCESS) {
        return ret_status;
    }
    // TODO: Your code goes here

    if(Player::compare_playerGoals(team -> getTopScorer(), player) == -1){
        team -> setTopScorer(player);
    }
    if(Player::compare_playerGoals(this -> topScorer, player) == -1){
        this -> topScorer = player;
    }

    team->incNumPlayers();
    player -> updateTeam();
    //playerTeam->setSumGoals(playerTeam->getSumGoals() + goals);
    //playerTeam->setSumCards(playerTeam->getSumCards() + cards);
    player-> setGamesPlayed(player->getGamesPlayed() - team -> getGamesPlayed());

    if (player -> getGoalKeeper()) {
        team->incGoalKeepers();
    }
    bool before = team->getTeamValid();
    if (team->getNumPlayers() >= PLAYERS && team->getGoalkeepers() > 0) {
        team->setTeamValid(true);
    }
    if(!before && team->getTeamValid()){
        validTeamsTree.Insert(team);
    }
    this->sumPlayers++;

    return StatusType::SUCCESS;
}

// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper) {
    StatusType ret_status;

    if ((goals < 0 || gamesPlayed < 0 || teamId <= 0 || playerId <= 0 || cards < 0) ||
        (gamesPlayed == 0 && (goals > 0 || cards > 0))) {
        return StatusType::INVALID_INPUT;
    }

    try {
        std::shared_ptr<Player> newPlayer = std::make_shared<Player>(playerId, gamesPlayed, goals, cards, goalKeeper);
        std::shared_ptr<Team> newTeam = std::make_shared<Team>(teamId, 0);


        ret_status = add_to_player_trees(newPlayer);
        if (ret_status != StatusType::SUCCESS) { return ret_status; }

        auto newTeamNode = this->teamsTree.Find(newTeam);
        if (newTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        auto playerTeam = newTeamNode->GetValue();

        ret_status = add_to_team_trees(playerTeam, newPlayer);
        if (ret_status != StatusType::SUCCESS) { return ret_status; }
        return StatusType::SUCCESS;
    }


    catch(const std::exception& e){
        return StatusType:: ALLOCATION_ERROR;
    }
}
StatusType world_cup_t::remove_team(int teamId) /////
{
    bool removed;
    if(teamId<=0) {
        return StatusType::INVALID_INPUT;
    }
    try{
        std::shared_ptr<Team> tempTeam =  std::shared_ptr<Team>(new Team(teamId, 0));
        auto tempTeamNode = this -> teamsTree.Find(tempTeam);
        if(tempTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        if(!tempTeamNode -> GetValue() -> getPlayersById() -> IsEmpty()) {
            return StatusType::FAILURE;
        }
        removed = teamsTree.Remove(tempTeam);

        if(removed) {
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    }
    catch(const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }


}

/*StatusType world_cup_t::remove_from_player_tree(const std::shared_ptr<Player>& player){


    auto playerTeam = player -> getTeam();
    auto temp_above = player -> getClosestAbove();
    auto temp_below = player -> getClosestBelow();
    if(temp_above != nullptr) {temp_above -> setClosestBelow(temp_below);}
    if(temp_below != nullptr) {temp_below -> setClosestAbove(temp_above);}
    bool removed = playersIdTree.Remove(player);
    if(!removed) {
        return StatusType::FAILURE;
    }
    removed = playerGoalsTree.Remove(player);
    if(!removed) {
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}*/
//TODO: Everything that has to do with closest
StatusType world_cup_t::remove_player(int playerId)
{
    bool removed;
    if(playerId<=0) {
        return StatusType::INVALID_INPUT;
    }
    try{
        std::shared_ptr<Player> tempPlayer = std::shared_ptr<Player>(new Player(playerId, 0, 0, 0, false));
        auto tempPlayerNode = this ->playersIdTree.Find(tempPlayer);

        if(tempPlayerNode == nullptr) {
            return StatusType::FAILURE;
        }
        tempPlayer = tempPlayerNode -> GetValue();
        auto playerTeam = tempPlayer -> getTeam();

        removed = this->playersIdTree.Remove(tempPlayer);
        if(!removed) {return StatusType::FAILURE;}

        removed = this->playerGoalsTree.Remove(tempPlayer);
        if(!removed) {return StatusType::FAILURE;}

        removed = playerTeam->getPlayersById()->Remove(tempPlayer);
        if(!removed) {return StatusType::FAILURE;}

        removed = playerTeam->getPlayersByGoals()->Remove(tempPlayer);
        if(!removed) {return StatusType::FAILURE;}


        auto temp_above = tempPlayer -> getClosestAbove();
        auto temp_below = tempPlayer -> getClosestBelow();
        if(temp_above != nullptr) {temp_above -> setClosestBelow(temp_below);}
        if(temp_below != nullptr) {temp_below -> setClosestAbove(temp_above);}

        /*if(tempPlayerNode->GetValue()->getClosestAbove()== nullptr) {
                if(tempPlayerNode->GetValue()->getClosestBelow()!=nullptr) {
                    tempPlayerNode->GetValue()->getClosestBelow()->setClosestAbove(nullptr);
                }

        } else {
            if(tempPlayerNode->GetValue()->getClosestBelow()== nullptr) {
                tempPlayerNode->GetValue()->getClosestAbove()->setClosestBelow(nullptr);
            } else {
                tempPlayerNode->GetValue()->getClosestAbove()->setClosestBelow(tempPlayerNode->GetValue()->getClosestBelow());
                tempPlayerNode->GetValue()->getClosestBelow()->setClosestAbove(tempPlayerNode->GetValue()->getClosestAbove());
            }
        }*/

        //////I don't check if they appear in each tree because they should
        if(this->topScorer->getPlayerId() == tempPlayer -> getPlayerId()) {
            this->topScorer  = (this->playerGoalsTree.FindMaxValInTree(this->playerGoalsTree.GetRoot()))->GetValue();
            this -> topScorer = tempPlayer -> getClosestBelow();
        }

       // auto tempPlayerNode = this -> playersIdTree.Find(tempPlayer);
        if(tempPlayerNode->GetValue()->getGoalKeeper()) {
            playerTeam->decGoalKeepers();
        }

        playerTeam->decNumPlayers();
        if(playerTeam->getNumPlayers()<PLAYERS || playerTeam -> getGoalkeepers() == 0) {
            playerTeam->setTeamValid(false);
            this->validTeamsTree.Remove(playerTeam);
        }
        playerTeam->setSumCards(playerTeam->getSumCards()-tempPlayerNode->GetValue()->getCards());
        playerTeam->setSumGoals(playerTeam->getSumGoals()-tempPlayerNode->GetValue()->getPlayerGoals());
        if(playerTeam->getTopScorer()->getPlayerId()==tempPlayer->getPlayerId())  {
            (playerTeam->setTopScorer((playerTeam->getPlayersByGoals()->FindMaxValInTree(playerTeam->getPlayersByGoals()->GetRoot())->GetValue())));
        }
        this->sumPlayers--;
    }
    catch(const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    // TODO: Your code goes here
    return StatusType::SUCCESS;
}


// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                            int scoredGoals, int cardsReceived)
{
    StatusType result;
    if(playerId<=0 || gamesPlayed<0 || scoredGoals<0 || cardsReceived<0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Player> newPlayer = shared_ptr<Player>(new Player(playerId, gamesPlayed, scoredGoals, cardsReceived, false));
        auto player = this->playersIdTree.Find(newPlayer);
        if (player == nullptr) {
            return StatusType::FAILURE;
        }
        newPlayer = player-> GetValue();
        newPlayer -> update(gamesPlayed, scoredGoals, cardsReceived);

        result = remove_player(playerId);
        if(result != StatusType::SUCCESS){return result;}

        result = add_to_player_trees(newPlayer);
        if(result != StatusType::SUCCESS){return result;}

        result = add_to_team_trees(newPlayer -> getTeam(), newPlayer);

    }
    catch (const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }
    return result;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 == teamId2 || teamId1<=0 || teamId2<=0) {
        return StatusType::INVALID_INPUT;
    }
    shared_ptr<Team> t1 = this->teamsTree.find(teamId1);
    if(t1 == nullptr || t1->getTeamValid() == false) {
        return StatusType::FAILURE;
    }
    shared_ptr<Team> t2 = this->teamsTree.find(teamId2);
    if(t2 == nullptr || t2->getTeamValid() ==false) {
        return StatusType::FAILURE;
    }
    int result1 = t1->getPoints() + t1->getSumGoals() - t1->getSumCards();
    int result2 = t2->getPoints() + t2->getSumGoals() - t2->getSumCards();
    if(result1==result2) {
        t1->setNumPoints(t1->getPoints()+TIE);
        t2->setNumPoints(t2->getPoints()+TIE);
    } else {
        if(result1>result2) {
            t1->setNumPoints(t1->getPoints()+WIN);
        } else {
            t2->setNumPoints(t2->getPoints()+WIN);
        }
    }
	// TODO: Your code goes here+
    /////need to update playergame
	return StatusType::SUCCESS;
}


// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Player> tempPlayer = shared_ptr<Player>(new Player(playerId, 0, 0, 0, 0));
        auto tempPlayerNode = this->playersIdTree.Find(tempPlayer);
        if (tempPlayerNode == nullptr) {
            return StatusType::FAILURE;
        }
        return (tempPlayerNode->GetValue()->getGamesPlayed() + tempPlayerNode->GetValue()->getTeam()->getGamesPlayed());
    }
    catch (const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<Team> tempTeam = shared_ptr<Team>(new Team(teamId, 0));
        auto tempTeamNode = this -> teamsTree.Find(tempTeam);
        if(tempTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        return tempTeamNode -> GetValue() -> getPoints();
    }
    catch(const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }

}
void world_cup_t::mergeArr(std::shared_ptr<Player> arr1[],int arr1Len,  std::shared_ptr<Player> arr2[], int arr2Len, std::shared_ptr<Player> arr3[], int (*comp)(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b)) {



        int i = 0, j = 0, k = 0;

        while (i<arr1Len && j <arr2Len)
        {
            if (comp(arr1[i], arr2[j])==1)
                arr3[k++] = arr1[i++];
            else
                arr3[k++] = arr2[j++];
        }

        // Store remaining elements of first array
        while (i < arr1Len)
            arr3[k++] = arr1[i++];

        // Store remaining elements of second array
        while (j < arr2Len)
            arr3[k++] = arr2[j++];
}
void world_cup_t::playersToArr(AVLNode<std::shared_ptr<Player>>* curr, std::shared_ptr<Player> arr[], int index) {
    if(curr== nullptr) {
        return;
    }
    playersToArr(curr->GetLeft(), arr, index);
    arr[index] = curr->GetValue();
    index++;
    playersToArr(curr->GetRight(), arr, index);
}
void world_cup_t::arrToTeam(std::shared_ptr<Player> arr1Id, std::shared_ptr<Player> arr2Id, std::shared_ptr<Team> teamToInsert) {

}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    try{
        shared_ptr<Team> tempTeam1 = shared_ptr<Team>(new Team(teamId1, 0));
        auto tempTeamNode1 = this->teamsTree.Find(tempTeam1);
        if (tempTeamNode1 == nullptr) {
            return StatusType::FAILURE;
        }
        shared_ptr<Team> tempTeam2 = shared_ptr<Team>(new Team(teamId2, 0));
        auto tempTeamNode2 = this->teamsTree.Find(tempTeam2);
        if (tempTeamNode2 == nullptr) {
            return StatusType::FAILURE;
        }
        shared_ptr<Team> tempNewTeam = shared_ptr<Team>(new Team(newTeamId, 0));
        auto tempNewTeamNode = this->teamsTree.Find(tempNewTeam);
        int len1 =tempTeamNode1->GetValue()->getNumPlayers();
        int len2 =tempTeamNode2->GetValue()->getNumPlayers();
        std::shared_ptr<Player> arr1Goals[len1];
        std::shared_ptr<Player> arr2Goals[len2];
        std::shared_ptr<Player> arr3Goals[len1 + len2];
        std::shared_ptr<Player> arr1Id[tempTeamNode1->GetValue()->getNumPlayers()];
        std::shared_ptr<Player> arr2Id[tempTeamNode2->GetValue()->getNumPlayers()];
        std::shared_ptr<Player> arr3Id[tempTeamNode2->GetValue()->getNumPlayers()+tempTeamNode1->GetValue()->getNumPlayers()];
        playersToArr(tempTeamNode1->GetValue()->getPlayersByGoals()->GetRoot(),arr1Goals, 0);
        playersToArr(tempTeamNode2->GetValue()->getPlayersByGoals()->GetRoot(),arr2Goals, 0);
        playersToArr(tempTeamNode1->GetValue()->getPlayersById()->GetRoot(), arr1Id, 0);
        playersToArr(tempTeamNode2->GetValue()->getPlayersById()->GetRoot(), arr2Id, 0);
        mergeArr(arr1Goals, len1, arr2Goals, len2, arr3Goals, Player::compare_playerGoals);
        mergeArr(arr1Id, len1, arr2Id, len2, arr3Id, Player::compare_playerID);
        if(newTeamId == teamId1) {
            arrToTeam(arr1Goals, arr2Goals, *tempTeamNode1->GetValue()->getPlayersByGoals());
            arrToTeam(arr1Id, arr2Id, *tempTeamNode1->GetValue()->getPlayersById());
        }
        if(newTeamId == teamId2) {
            mergeTeam(arr1Goals, arr2Goals);
        }
        if (tempNewTeamNode != nullptr) {
            return StatusType::FAILURE;
        }
    }
    catch(const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }

	// TODO: Your code goes here
	return StatusType::SUCCESS;
}


// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if(teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    if(teamId<0) {
        return this->topScorer->getPlayerId();
    }
    try{
        shared_ptr<Team> tempTeam = shared_ptr<Team>(new Team(teamId, 0));
        auto tempTeamNode = this -> teamsTree.Find(tempTeam);
        if(tempTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        //return
        return tempTeamNode-> GetValue() -> getTopScorer() -> getPlayerId();
        // TODO: Your code goes here
    }
    catch (const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }

}

// TODO : Nothing Elbaz did already no touching!!!!!!!!!!!!!!!!!!!!!!!!!
output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    if(teamId<0) {
        return this->sumPlayers;

    }
    try {
        shared_ptr<Team> tempTeam = shared_ptr<Team>(new Team(teamId, 0));
        auto tempTeamNode = this->teamsTree.Find(tempTeam);
        if (tempTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        return tempTeamNode->GetValue()->getNumPlayers();
    }
    catch(const std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(teamId==0 || output== nullptr) {
        return StatusType::FAILURE;
    }
    try{
        if(teamId>0) {
            shared_ptr<Team> tempTeam = shared_ptr<Team>(new Team(teamId, 0));
            auto tempTeamNode = this->teamsTree.Find(tempTeam);
            if (tempTeamNode == nullptr) {
                return StatusType::FAILURE;
            }
            if(tempTeamNode->GetValue()->getNumPlayers()==0) {
                return StatusType::FAILURE;
            }
            makePlayersList(tempTeamNode->GetValue()->getPlayersByGoals()->GetRoot(), output, 0);
        } else{
            if(this->sumPlayers==0) {
                return StatusType::FAILURE;
            }
            makePlayersList(this->playerGoalsTree.GetRoot(), output, 0);
        }
    }   catch(const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}
//////Not sure if this is how you move an index further in the array
/////Inorder
/////How can I check if there is an allocation error in makePlayersList
void world_cup_t::makePlayersList(AVLNode<std::shared_ptr<Player>>* curr, int* const output, int index) {
    if(curr== nullptr) {
        return;
    }
    makePlayersList(curr->GetLeft(), output, index);
    output[index] = curr->GetValue()->getPlayerId();
    index++;
    makePlayersList(curr->GetRight(), output, index);

}
int world_cup_t::dist(int num1, int num2) {
    return abs(num1-num2);
}
output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if(playerId<=0 || teamId<=0) {
        return StatusType::FAILURE;
    }
    if(this->sumPlayers==1) {
        return StatusType::FAILURE;
    }
    try{
        shared_ptr<Team> tempTeam = shared_ptr<Team>(new Team(teamId, 0));
        auto teamNode = this ->teamsTree.Find(tempTeam);
        if(teamNode == nullptr) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Player> tempPlayer = std::shared_ptr<Player>(new Player(playerId, 0, 0, 0, false));
        auto tempPlayerNode  = tempTeam->getPlayersById()->Find(tempPlayer);
        if(tempPlayerNode == nullptr) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Player> currPlayer = tempPlayerNode->GetValue();
        auto closest_above = currPlayer->getClosestAbove();
        auto closest_below = currPlayer->getClosestBelow();
        if(closest_above == nullptr) {
            if(closest_below == nullptr) {
                return StatusType::FAILURE;
            }
            return closest_below->getPlayerId();
        }
        if(closest_below == nullptr) {
            return closest_above->getPlayerId();
        }
        if(dist(closest_above->getPlayerGoals(),currPlayer->getPlayerGoals())<dist(closest_below->getPlayerGoals(),currPlayer->getPlayerGoals()))  {
            return closest_above->getPlayerId();
        }
        if(dist(closest_above->getPlayerGoals(),currPlayer->getPlayerGoals())>dist(closest_below->getPlayerGoals(),currPlayer->getPlayerGoals())) {
            return closest_below->getPlayerId();
        }
        //////they have the same goal distance
        if(dist(closest_above->getCards(),currPlayer->getCards())<dist(closest_below->getCards(),currPlayer->getCards()))  {
            return closest_above->getPlayerId();
        }
        if(dist(closest_above->getCards(),currPlayer->getCards())>dist(closest_below->getCards(),currPlayer->getCards()))  {
            return closest_below->getPlayerId();
        }
        if(dist(closest_above->getPlayerId(), currPlayer->getPlayerId())<dist(closest_below->getPlayerId(), currPlayer->getPlayerId())) {
            return closest_above->getPlayerId();
        }
        if(dist(closest_above->getPlayerId(), currPlayer->getPlayerId())>dist(closest_below->getPlayerId(), currPlayer->getPlayerId())) {
            return closest_below->getPlayerId();
        }
        return max(closest_above->getPlayerId(), closest_below->getPlayerId());
    }
    catch(const std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

/*StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper)
{
    std::shared_ptr<Player> temp_above;
    std::shared_ptr<Player> temp_below;

    if((goals<0 || gamesPlayed<0 || teamId<=0 || playerId<=0 || cards<0) || (gamesPlayed==0 && (goals>0 || cards>0))) {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Player> newPlayer = shared_ptr<Player>(new Player(playerId, gamesPlayed, goals, cards, goalKeeper));
        shared_ptr<Team> newTeam = shared_ptr<Team>(new Team(teamId, 0));

        auto newTeamNode = this->teamsTree.Find(newTeam);
        auto newPlayerNode = this -> playersIdTree.Find(newPlayer);
        if (newPlayerNode != nullptr || newTeamNode == nullptr) {
            return StatusType::FAILURE;
        }
        auto dadNode = this->playerGoalsTree.FindDad(newPlayer);

        if(Player::compare_playerGoals(dadNode -> GetValue(), newPlayer) == 1){
            temp_above = dadNode -> GetValue();
            temp_below = dadNode -> GetValue()-> getClosestBelow();
        }
        else{
            temp_above = dadNode -> GetValue() -> getClosestAbove();
            temp_below = dadNode -> GetValue();
        }
        if(temp_above != nullptr) {temp_above -> setClosestBelow(newPlayer);}
        if(temp_below != nullptr) {temp_below -> setClosestAbove(newPlayer);}
        newPlayer -> setClosestAbove(temp_above);
        newPlayer -> setClosestBelow(temp_below);


        StatusType ret_status = this->playersIdTree.Insert(newPlayer);
        if (ret_status != StatusType::SUCCESS) {
            return ret_status;
        }
        ret_status = this->playerGoalsTree.Insert(newPlayer);
        if (ret_status != StatusType::SUCCESS) {
            return ret_status;
        }

        auto playerTeam = newTeamNode->GetValue();

        ret_status = playerTeam -> getPlayersById() -> Insert(newPlayer);
        if (ret_status != StatusType::SUCCESS) {
            return ret_status;
        }
        ret_status = playerTeam -> getPlayersByGoals() -> Insert(newPlayer);
        if (ret_status != StatusType::SUCCESS) {
            return ret_status;
        }
        // TODO: Your code goes here

        if(Player::compare_playerGoals(playerTeam -> getTopScorer(), newPlayer) == -1){
            playerTeam -> setTopScorer(newPlayer);
        }
        if(Player::compare_playerGoals(this -> topScorer, newPlayer) == -1){
            this -> topScorer = newPlayer;
        }
        playerTeam->incNumPlayers();
        newPlayer -> updateTeam();
        //playerTeam->setSumGoals(playerTeam->getSumGoals() + goals);
        //playerTeam->setSumCards(playerTeam->getSumCards() + cards);
        newPlayer-> setGamesPlayed(newPlayer->getGamesPlayed() - playerTeam -> getGamesPlayed());

        if (goalKeeper == true) {
            playerTeam->incGoalKeepers();
        }
        bool before = playerTeam->getTeamValid();
        if (playerTeam->getNumPlayers() >= PLAYERS && playerTeam->getGoalkeepers() > 0) {
            playerTeam->setTeamValid(true);
        }
        if(!before && playerTeam->getTeamValid()){
            validTeamsTree.Insert(playerTeam);
        }
        this->sumPlayers++;
        return StatusType::SUCCESS;
    }
    catch(const std::exception& e){
        return StatusType:: ALLOCATION_ERROR;
    }
}*/
#endif