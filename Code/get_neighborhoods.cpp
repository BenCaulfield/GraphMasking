#ifndef GET_NEIGHBORHOOD_CPP_
#define GET_NEIGHBORHOOD_CPP_

#include <vector>
#include <list>
#include <iostream>
#include <set>

using namespace std;

//takes in a graph, an int representing the node the BFS starts from, the depth the BFS goes, and a vector of bools all set to false
//the vector of bools are set to true when a node is visited, so the BFS doesn't visit the same node twice.
//returns a vector of ints, representing the result of the BFS
//the different depths for the BFS are seperated by -1s. 
//ex: [0, -1, 1, 2, -1, 3, 4] is a BFS from 0, with 1 & 2 at depth 1, and 3 & 4 at depth 2
vector<int> BFS(vector<list<int> >& graph, int start, int depth, vector<bool>& viewed_nodes){
    vector<int> queue;
    if (graph[start].empty()){return queue;}
    int position = 0;
    viewed_nodes[start] = true;
    queue.push_back(start);
    queue.push_back(-1);
    while(depth > 0){
        while(queue[position] != -1){
            for(list<int>::iterator list_itr = (graph[queue[position]]).begin(); list_itr != (graph[queue[position]]).end(); list_itr++){
                if(!viewed_nodes[*list_itr]){
                    queue.push_back(*list_itr);
                    viewed_nodes[*list_itr] = true;
                }
            }
            position++;
        }
        queue.push_back(-1);
        position++;
        depth--;
    }
    for(int i=0; i<queue.size(); i++){
        if(queue[i] != -1){
            viewed_nodes[queue[i]] = false;
        }
    }
    viewed_nodes[start] = false;
    return queue;
}

//Creates the K_neighborhood graph
void Create_K_Graph(vector<list<int> >& Adjacency_Graph, int k,  vector<set<int> >& K_neighborhood_Graph){
    pair<int, int> temp_pair;
    vector<bool> viewed_nodes(Adjacency_Graph.size(), false);
    set<int> k_adjs;
    K_neighborhood_Graph.push_back(k_adjs);
    for(int i=1; i<Adjacency_Graph.size(); i++){
        vector<int> queue = BFS(Adjacency_Graph, i, k, viewed_nodes);
        K_neighborhood_Graph.push_back(k_adjs);
        for(int j=1; j < queue.size(); j++){
            if(queue[j] != -1){
                K_neighborhood_Graph.back().insert(queue[j]);
            }
        }
    }
}

void Create_K_Graph_Borders(vector<list<int> >& Adjacency_Graph, int k, vector<set<int> >& K_neighborhood_Graph, vector<set<int> >& Kplus1_Border){
    pair<int, int> temp_pair;
    vector<bool> viewed_nodes(Adjacency_Graph.size(), false);
    set<int> k_adjs;
    K_neighborhood_Graph.push_back(k_adjs);
    Kplus1_Border.push_back(k_adjs);   
    for(int i=1; i<Adjacency_Graph.size(); i++){
        vector<int> queue = BFS(Adjacency_Graph, i, k+2, viewed_nodes);
        K_neighborhood_Graph.push_back(k_adjs);
        Kplus1_Border.push_back(k_adjs);
        int depth = 0;
        for(int j=1; j < queue.size(); j++){
            if(queue[j] == -1){depth++; continue; }
            if(depth <= k){K_neighborhood_Graph.back().insert(queue[j]);}
            else{Kplus1_Border.back().insert(queue[j]);}
        }
    }
}

#endif