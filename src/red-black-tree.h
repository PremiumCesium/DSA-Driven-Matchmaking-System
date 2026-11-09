#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

//RBTree that keeps track of players and sorts them by ranking.


class rbtree{

struct Node {

    int rating;
    vector<string> player_ids;

    Node* left = nullptr;
    Node* right = nullptr;
    bool red = true;


    Node(int rating, string id) : rating(rating), player_ids(1, id) {}
    bool isRed() { return red;}
};

Node* root = nullptr;

bool isRed(Node* node){
    return node != nullptr && node->isRed();
}

void getPlayersInRange(Node* node, int minRating, int maxRating, vector<pair<string, int>>& players) const {

    if(!node){
        return;
    }

    if(node->rating > minRating){
        getPlayersInRange(node->left, minRating, maxRating, players);
    }

    if(node->rating >= minRating && node->rating <= maxRating){
        for(const string& id : node->player_ids){
            players.push_back({id, node->rating});
        }
    }

    if(node->rating < maxRating){
        getPlayersInRange(node->right, minRating, maxRating, players);
    }

}

Node* rotate_left(Node* node){
    Node* temp = node->right;

    node->right = temp->left;
    temp->left = node;

    temp->red = node->red;
    node->red = true;

    return temp;
}

Node* rotate_right(Node* node){
    Node* temp = node->left;

    node->left = temp->right;
    temp->right = node;

    temp->red = node->red;
    node->red = true;

    return temp;
}

void flip_colors(Node* node){
    node->red = !node->red;
    node->left->red = !node->left->red;
    node->right->red = !node->right->red;
}

Node* insert_node(Node* node, int rating, const string& id){
    if(!node){
        return new Node(rating, id);
    }

    if(rating < node->rating){
        node->left = insert_node(node->left, rating, id);
    }else if(rating > node->rating){
        node->right = insert_node(node->right, rating, id);
    }else {
        for(const string& present : node->player_ids){
            if(present == id){
                return node;
            }
        }

        node->player_ids.push_back(id);
    }


    //Reds go to the left
    if(isRed(node->right) && !isRed(node->left)){
        node = rotate_left(node);
    }

    //Consecutive Reds
    if(isRed(node->left) && isRed(node->left->left)){
        node = rotate_right(node);
    }

    //Two red children
    if(isRed(node->left) && isRed(node->right)){
        flip_colors(node);
    }

    return node;
}


void deleteNode(Node* node){
    if(!node){
        return;
    }

    deleteNode(node->left);
    deleteNode(node->right);
    delete node;
}


    public:

    ~rbtree(){
        deleteNode(root);
    }

    void insert(int rating, const string& id){
        root = insert_node(root, rating, id);
        root->red = false;
    }

    vector<string> findPlayersByRating(int rating) const {
        const Node* current = root;

        while(current){
            if(rating < current->rating){
                current = current->left;
            }else if(rating > current->rating){
                current = current->right;
            }else{
                return current->player_ids;
            }
        }

        return {};
    }


    vector<pair<string,int>> findPlayersInRange(int minRating, int maxRating) const {
        vector<pair<string, int>> playersInRange;
        
        if(minRating <= maxRating){
            getPlayersInRange(root, minRating, maxRating, playersInRange);
        }


        return playersInRange;

    }

    bool removePlayer(int rating, const string& id) {

        Node* current = root;

        while(current){
            if(rating < current->rating){
                current = current->left;
            }else if(rating > current->rating){
                current = current->right;
            }else{
                for(auto player = current->player_ids.begin(); player != current->player_ids.end(); ++player){
                    if(*player == id){
                        current->player_ids.erase(player);
                        return true;
                    }
                }

                return false;
            }
        }
        
        return false;
    }

};