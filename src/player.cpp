#pragma once
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

/*

We will likely develop a randomized dataset generator to produce a large dataset of player profiles. These will include:
Player ID: A unique ID of the queued player.
MMR: Matchmaking rating, likely ranging from 0 to 5000.
Behavior Score: Behavior rating ranging from 1 to 100.
Queue_time: Total seconds spent waiting in queue.


*/

class Player {

    string player_id = ""; //100,000 planned so minimum 6 digits
    int rating;
    int behavior_score;

    const chrono::steady_clock::time_point queue_start = chrono::steady_clock::now();


public:
    Player() : player_id("0000000"), rating(0), behavior_score(0) {};

    Player(string id, int rating, int behavior_score) : player_id(id), rating(rating), behavior_score(behavior_score) {};

    string getID(){
        return player_id;
    }
    
    int getRating(){
        return rating;
    }

    int getBehaviorScore(){
        return behavior_score;
    }

    chrono::seconds::rep get_queue_time() const {
        return chrono::duration_cast<chrono::seconds>(
            chrono::steady_clock::now() - queue_start).count();
    }

};
