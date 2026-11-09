#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "player.cpp"
#include "red-black-tree.h"
#include "b+tree.h"
using namespace std;

vector<Player> players;
string file = "./data/player_data.txt";

int main()
{
    int option = 0;

    rbtree RBTree;
    b_plus_tree BTree;


    double redBlackTime = 0;
    double bTreeTime = 0;

    double redBlackMatch = 0;
    double bTreeMatch = 0;
    double redBlackMatchTime = 0;
    double bTreeMatchTime = 0;

    bool data = false;
    bool match = false;
    bool RBinserted = false;
    bool Binserted = false;

    while(option != 7)
    {
        cout<<"Matchmaking Performance Benchmark"<<endl;
        cout<<"1. Initialize Dataset" << endl;
        cout<<"2. Insert Players into Red-Black Tree"<< endl;
        cout<<"3. Insert Players into B+ Tree" << endl;
        cout<<"4. Run Matchmaking" << endl;
        cout<<"5. Compare Matchmaking Times" << endl;
        cout<<"6. Compare Insert Times" << endl;
        cout<<"7. Exit" << endl;
        cout<<"Enter your choice: ";
        cin>>option;


        if (option == 1)
        {
            if( RBinserted == false)
            {
                if ( Binserted == false)
                {
                    int playerCount;
                    playerCount = 100000;
                    int seed;
                    seed = 42;

                    srand(seed);
                    ofstream outputFile(file);

                    int i;
                    i = 1;

                    while ( i<= playerCount)
                    {
                        string id = "";
                        int x = i;

                        while (x > 0)
                        {
                            int last = x % 10;
                            char digit = '0' + last;
                            id = digit + id;
                            x = x / 10;
                        }

                        while ( id.length() < 7)
                        {
                            id = "0" + id;
                        }

                        int ranks;
                        ranks = rand() % 5001;

                        int score;
                        score = rand() % 100;
                        score += 1;

                        outputFile << id << " " << ranks << " " << score << endl;

                        i = i + 1;
                    
                    }

                    outputFile.close();

                    players.clear();

                    ifstream inputFile(file);

                    string idTwo;
                    int scoreTwo;
                    int ranksTwo;

                    while (inputFile >> idTwo >> ranksTwo >> scoreTwo)
                    {
                        Player player(idTwo, ranksTwo, scoreTwo);
                        players.push_back(player);
                    }

                    inputFile.close();

                    cout<<"Completed initializing dataset"<< endl;

                    data = true;



                }
                else
                {
                    cout << "Data has already been initialized." << endl;
                }
            }
            else
            {
                cout << "Data has already been initialized." << endl;
            }
        }
        else if ( option == 2)
        {

            if (RBinserted == false)
            {
                if ( players.size() > 0)
                {

                    clock_t startTime;
                    startTime = clock();

                    int x = 0;

                    while ( x < players.size())
                    {
                        RBTree.insert(players[x].getRating(), players[x].getID());
                        x++;
                    }
                    clock_t endTime;
                    endTime = clock();

                    redBlackTime = ((endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;

                    RBinserted = true;

                    cout<<"Red-Black insert time: " << redBlackTime << " ms" << endl;



                }
                else if(players.size() == 0)
                {
                    cout << "Data has not been initialized. Must choose option 1 first" << endl;

                }


            }
        

        }
        else if ( option == 3)
        {

            if( Binserted == false)
            {
                if ( players.size() > 0)
                {
                    clock_t startTime = clock();
                    int r;
                    r = 0;

                    while ( r < players.size())
                    {
                        BTree.insert(players[r].getRating(), players[r].getID());
                        r++;
                    }

                    clock_t endTime = clock();
                    bTreeTime = ((endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;

                    Binserted = true;

                    cout<<"B+ Tree insert time: " << bTreeTime << " ms" << endl;
                }
                else if ( players.size() == 0)
                {
                    cout << "Data has not been initialized. Must choose option 1 first" << endl;
                }
                
            }
        

        }

        else if ( option == 4)
        {
            if ( RBinserted == true)
            {
                if(Binserted == true)
                {
                    //Red-black tree section
                
                    redBlackMatch = 0;

                    clock_t startTime = clock();

                   for(int minRating = 0; minRating <= 5000; minRating+=100){
                        int maxRating = minRating+99;
                        vector<pair<string,int>> pool = RBTree.findPlayersInRange(minRating, maxRating);
                        while(pool.size()>=12){
                            redBlackMatch++;
                            for (int i = 0; i < 12; i++) {
                                string id = pool[i].first;      // player ID
                                int rating = pool[i].second;    // Player rating
                                RBTree.removePlayer(rating, id); 
                            }
                            pool.erase(pool.begin(), pool.begin() + 12);
                        }
                    }

                    clock_t endTime = clock();
                    redBlackMatchTime = ((endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;

                    cout << "Red-Black Tree Matchmaking Time: " << redBlackMatchTime << " ms" << endl;
                    cout << "Red-Black Tree Matches Made: " << redBlackMatch << endl;


                    // B+ tree section
                    bTreeMatch = 0;

                    startTime = clock();

                    for(int minRating = 0; minRating <= 5000; minRating+=100){
                        int maxRating = minRating+99;
                        vector<pair<string,int>> pool = BTree.findPlayersInRange(minRating, maxRating);
                        while(pool.size()>=12){
                            bTreeMatch++;
                            for (int i = 0; i < 12; i++) {
                                string id = pool[i].first;      // player ID
                                int rating = pool[i].second;    // Player rating
                                BTree.removePlayer(rating, id); 
                            }
                            pool.erase(pool.begin(), pool.begin() + 12);
                        }
                    }

                    endTime = clock();
                    bTreeMatchTime = ((endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;

                    match = true;

                
                    cout << "B+ Tree Matchmaking Time: " << bTreeMatchTime << " ms" << endl;
                    cout << "B+ Tree Matches Made: " << bTreeMatch << endl;


                }
                else
                {
                    cout<<"Insert players into trees first."<<endl;
                }
                
            }
            else
            {
                cout<<"Insert players into trees first."<<endl;

            }
        }
        else if ( option == 5)
        {
            if ( RBinserted == true)
            {
                if ( Binserted == true)
                {
                    if ( match == true)
                    {
                        if(bTreeMatchTime > redBlackMatchTime)
                        {
                            cout << "Red-Black Tree matchmaking time: " << redBlackMatchTime << " ms" << endl;
                            cout << "B+ Tree matchmaking time: " << bTreeMatchTime << " ms" << endl;
                            cout << "Red-Black Tree is faster for matchmaking" << endl;
                            
                        }
                        else if (bTreeMatchTime < redBlackMatchTime)
                        {
                            cout << "Red-Black Tree matchmaking time: " << redBlackMatchTime << " ms" << endl;
                            cout << "B+ Tree matchmaking time: " << bTreeMatchTime << " ms" << endl;
                            cout << "B+ Tree is faster for matchmaking" << endl;
                        }
                        else
                        {
                            cout << "Red-Black Tree matchmaking time: " << redBlackMatchTime << " ms" << endl;
                            cout << "B+ Tree matchmaking time: " << bTreeMatchTime << " ms" << endl;
                            cout << "Both trees have the same matchmaking time" << endl;
                        }
                    }
                    else
                    {
                        cout<<"Must run option 4 first"<<endl;
                    }
                }
                else
                {
                    cout<<"Must run option 4 first"<<endl;


                }
            }
            else
            {
                cout<<"Must run option 4 first"<<endl;
            }
        }
        else if ( option == 6)
        {
            if ( RBinserted == true)
            {
                if ( Binserted == true)
                {
                    if ( redBlackTime > bTreeTime)
                    {
                        cout<<"Red-Black Tree insert time: " << redBlackTime << " ms" << endl;
                        cout<<"B+ Tree insert time: " << bTreeTime << " ms" << endl;
                        cout << "B+ Tree is faster for insertion" << endl;
                    }
                    else if ( redBlackTime < bTreeTime)
                    {
                        cout<<"Red-Black Tree insert time: " << redBlackTime << " ms" << endl;
                        cout<<"B+ Tree insert time: " << bTreeTime << " ms" << endl;
                        cout << "Red-Black Tree is faster for insertion" << endl;
                    }
                    else
                    {
                        cout<<"Red-Black Tree insert time: " << redBlackTime << " ms" << endl;
                        cout<<"B+ Tree insert time: " << bTreeTime << " ms" << endl;
                        cout << "Both trees have the same insertion time" << endl;
                    }
                }
                else
                {
                    cout<<"Insert players into trees first."<<endl;

                }
            }
            else
            {
                cout<<"Insert players into trees first."<<endl;

            }
            

        }
        else if (option == 7)
        {
            cout<< "Thank you for using the Matchmaking Performance Benchmark program. Goodbye!" << endl;
        }

    }


    
}
