#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class b_plus_tree {
    // this is the max ratings for every node before it splits.
    int keys = 4;


    struct Node{
        bool isLeaf;
        vector<int> ratings;
        vector<vector<string>> player_ids;
        vector<Node*> children;
        Node* next;

        Node(bool leaf){
            isLeaf = leaf;
        }
    }

    Node* root = nullptr;

    //Splits a child node that is full in half and sends the middle rating up to the parent.
    void split(Node* parent, int index, Node* child){
        Node* newChild = new Node(child->isLeaf)

        //had this originally just set to 2, but this'll keep it more modular. 
        int mid = keys/2;

        // move the second half of ratings and all the players inside it into newNode.
        if(child->isLeaf){
            for(int i = mid; i < child->ratings.size(); i++){
                newNode -> ratings.push_back(child->ratings[i]);
                newNode -> player_ids.push_back(child->player_ids[i]);
            }

            // resize both vectors to 2.
            child->ratings.resize(mid);
            child->player_ids.resize(mid);

            newNode -> next = child->next; 
            child->next = newNode;

            // Need to figure out how to copy the first rating of node up to the parent /////
            
        }else{
            //Splitting an internal node, we will send the middle rating up to the parent.
            int midRating = child->ratings[mid];
            for(int i = mid + 1; i < child->ratings.size(); i++){
                
            }

        }

    }

public:
    ~b_plus_tree(){
        deleteNode(root);
    }
};