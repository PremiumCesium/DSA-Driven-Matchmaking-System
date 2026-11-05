#pragma once


#include <iostream>
#include <string>
#include <vector>

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
    if(node->right->isRed() && !node->left->isRed()){
        node = rotate_left(node);
    }

    //Consecutive Reds
    if(node->left->isRed() && node->left->left->isRed()){
        node = rotate_right(node);
    }

    //Two red children
    if(node->left->isRed() && node->right->isRed()){
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

};