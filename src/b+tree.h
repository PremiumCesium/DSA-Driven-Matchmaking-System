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
            next = nullptr;
        }
    };

    Node* root = nullptr;

    // splits a child node that is full in half, keeping the right half as 2 and the left as 2, and sends the middle rating up to the parent but also keeps it in the child.
    void split(Node* parent, int index, Node* child){
        Node* otherHalf = new Node(child->isLeaf);

        //I had this originally just set to 2, but this'll keep it more modular. 
        int mid = keys/2;

        // move the second half of ratings and all the players inside it into otherHalf.
        if(child->isLeaf){
            for(int i = mid; i < child->ratings.size(); i++){
                otherHalf -> ratings.push_back(child->ratings[i]);
                otherHalf -> player_ids.push_back(child->player_ids[i]);
            }

            // resize both vectors to 2.
            child->ratings.resize(mid);
            child->player_ids.resize(mid);

            otherHalf -> next = child->next; 
            child->next = otherHalf;

            // dummy to add an extra slot to vector.
            parent->ratings.push_back(0);

            //copies all of them one to the right so as to put the rating in the correct routing index.
            for(int i = parent->ratings.size() - 1; i > index; i--){
                parent->ratings[i] = parent->ratings[i - 1];
            }

            parent->ratings[index] = otherHalf->ratings[0];
            
            //shift children right to make room at index + 1.
            parent->children.push_back(nullptr);
            for(int i = parent->children.size() - 1; i > index + 1; i--){
                parent->children[i] = parent->children[i - 1];
            }
            parent->children[index + 1] = otherHalf;
        }else{
            //Splitting an internal node, this will send the middle rating up to the parent.
            int midRating = child->ratings[mid];
            for(int i = mid + 1; i < child->ratings.size(); i++){
                otherHalf -> ratings.push_back(child->ratings[i]);
            }
            for(int i = mid+1; i < child->children.size();i++){
                otherHalf->children.push_back(child->children[i]);
            }

            child->ratings.resize(mid);
            child->children.resize(mid+1);

            // Same as in the main statement
            parent->ratings.push_back(0);
            for(int i = parent->ratings.size() - 1; i > index; i--){
                parent->ratings[i] = parent->ratings[i - 1];
            }
            parent->ratings[index] = midRating;

            parent->children.push_back(nullptr);
            for(int i = parent->children.size() - 1; i > index + 1; i--){
                parent->children[i] = parent->children[i - 1];
            }
            parent->children[index + 1] = otherHalf;
            // --------------------------------------
        }
    }

    void insert_node(Node* node, int rating, string id){
        if(node->isLeaf){
            // check if this rating is already in the leaf
            for(int i = 0; i < node->ratings.size(); i++){
                if(node->ratings[i] == rating){
                    //if already there just exit
                    for(auto present : node->player_ids[i]){
                        if(present == id){
                            return;
                        }
                    }
                    node->player_ids[i].push_back(id);
                    return;
                }
            }

            // find the spot where the new rating should go
            int pos = 0;
            while(pos < node->ratings.size() && node->ratings[pos] < rating){
                pos++;
            }

            //add 1 space to the vector and then we'll copy stuff over to put the rating in the correct spot.
            node->ratings.push_back(0);
            node->player_ids.push_back(vector<string>());
            for(int i = node->ratings.size() -1 ; i>pos; i--){
                node->ratings[i]  = node->ratings[i-1];
                node->player_ids[i] = node->player_ids[i - 1];
            }

            node->ratings[pos] = rating;
            node->player_ids[pos] = vector<string>(1, id);
        }else{
            // find which child to go down too
            int pos = 0;
            while(pos < node->ratings.size() && rating >= node->ratings[pos]){
                pos++;
            }

            // if the child found is full then split it first and then rerun the function to place the rating in the correct index.
            if(node->children[pos]->ratings.size() == keys){
                split(node, pos, node->children[pos]);
                if(rating >= node->ratings[pos]){
                    pos++;
                }
            }
            
            insert_node(node->children[pos], rating, id);
        }
    }

    void deleteNode(Node* node){
        if(node == nullptr){
            return;
        }
        //delete all the nodes children and grandchildren etc and then delete the node itself.
        for(int i = 0; i < node->children.size(); i++){
            deleteNode(node->children[i]);
        }
        delete node;
    }

    //This is a helper to get down to the leaf node where a rating should be so I can just reuse it for findPlayersByRating, findPlayersInRange, and removePlayer.
    Node* findLeaf(int rating) const{
        Node* current = root;
        if(!current){
            return{};
        }

        // go down the tree until we hit a leaf and then search it.
        while(!current->isLeaf){
            int pos = 0;
            while(pos<current->ratings.size() && rating >= current->ratings[pos]){
                pos++;
            }
            current = current->children[pos];
        }
        return current;
    }
/*
Still missing a remove player function that will help to dequeue a player and show that they've joined a match and also we 
probably need to implement a range finger that will not only take people that have a specific mmr but also adjecent mmrs. This will help
to actually find a queue if no one is in your mmr range and also since there are only 4 buckets per leaf, it's kind of impossible to fill a match at times.
This should be pretty quick, I just need to traverse down like the find players by rating does and then also traverse horizontally through the next pointer.
*/ 
public:
    ~b_plus_tree(){
        deleteNode(root);
    }

    void insert(int rating, const string& id){
        //if first root.
        if(!root){
            root = new Node(true);
            root->ratings.push_back(rating);
            root->player_ids.push_back(vector<string>{id});
            return;
        }

        if(root->ratings.size() == keys){
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            split(newRoot, 0, root);
            root = newRoot;
        }

        insert_node(root, rating, id);
    }


    vector<string> findPlayersByRating(int rating)const {
        Node* current = findLeaf(rating);

        if(!current){
            return {};
        }

        for(int i = 0; i <current->ratings.size(); i++){
            if(current->ratings[i] == rating){
                return current->player_ids[i];
            }
        }
        return{};
    }
    //still working on this
    vector<string> matchmakePlayers(int minRating, int maxRating)const{
        vector<string> playersInRange;
        Node* current = findLeaf(minRating);

    }

    //remove a player when they join a match so that they're not in queue anymore.
    bool removePlayer(int rating, const string& id){
        Node* current = findLeaf(rating);
        if(!current){
            return false;
        }

        for(int i = 0; i < current->ratings.size(); i++){
            //find the rating inside the leaf
            if(current->ratings[i] == rating){
                // find the player id in this rating's vector
                for(int j = 0; j < current->player_ids[i].size(); j++){
                    if(current->player_ids[i][j] == id){
                        // shift everyone after j one spot to the left and pop the back which deletes that player id.
                        for(int k = j; k < current->player_ids[i].size() - 1; k++){
                            current->player_ids[i][k] = current->player_ids[i][k + 1];
                        }
                        current->player_ids[i].pop_back();
                        return true;
                    }
                }
            }
        return false;
    }
};