#include <fstream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include "get_neighborhoods.cpp"
//#include "BFS.cpp"

using namespace std;

//A structure used in K_set (see below)
struct K_struct{
    K_struct(int n, int p): node(n), working_position(p), satisfied(false) {}
    int node;
    mutable int working_position;
    mutable bool satisfied;
    bool operator<(const K_struct& other) const {
        return node < other.node;
    }
};

//This set is used in the K_neighborhoods
//for the K_struct <i,j> of ints in K_neighborhood[i], (i, j) is the edge being viewed,
// and k points to the edges position in the working graph
typedef set<K_struct> K_set;

//(node1, node2) represents an edge, where node1 < node2
// back_itr1 points to the position of (node1, node2) in the K_neighborhood graph,
// and back_itr2 points to the position of (node2, node1) in the K_neighborhood graph.
struct working_edge {
    int node1;
    int node2;
    K_set::iterator back_itr1;
    K_set::iterator back_itr2;
    //operator=(const other&){node1=other.node1 }
};

void print_working_edge(working_edge& w){
    cout << w.node1 << ":" << w.node2 << " " << w.back_itr1 -> node << ":" << w.back_itr1 -> working_position << endl;
}

void Assemble_Working_List(vector<set<int> >& old_K_Graph, vector<K_set>& new_K_Graph, vector<working_edge>& working_list){
    K_set temp_set;
    new_K_Graph.resize(old_K_Graph.size(), temp_set);
    for(int i=0; i<old_K_Graph.size(); i++){
        working_edge temp_edge; temp_edge.node1 = i;
        for(set<int>::iterator set_itr = old_K_Graph[i].begin(); set_itr != old_K_Graph[i].end(); set_itr++){
            if(i < *set_itr){
                temp_edge.node2 = *set_itr;
                K_struct K_temp(*set_itr, working_list.size());
                temp_edge.back_itr1 = new_K_Graph[i].insert(K_temp).first;
                K_temp.node = i;
                temp_edge.back_itr2 = new_K_Graph[*set_itr].insert(K_temp).first;
                working_list.push_back(temp_edge);
            }
        }
    }
}

bool operator<(pair<int, int>& p1, pair<int, int>& p2){
    if(p1.first < p2.first){return true;}
    else if(p1.first > p2.first){return false;}
    else if(p1.second < p2.second){return true;}
    return false;
}

pair<int, int> get_random_edge(vector<working_edge>& working_list, int& position){
    position = rand() % working_list.size();
    pair<int, int> return_edge;
    return_edge.first = working_list[position].node1;
    return_edge.second = working_list[position].node2;
    return return_edge;
}

//Prints K_neighborhood
void Print_Adjacency_Graph(vector<list<int> >& neighborhood){
    cout << "GRAPH: " << endl;
    for(int i=0; i<neighborhood.size(); i++){
        for(list<int>::iterator list_itr = neighborhood[i].begin(); list_itr != neighborhood[i].end(); list_itr++){
            cout << i << " " << *list_itr << endl;
        }
    }
}

//gets random edge from working list, removes that edge from list
pair<int, int> get_rand_edge(vector<pair<int, int> >& working_list){
    int position = rand() % working_list.size();
    pair<int, int> return_edge;
    return_edge.first = working_list[position].first;
    return_edge.second = working_list[position].second;
    working_list[position] = working_list.back();
    working_list.pop_back();
    return return_edge;
}


void Edge_Adding(vector<set<int> >& K_neighborhood, vector<list<int> >& Final_graph, int k){

    //Fills working list with all elements of K_neighborhood
    vector<pair<int, int> > working_list;
    for(int i=0; i<K_neighborhood.size(); i++){
        for(set<int>::iterator itr = K_neighborhood[i].begin(); itr != K_neighborhood[i].end(); itr++){
            pair<int, int> new_edge;
            if(i < *itr){
                new_edge.first = i;
                new_edge.second = *itr;
                working_list.push_back(new_edge);
            }
        }
    }

    vector<bool> viewed_nodes(K_neighborhood.size(), false);

    cout << viewed_nodes.size() << " " << K_neighborhood.size() << " " << Final_graph.size() << " " << working_list.size() << endl;
    while(working_list.size() != 0){
        pair<int, int> current_edge = get_rand_edge(working_list);

        viewed_nodes[current_edge.second] = true;
        vector<int> source_BFS = BFS(Final_graph, current_edge.first, k-1, viewed_nodes);
        viewed_nodes[current_edge.second] = false;

        viewed_nodes[current_edge.first] = true;
        vector<int> target_BFS = BFS(Final_graph, current_edge.second, k-1, viewed_nodes);
        viewed_nodes[current_edge.first] = false;


        bool violating_edge = false;
        int source_depth = 0;
        for(int i=0; i<source_BFS.size(); i++){
            if(source_BFS[i] == -1){source_depth++; continue;}
            int target_depth = 0;
            bool end_loop = false;
            for(int j=0; j<target_BFS.size() && target_depth + source_depth < k; j++){
                if(target_BFS[j] == -1){target_depth++; continue;}
                if(K_neighborhood[i].find(j) == K_neighborhood[i].end()){
                    end_loop = true;
                    violating_edge = true;
                    break;
                }
            }
            if(end_loop){break;}
        }


        if(violating_edge){continue;}

        Final_graph[current_edge.first].push_back(current_edge.second);
        Final_graph[current_edge.second].push_back(current_edge.first);

    }

}

