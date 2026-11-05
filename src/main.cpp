#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "player.cpp"

const string DATA_FILE = "./data/player_data.txt";

vector<Player> player_vector;



using namespace std;



//Algorithms will involve Red-Black Trees and B+ trees


//Import player data from file
void load_data(string data_file){

    ifstream file(data_file);

    if(!file){
        cerr << "Could not open: " << data_file << endl;
    }

    string id;
    int rating;
    int behavior_score;

    while(file >> id >> rating >> behavior_score){
        player_vector.emplace_back(id, rating, behavior_score);
    }
}



    






