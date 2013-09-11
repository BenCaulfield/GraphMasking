#include "professors_algorithm.cpp"
#include "my_algorithm.cpp"
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


//Prints K_neighborhood
void Print_Neighborhood(vector<set<int> >& neighborhood, ofstream& out){
    for(int i=0; i<neighborhood.size(); i++){
        for(set<int>::iterator set_itr = neighborhood[i].begin(); set_itr != neighborhood[i].end(); set_itr++){
            out << i << " " << *set_itr << endl;
        }
    }
}

//used to track how often an edge appears (when looking at multiple graphs over same node-set)
struct frequency_edge{
    frequency_edge(int a, int b, int c): node1(a), node2(b), frequency(c){}
    int node1;
    int node2;
    int frequency;
};

bool edge_freq_less(frequency_edge e1, int n1, int n2){
    if(e1.node1 < n1){return true;}
    else if(e1.node1 > n1){return false;}
    return e1.node2 < n2;
}

//removes edge (node1, node2) and adds edge (node1, node3)
//Graph remains sorted
void exchange_edge(vector<list<int> >& Graph, int node1, int node2, int node3){
    bool found_3 = false;
    bool found_2 = false;
    for(list<int>::iterator itr=Graph[node1].begin(); !found_2 || !found_3;){
        if(itr==Graph[node1].end()){Graph[node1].insert(itr, node3); found_3 = true; assert(found_2); break;}
        else if(*itr == node3){found_3=true;}
        else if(*itr > node3 && !found_3){Graph[node1].insert(itr, node3); found_3=true;}
        if(*itr==node2){itr = Graph[node1].erase(itr); found_2=true;}
        else{itr++;}
    }
    for(list<int>::iterator itr=Graph[node2].begin(); itr != Graph[node2].end(); itr++){
        if(*itr==node1){Graph[node2].erase(itr); break;}
    }
    for(list<int>::iterator itr=Graph[node3].begin(); true; itr++){
        if(itr==Graph[node3].end() || *itr > node1){Graph[node3].insert(itr, node1); break;}
    }
}

//returns an int representing a node that is adjacent to s2 and a K_neighbor of s1
int Find_swap_edge(int s1, int s2, vector<list<int> >& Final_graph){
    vector<int> random_vec;
    for(list<int>::iterator itr2 = Final_graph[s2].begin(); itr2 != Final_graph[s2].end(); itr2++){
        random_vec.push_back(*itr2);
    }
    randomize_vector(random_vec);
    for(int j=0; j<random_vec.size(); j++){
        for(list<int>::iterator itr2=Final_graph[random_vec[j]].begin(); itr2 != Final_graph[random_vec[j]].end(); itr2++){
            if(*itr2==s1){return random_vec[j];}
        }
    }
    return -1;
}
//bool operator< (pair<int,int>& p1, pair<int,int>& p2){return p1.first<p2.first || (p1.first==p2.first && p1.second<p2.second);}


//ASSUMES THAT INPUT FILE WILL BE IN THE FORMAT OF THE RANDOM NETWORK GRAPHS
int main(int argc, char* argv[]){
    srand(clock());
    ofstream adj_dif("adjacency_group_differences.txt");

    //reads in arguments
    if(argc != 4 && argc != 5 && argc != 6){cout << "Please include input file, k value, profs/my, output type (overlap, edge_frequency, both, neither), [and maybe repeats] \n"; return 0;}
    ifstream in_str(argv[1]);
    int k = atoi(argv[2]);
    bool profs = false;
    string test_profs("profs");
    if(argv[3] == test_profs){profs = true;}
    string output_type = "neither";
    if(argc >= 5){output_type = argv[4];}
    int repeats = 1;
    if(argc == 6){repeats = atoi(argv[5]);}
    string s;
    vector<list<int> > Adjacency_graph;
    vector<list<int> > Final_graph;
    list<int> l;
    set<int> a_set;
    int max_node = 0;
    //BAD FORM!!!!
    if(in_str.peek() == 'n'){
        in_str >> s;
        while(s!="###"){in_str >> s;}
    }

    //reads in all the edges, and creates the adjacency graph
    while(!in_str.eof()){

        in_str >> s;
        int source = atoi(s.c_str());
        in_str >> s;
        int target = atoi(s.c_str());

        //list<int> l; set<int> a_set;
       // cout << source << " " << target << "." << endl;
        while(Adjacency_graph.size() <= source){Adjacency_graph.push_back(l);}

        max_node = max(max_node, max(source, target));
        Adjacency_graph[source].push_back(target);
    }

    /*
    //used to investigate LinkedIn data
    int max_degree = 0;
    double total_degree = 0;
    double triangles = 0;
    double possible_triangles = 0;
    for(int i=1; i<Adjacency_graph.size(); i++){
        for(list<int>::iterator itr=Adjacency_graph[i].begin(); *itr < i; itr++){
            for(list<int>::iterator itr2=Adjacency_graph[*itr].begin(); itr2 != Adjacency_graph[*itr].end(); itr2++){
                for(list<int>::iterator itr3=Adjacency_graph[*itr2].begin(); *itr3 <= i; itr3++){
                    if(*itr3 == i){triangles++;}
                }
            }
            possible_triangles++;
        }
        if(Adjacency_graph[i].size() > max_degree){max_degree = Adjacency_graph[i].size();}
        total_degree += Adjacency_graph[i].size();
    }
    cout << "stats: " <<  max_degree << " " << total_degree / (double)(Adjacency_graph.size()) << " " << triangles << " " << possible_triangles;
    return 0;*/

    vector<set<int> > K_neighborhood;
    vector<set<int> > Kplus1_Borders;
    Create_K_Graph_Borders(Adjacency_graph, k, K_neighborhood, Kplus1_Borders);
    list<frequency_edge> edge_frequencies;
    vector<list<int> > K_neighborhood_lists;
    vector<int> clique_lookup;
    vector<int> group_size;
    vector<int> temp_vec;
    list<vector<int> >adj_groups; adj_groups.push_back(temp_vec);
    vector<vector<int> > same_cliques;
    vector<adj_itr> adj_map;

    //outputs k_neighborhood:
    ofstream K_neigh_out("K_neighborhood");
    for(int i=1; i<K_neighborhood.size(); i++){
        K_neigh_out << i << " ";
        for(set<int>::iterator set_itr = K_neighborhood[i].begin(); set_itr != K_neighborhood[i].end(); set_itr++){
            K_neigh_out << *set_itr << " ";
        }
        K_neigh_out << endl << endl;
    }

    //finds the cliques necessary for the label-swapping algorithm
    if(!profs){
        for(int i=0; i<K_neighborhood.size(); i++){
            list<int> l;
            for(set<int>::iterator sitr = K_neighborhood[i].begin(); sitr != K_neighborhood[i].end(); sitr++){
                l.push_back(*sitr); //cout << *sitr << " ";
            }
            K_neighborhood_lists.push_back(l);
        }
        //get_cliques(K_neighborhood_lists, k, adj_groups, clique_lookup, group_size);
        kSort(K_neighborhood, adj_groups, adj_map);

        //super bad. fix later
        for(adj_itr itr1 = adj_groups.begin(); itr1 != adj_groups.end(); itr1++){
            same_cliques.push_back(*itr1);
        }


        //assembles clique_lookup (allows const-time checking if two nodes are in all same cliques)
        //clique_lookup[i]=j if node i is in the jth element of adj_groups
        clique_lookup.insert(clique_lookup.begin(), K_neighborhood.size(), 0);
        group_size.insert(group_size.begin(), K_neighborhood.size(), 0);
        for(int i=0; i<same_cliques.size(); i++){
            for(int j=0; j<same_cliques[i].size(); j++){
                clique_lookup[same_cliques[i][j]] = i;
                group_size[same_cliques[i][j]] = same_cliques[i].size();
            }
        }

        ofstream same_out("same_cliques");
        int lone_nodes = 0;
        for(int i=0; i<same_cliques.size(); i++){
            same_out << i << ": "; if(same_cliques[i].size()==1){lone_nodes++;}
            for(int j=0; j<same_cliques[i].size(); j++){
                same_out << same_cliques[i][j] << " ";
            }
            same_out << "\n";
        }
        same_out << "lone nodes: " <<lone_nodes << endl;
    }
    for(int i=0; i<repeats; i++){
        if(profs){
            Final_graph.insert(Final_graph.begin(), K_neighborhood.size(), l);
            //cout << "A" << endl;
            newProfs(K_neighborhood, Final_graph, k);
            //cout << "B" << endl;
        }
        else{
            list<vector<int> > temp_adj_groups = adj_groups;
            //cout << "A" << endl;
            //merge_similar_adj_groups(5, 20000000000, Adjacency_graph, K_neighborhood, temp_adj_groups, adj_map);
            wesley_merge(Adjacency_graph, K_neighborhood, temp_adj_groups, adj_map, int mergecap, bool can_remerge);
            ofstream merged_adj("merged_adj");
            for(adj_itr i=temp_adj_groups.begin(); i!=temp_adj_groups.end(); i++){
                for(int j=0; j<(*i).size(); j++){
                    merged_adj << (*i)[j] << " ";
                }
                merged_adj << endl;
            }
            //cout << "B" << endl;
            my_algorithm(Adjacency_graph, Final_graph, temp_adj_groups);
        }

        //updates list of edge-frequencies based on Final_graph
        list<frequency_edge>::iterator freq_itr = edge_frequencies.begin();
        for(int j=1; j<Final_graph.size(); j++){
            Final_graph[j].sort(); // MAY BE INEFFICIENT!!!!!
            list<int>::iterator itr = Final_graph[j].begin();
            while(*itr < j && itr != Final_graph[j].end()){itr++;}
            while(itr != Final_graph[j].end()){
                //cout << j << " " << *itr << endl;
                if(edge_freq_less(*freq_itr, j, *itr)){freq_itr++;}
                else if(freq_itr->node1==j && freq_itr->node2==*itr){(freq_itr->frequency)++;freq_itr++;itr++;}
                else {frequency_edge t(j, *itr, 1); edge_frequencies.insert(freq_itr, t); itr++;}
            }
        }
        if(i+1 < repeats){Final_graph.clear();}
    }
    //outputs overlap from last Final_graph
    if(output_type == "both" || output_type == "overlap"){
        ofstream overlap_out("overlap.txt");
        vector<set<int> > New_K_neighborhood;
        vector<set<int> > New_Borders;
        Create_K_Graph_Borders(Final_graph, k, New_K_neighborhood, New_Borders);
        modified_output_overlap(overlap_out, K_neighborhood, New_K_neighborhood, Kplus1_Borders, New_Borders, k);

        ofstream newK("K_neighborhoods_Borders");
        for(int i=0; i<New_K_neighborhood.size(); i++){
            newK << i << ": " << endl;
            set<int>::iterator itr1 = K_neighborhood[i].begin();
            newK << "K_neighborhood: ";
            while(itr1 != K_neighborhood[i].end()){newK << *itr1 << " "; itr1++;}
            newK << endl;
            set<int>::iterator itr2 = Kplus1_Borders[i].begin();
            newK << "K_Borders: ";
            while(itr2 != Kplus1_Borders[i].end()){newK << *itr2 << " "; itr2++;}
            newK << endl;
            set<int>::iterator itr3 = New_K_neighborhood[i].begin();
            newK << "New_K_neighborhood: ";
            while(itr3 != New_K_neighborhood[i].end()){newK << *itr3 << " "; itr3++;}
            newK << endl;
            set<int>::iterator itr4 = New_Borders[i].begin();
            newK << "New_Borders: ";
            while(itr4 != New_Borders[i].end()){newK << *itr4 << " "; itr4++;}
            newK << endl; newK << endl;
        }
       // newK << "_____________________BORDERS_______________________" << endl;
       /*for(int i=0; i<New_Borders.size(); i++){
            newK << i << ": ";
            for(set<int>::iterator itr = New_Borders[i].begin(); itr != New_Borders[i].end(); itr++){
                    newK << *itr << " ";
            }
            newK << endl;
        }

        K_neigh_out << "_____________________BORDERS_______________________" << endl;
        for(int i=0; i<Kplus1_Borders.size(); i++){
            K_neigh_out << i << ": ";
            for(set<int>::iterator itr = Kplus1_Borders[i].begin(); itr != Kplus1_Borders[i].end(); itr++){
                    K_neigh_out << *itr << " ";
            }
            K_neigh_out << endl;
        }*/

    }

    //outputs frequencies from edge_frequencies
    //calculates relevant statistics
    if(output_type == "both" || output_type == "edge_frequency"){
        ofstream freq_out("edge_frequencies.txt");
        ofstream edge_out("all_edge_frequencies.txt");
        vector<int> frequencies;
        while(frequencies.size() <= repeats){frequencies.push_back(0);}
        for(list<frequency_edge>::iterator itr=edge_frequencies.begin(); itr!=edge_frequencies.end(); itr++){
            //while(frequencies.size() <= itr->frequency){frequencies.push_back(0);}
            frequencies[itr->frequency]++;
            edge_out << itr->node1 << " " << itr->node2 << " " << itr->frequency << endl;
        }

        //Types of "const-edges"
        //ofstream const_out("constant_edges.txt");
        int const_inner_edges = 0; //num edges between two nodes in the same adj_group that are constant
        int all_inner_edges = 0;
        int complete_group_size = 0;
        int outer_edges = 0; //num edges between two nodes of different adj_groups
        int full_singletons = 0; //edges between two singelton adj_groups
        int const_half_singletons = 0; //half singletons that are constant
        int half_singletons = 0; //edges where one node is in singleton adj-group
        int potential_fulls = 0; //number of k_neighborhood edges between singleton-adj-groups
        int double_groups = 0; //adj_groups with just two nodes

        if(!profs){
            //cout << "C" << endl;
            for(list<frequency_edge>::iterator itr = edge_frequencies.begin(); itr != edge_frequencies.end(); itr++){
                //cout << "0" << endl;
                //cout << group_size.size() << " " << itr->node1 << " " << itr->node2 << endl;
                //cout << group_size[itr->node1] << " " << group_size[itr->node2] << endl;
                //cout << "0.1" << endl;
                if(group_size[itr->node1]==1 && group_size[itr->node2]==1){/*cout << "@" << endl;*/full_singletons++;}
                else if(group_size[itr->node1]==1 || group_size[itr->node2]==1){half_singletons++;}
                //cout << "1" << endl;
                if(clique_lookup[itr->node1]==clique_lookup[itr->node2]){all_inner_edges++;}
                if(itr -> frequency == repeats){
                    if(clique_lookup[itr->node1]==clique_lookup[itr->node2]){/*cout << "2.1" << endl;*/ const_inner_edges++; complete_group_size += group_size[itr->node1];}
                    else{ //cout << "2.2" << endl;
                        outer_edges++;
                        if(group_size[itr->node1]==1 ^ group_size[itr->node2]==1){const_half_singletons++;} //^ = XOR
                    }
                }
            }

            //take out later?
            for(int i=0; i<K_neighborhood.size(); i++){
                if(group_size[i]==2){double_groups++;}
                if(group_size[i]!=1){continue;}
                for(set<int>::iterator itr=K_neighborhood[i].begin(); itr!=K_neighborhood[i].end(); itr++){
                    if(*itr < i && group_size[*itr]==1){potential_fulls++;}
                }
            }
            double_groups /= 2;

            //cout << "E" << endl;
        }


        /*
        adj_dif << "________________________________________" << endl;
        sample_adj_dif(Adjacency_graph, K_neighborhood, same_cliques, adj_dif);
        //freq_out << "total_edges constant_edges inner_edges double_groups avg_complete_group outer_edges
        //potential_fulls full_singletons constr_half_singletons half_singletons" <<endl;
        freq_out << edge_frequencies.size() << " " << frequencies.back() << " " << all_inner_edges << " " << const_inner_edges << " " << double_groups << " "
        << (double)complete_group_size / const_inner_edges << " " << outer_edges << " " << potential_fulls << " " << full_singletons <<
        " " << const_half_singletons << " " << half_singletons << " " << endl;
        freq_out << "repeats: " << repeats << endl;
        freq_out << "k-value: " << k << endl;
        freq_out << "# of edges: " << edge_frequencies.size() << endl;
        freq_out << "# of constant edges: " << frequencies.back() << endl;
        freq_out << "--------------------" << endl;
        if(!profs){
            freq_out << "const inner edges: " << const_inner_edges << endl;
            freq_out << "double groups:  " << double_groups<< endl;
            freq_out << "average complete group size: " << (double)complete_group_size / const_inner_edges << endl;
            freq_out << "--------------------" << endl;
            freq_out << "outer edges: " << outer_edges << endl;
            freq_out << "potential full singletons " << potential_fulls << endl;
            freq_out << "full-singleton edges: " << full_singletons << endl;
            freq_out << "constrained half-singleton edges: " << const_half_singletons << endl;
            freq_out << "half-singleton edges: " << half_singletons << endl;
        }
        for(int i=1; i<frequencies.size(); i++){
            freq_out << i << " " <<frequencies[i] << "\n";
        }*/


    }

    //outputs last final_graph
    ofstream out_str("output.txt");
    for(int i=0; i<Final_graph.size(); i++){
        for(list<int>::iterator listitr = Final_graph[i].begin(); listitr != Final_graph[i].end(); listitr++){
            out_str << i << " " << *listitr << endl;
        }
    }

}





/*
    void remove_singletons(vector<list<int> >& Final_graph, vector<int>& group_size){
            //edits results of my algorithm based on singleton edges, etc.
            list<int> temp;
            //vector<list<int> > New_final(Final_graph.size(),temp);
            for(int i=0; i<Final_graph.size(); i++){
                list<int>::iterator itr = Final_graph[i].begin();
                while(itr != Final_graph[i].end()){
                    if(i > *itr){itr++; continue;}
                    if((group_size[i]==1 || group_size[*itr]==1) and rand()%2==0){
                        int itr_val = *itr;
                        itr = Final_graph[i].erase(itr);
                        Final_graph[itr_val].remove(i);
                    }
                    else{itr++;}
                    else if(((group_size[i]==1 && group_size[*itr]==1) || (clique_lookup[i]==clique_lookup[*itr] && group_size[i]==2))
                             && rand()%1==0){
                        int itr_val = *itr; itr++;
                        int new_node = Find_swap_edge(i, itr_val, Final_graph);
                        int nn2 = Find_swap_edge(itr_val, i, Final_graph);
                        if(new_node != -1){
                           exchange_edge(Final_graph, i, itr_val, new_node);
                        }
                        else if(nn2 != -1){exchange_edge(Final_graph, itr_val, i, nn2);}
                    }
                    else{itr++;}
               }
            }
            Final_graph = New_final;
            for(int i=0; i<Final_graph.size(); i++){Final_graph[i].sort();}
        }*/
