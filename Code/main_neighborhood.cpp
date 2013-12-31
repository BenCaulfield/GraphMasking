#include "edge_add.cpp"
#include "label_swap.cpp"
#include "merge_groups.cpp"
#include "handle_overlap.cpp"
#include "kSort.cpp"
#include <fstream>
#include <map>
#include <assert.h>



//finds length of shortest path between two nodes in a graph (stops if path-length is less than given int)
int node_distance(int n1, int n2, vector<list<int> >& Graph, int k){
    vector<bool> viewed_nodes(Graph.size(), false);
    vector<int> bfs_return = BFS(Graph, n1, k, viewed_nodes);
    int depth =0;
    for(int i=0; i<bfs_return.size(); i++){
        if(bfs_return[i] == -1){depth++;}
        if(bfs_return[i] == n2){cout << "depth: " << depth << endl; return depth;}
    }
    cout << "not within depth " << k << endl;
}


//outputs K_neighborhood
void Print_Neighborhood(vector<set<int> >& neighborhood, ofstream& out){
    for(int i=0; i<neighborhood.size(); i++){
        for(set<int>::iterator set_itr = neighborhood[i].begin(); set_itr != neighborhood[i].end(); set_itr++){
            out << i << " " << *set_itr << endl;
        }
    }
}

//used to track how often an edge appears (when looking at multiple graphs over same node-set)
struct frequency_edge{
    frequency_edge(int a, int b, int c, string d): node1(a), node2(b), frequency(c), edge_type(d){}
    int node1;
    int node2;
    int frequency;
    string edge_type; //either "original", "valid", or "invalid"
};

//less-than function used for sorting frequency_edges
bool freq_edge_less(frequency_edge e1, int n1, int n2){
    if(e1.node1 < n1){return true;}
    else if(e1.node1 > n1){return false;}
    return e1.node2 < n2;
}

bool edge_freq_less(frequency_edge e1, frequency_edge e2){
    if(e1.frequency < e2.frequency){return true;}
    else if(e1.frequency > e2.frequency){return false;}

    //follows pattern: original < valid < invalid
    if(e1.edge_type == "original" && e2.edge_type != "original"){return true;}
    if(e1.edge_type == "valid"){
       if(e2.edge_type == "original"){return false;}
       else if(e2.edge_type == "invalid"){return true;}
    }
    else if(e1.edge_type == "invalid" && e2.edge_type != "invalid"){return false;}

    if(e1.node1 < e2.node1){return true;}
    else if(e1.node1 > e2.node1){return false;}

    if(e1.node2 < e2.node2){return true;}
    else if(e1.node2 > e2.node2){return false;}

    return false;
}

//main executable
//assumes input file will be list of directed edges (u, v), where each line is of form: u v (no commas)
int main(int argc, char* argv[]){
    srand(clock());
    ofstream adj_dif("adjacency_group_differences.txt");

    //reads in arguments
    if(argc != 4 && argc != 5 && argc != 6 && argc != 7){cout << "Please include input file, k value, edge_add/label_swap, output type (overlap, edge_frequency, both, neither), [repeats, det_merge/nondet_merge] \n"; return 0;}
    ifstream in_str(argv[1]);
    int k = atoi(argv[2]);
    bool edge_add = false;
    string test_profs("edge_add");
    if(argv[3] == test_profs){edge_add = true;}
    string output_type = "neither";
    if(argc >= 5){output_type = argv[4];}
    int repeats = 1;
    if(argc >= 6){repeats = atoi(argv[5]);}
    string merge = "none";
    if(argc >= 7){merge = argv[6];}

    //declares basic variables
    string s;
    vector<list<int> > Adjacency_graph;
    vector<list<int> > Final_graph;
    list<int> l;
    set<int> a_set;

    //reads in all the edges, and creates the adjacency graph
    while(!in_str.eof()){
        in_str >> s;
        int source = atoi(s.c_str());
        in_str >> s;
        int target = atoi(s.c_str());

        while(Adjacency_graph.size() <= source){Adjacency_graph.push_back(l);}

        Adjacency_graph[source].push_back(target);
    }

    //declares more variables
    vector<set<int> > K_neighborhood;
    Create_K_Graph(Adjacency_graph, k, K_neighborhood); //Problem should be here!
    list<frequency_edge> edge_frequencies;
    vector<list<int> > K_neighborhood_lists;
    vector<int> clique_lookup;
    vector<int> group_size;
    vector<int> temp_vec;
    list<vector<int> >adj_groups; adj_groups.push_back(temp_vec);
    vector<vector<int> > same_cliques;
    vector<adj_itr> adj_map;

    //finds adjacency_groups for label-swapping algorithm
    if(!edge_add){
        for(int i=0; i<K_neighborhood.size(); i++){
            list<int> l;
            for(set<int>::iterator sitr = K_neighborhood[i].begin(); sitr != K_neighborhood[i].end(); sitr++){
                l.push_back(*sitr);
            }
            K_neighborhood_lists.push_back(l);
        }
        kSort(K_neighborhood, adj_groups, adj_map);

        //super bad. fix later
        for(adj_itr itr1 = adj_groups.begin(); itr1 != adj_groups.end(); itr1++){
            same_cliques.push_back(*itr1);
        }


        //assembles clique_lookup (allows constant time checking if two nodes are in all same cliques)
        //clique_lookup[i]=j if node i is in the jth element of adj_groups
        clique_lookup.insert(clique_lookup.begin(), K_neighborhood.size(), 0);
        group_size.insert(group_size.begin(), K_neighborhood.size(), 0);
        for(int i=0; i<same_cliques.size(); i++){
            for(int j=0; j<same_cliques[i].size(); j++){
                clique_lookup[same_cliques[i][j]] = i;
                group_size[same_cliques[i][j]] = same_cliques[i].size();
            }
        }
    }

    for(int i=0; i<repeats; i++){
        if(edge_add){
            Final_graph.insert(Final_graph.begin(), K_neighborhood.size(), l);
            Edge_Adding(K_neighborhood, Final_graph, k);
        }
        else{
            list<vector<int> > temp_adj_groups = adj_groups;
            if(merge == "nondet_merge"){
                int maximum_difference = 5; //the maximum difference between any two adjacency groups that can be merged
                int total_difference = 1000000; //the sum of the differences between merged adjacency groups

                //non-deterministically merges similar adjacency groups
                //algorithm stops when all adjacency groups have difference greater than maximum_difference, or when sum of differences
                //of merged adjacency groups is greater than total_differences
                merge_similar_adj_groups(maximum_difference, total_difference, Adjacency_graph, K_neighborhood, temp_adj_groups, adj_map);
            }
            else if(merge == "det_merge"){
                int maximum_difference = 5;
                //deterministically merges closest adjacency groups until all remaining groups have a difference greater than maximum difference
                deterministic_merge2(Adjacency_graph, K_neighborhood, temp_adj_groups, adj_map, maximum_difference);
            }
            ofstream merged_adj("merged_adj");
            for(adj_itr i=temp_adj_groups.begin(); i!=temp_adj_groups.end(); i++){
                for(int j=0; j<(*i).size(); j++){
                    merged_adj << (*i)[j] << " ";
                }
                merged_adj << endl;
            }
            Label_Swap(Adjacency_graph, Final_graph, temp_adj_groups);
        }

        //updates list of edge-frequencies based on Final_graph
        list<frequency_edge>::iterator freq_itr = edge_frequencies.begin();
        for(int j=1; j<Final_graph.size(); j++){
            Final_graph[j].sort();
            list<int>::iterator itr = Final_graph[j].begin();
            while(*itr < j && itr != Final_graph[j].end()){itr++;}
            while(itr != Final_graph[j].end()){
                //cout << j << " " << *itr << endl;
                //cout << freq_itr -> edge_type;
                if(freq_itr != edge_frequencies.end() && freq_edge_less(*freq_itr, j, *itr)){freq_itr++;}
                else if(freq_itr != edge_frequencies.end() && freq_itr->node1==j && freq_itr->node2==*itr){(freq_itr->frequency)++;freq_itr++;itr++;}
                else{//adds new frequency edge to list
                    //assigns type to frequency edge
                    string edge_type = "original";
                    bool in_original = false;
                    for(list<int>::iterator listitr = Adjacency_graph[j].begin(); listitr != Adjacency_graph[j].end(); listitr++){
                        if(*listitr == *itr){in_original = true; break;}
                    }
                    if(!in_original){
                        if(K_neighborhood[j].find(*itr) == K_neighborhood[j].end()){edge_type = "invalid";}
                        else{edge_type = "valid";}
                    }

                    frequency_edge t(j, *itr, 1, edge_type);
                    edge_frequencies.insert(freq_itr, t);
                    itr++;
                }
            }
        }
        if(i+1 < repeats){Final_graph.clear();}
    }
    //outputs overlap from last Final_graph
    if(output_type == "both" || output_type == "overlap"){
        ofstream overlap_out("overlap.txt");
        vector<set<int> > New_K_neighborhood;
        Create_K_Graph(Final_graph, k, New_K_neighborhood);
        output_overlap(overlap_out, K_neighborhood, New_K_neighborhood);
    }

    //outputs frequencies from edge_frequencies
    //calculates relevant statistics
    if(output_type == "both" || output_type == "edge_frequency"){
        ofstream freq_out("edge_frequencies.txt");
        ofstream edge_out("all_edge_frequencies.txt");
        vector<int> frequencies;
        while(frequencies.size() <= repeats){frequencies.push_back(0);}
        for(list<frequency_edge>::iterator itr=edge_frequencies.begin(); itr!=edge_frequencies.end(); itr++){
            frequencies[itr->frequency]++;
            edge_out << itr->node1 << " " << itr->node2 << " " << itr->frequency << " " << itr -> edge_type << endl;
        }

    }

    //outputs last final_graph
    ofstream out_str("output.txt");
    for(int i=0; i<Final_graph.size(); i++){
        for(list<int>::iterator listitr = Final_graph[i].begin(); listitr != Final_graph[i].end(); listitr++){
            out_str << i << " " << *listitr << endl;
        }
    }

}
