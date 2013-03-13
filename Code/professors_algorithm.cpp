#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <set>
#include <list>
#include <algorithm>
#include <assert.h>
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
    cout << "A " << w.node1 << ":" << w.node2 << " " << w.back_itr1 -> node << ":" << w.back_itr1 -> working_position << endl;
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

void delete_working_element(vector<working_edge>& working_list, int position){
    //cout << position << " " << working_list.size() << endl;
    if(position == -1){return;}
    working_list[position].back_itr1 -> working_position = -1;
    working_list[position].back_itr2 -> working_position = -1;
    if(position+1 == working_list.size()){working_list.pop_back(); return;}
    working_list[position] = working_list.back();
    working_list[position].back_itr1 -> working_position = position;
    working_list[position].back_itr2 -> working_position = position;
    working_list.pop_back();
}

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
    for(int i=0; i<Adjacency_Graph.size(); i++){
        vector<int> queue = BFS(Adjacency_Graph, i, k, viewed_nodes);
        set<int> k_adjs;
        for(int j=1; j < queue.size(); j++){
            if(queue[j] != -1){k_adjs.insert(queue[j]);}
        }
        K_neighborhood_Graph.push_back(k_adjs);
    }
}


/*
void randomize_working_graph(list<pair<int, int> >& working_graph){
    vector<pair<int, int> > temp;
    for(list<pair<int, int> >::iterator list_itr = working_graph.begin(); list_itr != working_graph.end(); list_itr++){
        temp.push_back(*list_itr);
    }
    working_graph.clear();
    for(int i=temp.size() - 2; i > 0; i--){
        int r = rand() % i;
        working_graph.push_back(temp[r]);
        temp[r] = temp[i + 1];
    }
}
*/
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
    //delete_working_element(working_list, position);
    return return_edge;
}


/*
pair<int, int> get_random_pair(list<pair<int, int> >& working_list){
    int position = rand() % working_list.size();
    list<pair<int, int> >::iterator itr = working_list.begin();
    while(position > 0){
        itr++;
        position--;
    }
    pair<int, int> temp = *itr;
    working_list.erase(itr);
    return temp;
}
*/
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


void newProfs(vector<set<int> >& K_neighborhood, vector<list<int> >& Final_graph, int k){
    
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



void Prof_Alg(vector<set<int> >& old_K_neighborhood, vector<list<int> >& Final_graph, int k){
    //cout << "Beginning algorithm" << endl;
    vector<set<int> > K_1_neighborhood;
    set<int> a_set;
    K_1_neighborhood.insert(K_1_neighborhood.begin(), old_K_neighborhood.size(), a_set); //old-k-neighborhood.size() may not be big enough

    ofstream out("profs_out");
    long int t1 = 0;
    long int t2 = 0;
    long int t3 = 0;
    long int t4 = 0;
    long int current_time;
    long int t1_1 = 0;
    long int t1_2 = 0;
    long int t1_3 = 0;

    vector<K_set> K_neighborhood;
    vector<working_edge> working_list;
    Assemble_Working_List(old_K_neighborhood, K_neighborhood, working_list);
    vector<bool> viewed_nodes(K_neighborhood.size(), false);
    current_time = clock();
    int original_wl = working_list.size();
    int num_edges = 0;
    int num_edges2 = 0;
    while(!working_list.empty()){
        num_edges2++;
        //Print_Adjacency_Graph(Final_graph);

        current_time = clock();

        int current_edge_position;
        pair<int, int> current_edge = get_random_edge(working_list, current_edge_position);
        t1_1 += clock() - current_time;
        current_time = clock();

        delete_working_element(working_list, current_edge_position);
        Final_graph[current_edge.first].push_back(current_edge.second);
        Final_graph[current_edge.second].push_back(current_edge.first);


        pair<int,int> temp_edge;
        set<pair<int,int> > k_1_edges;
        //shortcut for when k==2.
        if(k==2){
            k_1_edges.insert(current_edge);
            /*for(list<int>::iterator itr = Final_graph[current_edge.first].begin(); itr!=Final_graph[current_edge.first].end(); itr++){
                if(*itr == current_edge.second){continue;}
                temp_edge.first = min(*itr, current_edge.second);
                temp_edge.second = max(*itr, current_edge.second);
                k_1_edges.insert(temp_edge);
            }
            for(list<int>::iterator itr = Final_graph[current_edge.second].begin(); itr!=Final_graph[current_edge.second].end(); itr++){
                if(*itr == current_edge.first){continue;}
                temp_edge.first = min(*itr, current_edge.first);
                temp_edge.second = max(*itr, current_edge.first);
                k_1_edges.insert(temp_edge);
            }*/
        }

        else{

            viewed_nodes[current_edge.second] = true;
            vector<int> source_BFS = BFS(Final_graph, current_edge.first, k-1, viewed_nodes);
            viewed_nodes[current_edge.second] = false; viewed_nodes[current_edge.first] = true;

            t1_2 += clock() - current_time;
            current_time = clock();

            vector<int> target_BFS = BFS(Final_graph, current_edge.second, k-1, viewed_nodes);
            viewed_nodes[current_edge.first] = false;

            t1_3 += clock() - current_time;
            current_time = clock();

            //Finds all edges less than k-1 from both nodes in the "current_edge"
            //set<pair<int, int> > k_1_edges;
            int source_depth = 0;
            for(int i=0; source_depth < k-1 && i < source_BFS.size(); i++){
                if(source_BFS[i] == -1){source_depth++; continue;}
                int target_depth = 0; int j = 0;
                while(target_depth + source_depth < k-1 && j < target_BFS.size()){
                    if(target_BFS[j] == -1){j++; target_depth++; continue;}
                    pair<int, int> new_edge;
                    new_edge.first = min(source_BFS[i], target_BFS[j]);
                    new_edge.second = max(source_BFS[i], target_BFS[j]);
                    k_1_edges.insert(new_edge);
                    j++;
                }
            }


            //out << clock() - current_time << " ";
            t2 += clock() - current_time;
            current_time = clock();

        }

            //Finds out which of the k-1 edges are actually new
            //set<pair<int, int> > new_edges = k_1_edges; set< pair<int, int> >::iterator set_itr;
            set<pair<int, int> >::iterator set_itr = k_1_edges.begin();
            set<pair<int,int> > new_edges;
            while(set_itr != k_1_edges.end()){
                //if the edge is new, adds it to new_edges
                bool is_new_edge = (K_1_neighborhood[set_itr -> first].insert(set_itr -> second)).second;

                if(is_new_edge){new_edges.insert(*set_itr);}
                set_itr++;
            }

            //out << clock() - current_time << " ";
            t3 += clock() - current_time;
            current_time = clock();


        //finds new edges to remove from working_graph
        for(set_itr = new_edges.begin(); set_itr != new_edges.end(); set_itr++){
            K_set& source_adj = K_neighborhood[set_itr ->first];
            K_set& target_adj = K_neighborhood[set_itr -> second];
            K_set::iterator source_itr = source_adj.begin();
            K_set::iterator target_itr = target_adj.begin();
            pair<int, int> temp_edge;
            while(source_itr != source_adj.end() && target_itr != target_adj.end()){
                if((source_itr->node) == (target_itr->node)){source_itr++; target_itr++;}
                else if(source_itr->node > target_itr->node){ //if
                    delete_working_element(working_list, target_itr->working_position);
                    target_itr++;
                }
                else if((source_itr->node) < (target_itr->node)){
                    delete_working_element(working_list, source_itr->working_position);
                    source_itr++;
                }
            }
            while(source_itr != source_adj.end()){
                delete_working_element(working_list, source_itr->working_position);
                source_itr++;
            }
            while(target_itr != target_adj.end()){
                delete_working_element(working_list, target_itr->working_position);
                target_itr++;
            }
    }

    //out << clock() - current_time << " ";
    t4 += clock() - current_time;
    current_time = clock();

    //out << "\n";

    //print progress:
    cout << "___" << endl;
    cout << current_edge.first << ":" << current_edge.second << endl;
    for(int i=0; i< working_list.size(); i++){
        cout << working_list[i].node1 << ":" << working_list[i].node2 << " ";
    }
    cout << endl;
    cout << new_edges.size() << endl;

    num_edges++;
    }
   // t1 = t1_1+t1_2+t1_3;
  //cout << t1_1 << ":" << t1_2 << ":" << t1_3 << "=" << t1 << " " << t2 << " " << t3 << " " << t4 << ": " << t1+t2+t3+t4 << endl;
  /*cout << "num_edges: " << num_edges << " " << num_edges2 << endl;
  cout << "k_edges: " << original_wl << endl;
  cout << "average_degree: " << (double)num_edges / (double)(K_neighborhood.size()-1) << endl;
  cout << "num_nodes: " << K_neighborhood.size()-1 << endl;
  int maxk_degree = 0;
  int total_K = 0;
  for(int i=0; i<K_neighborhood.size();i++){maxk_degree=max(maxk_degree, (int)K_neighborhood[i].size()); total_K+=K_neighborhood[i].size();}
  int maxf_degree = 0;
  for(int i=0; i<Final_graph.size();i++){maxf_degree=max(maxf_degree, (int)Final_graph[i].size());}
  cout << "max k-degree: " << maxk_degree << endl;
  cout << "max final_graph degree: " << maxf_degree << endl;
  cout << "average K-neighborhood: " << (double)total_K / (double)(K_neighborhood.size()-1) << endl;*/

  vector<set<int> > sample_k_1;
  Create_K_Graph(Final_graph, k-1, sample_k_1);
  cout << "sample: " << sample_k_1.size() << endl;
  int index = 0;
  while(index<sample_k_1.size() && index < K_1_neighborhood.size()){
    set<int>::iterator s_itr = sample_k_1[index].begin();
    set<int>::iterator k_itr = K_1_neighborhood[index].begin();
    //if(sample_k_1[index].size() > K_1_neighborhood[index].size()){cout << "missing k-1 edges" << endl;}
    while(s_itr != sample_k_1[index].end() && k_itr != K_1_neighborhood[index].end()){
        if(*s_itr < index){s_itr++; continue;}
        if(*s_itr < *k_itr){cout << "missing: " << index << " " << *s_itr << endl; s_itr++;}
        else if(*k_itr < *s_itr){cout << "extra: " << index << " " << *k_itr << endl; k_itr++;}
        else{s_itr++; k_itr++;}
    }
    index++;
  }


}

/*
void test_edges(vector<list<int> >& Adjacency_graph, vector<list<int> >& Final_graph, vector<set<int> >&K_1_neighborhood,  int k){
    list<pair<int, int> > working_list;
    vector<set<int> > K_neighborhood;
    Create_K_Graph(Adjacency_graph, k, K_neighborhood, working_list);
    int edge_count = 0;
    vector<list<int> > Final_temp = Final_graph;

    ofstream out_str("output.txt");
    for(int times=0; times < 10; times++){
        Prof_Alg(working_list, K_neighborhood, K_1_neighborhood, Final_graph, k);
        int counter = 0;
        for(int i=0; i< Final_graph.size(); i++){
            for(list<int>::iterator list_int = Final_graph[i].begin(); list_int != Final_graph[i].end(); list_int++){
                out_str << i << " " << *list_int << endl;
                if(i == 1 && *list_int == 82){edge_count++;}
                counter++;
            }
        }
        cout << "Final Graph Size: " << counter << endl;
        Final_graph = Final_temp;
    }
    cout << "edge appeared: " << edge_count << " times" << endl;
}
*/


