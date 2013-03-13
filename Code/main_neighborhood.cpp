#include "professors_algorithm.cpp"
#include "my_algorithm.cpp"
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

//Finds amount of overlap between two sets using formula: |A intersect B| / |A union B|
double Overlap_Factor(set<int>& adj1, set<int>& adj2, int& problem){
    if(adj1.size() == adj2.size() && adj1.size() == 0){return 1;}
    set<int>::iterator itr1 = adj1.begin();
    set<int>::iterator itr2 = adj2.begin();
    int intersection_size = 0;
    while(itr1 != adj1.end() && itr2 != adj2.end()){
        if(*itr1 < *itr2){itr1++; }
        else if(*itr1 > *itr2){problem=*itr2; itr2++; cout << "!"; }
        else {intersection_size++; itr1++; itr2++;}
    }
    double return_val = (double)intersection_size / (double)((adj1.size() + adj2.size()) - intersection_size);
    return return_val;
}

//For each node n in K1 and K2, outputs the overlap between the set of nodes adjacent to n in each graph.
void output_overlap(ofstream& out, vector<set<int> >& K1, vector<set<int> >& K2, vector<list<int> > graph, int k){
  double average;
  for(int i=1; i<K1.size(); i++){
      int problem = -1;
      //cout << i << " ";
      double overlap = Overlap_Factor(K1[i], K2[i], problem);
      if(problem != -1){cout << i << " " << problem << endl; node_distance(i, problem, graph, k);}
      out << i << " " << overlap << endl;
      average += overlap;
    }
    average /= (K1.size() - 1);
    out << "average_overlap "<<  average << endl;
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

    /*int k = atoi(argv[1]);
    string network_inputs;
    network_inputs += "./benchmark ";
    for(int i=2; i < argc; i++){
        network_inputs += argv[i];
        network_inputs += " ";
    }
    //system("cd ./binary_networks");
    system("network_inputs");
    ifstream in_str("network.dat", ifstream::in);
    */
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

        list<int> l; set<int> a_set;
        while(Adjacency_graph.size() <= source){Adjacency_graph.push_back(l);}

        max_node = max(max_node, max(source, target));
        Adjacency_graph[source].push_back(target);
    }

    vector<set<int> > K_neighborhood;
    Create_K_Graph(Adjacency_graph, k, K_neighborhood);
    list<frequency_edge> edge_frequencies;
    vector<list<int> > K_neighborhood_lists;
    vector<int> clique_lookup;
    vector<int> group_size;
    vector<int> temp_vec;
    vector<vector<int> >same_cliques; same_cliques.push_back(temp_vec);

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
        get_cliques(K_neighborhood_lists, k, same_cliques, clique_lookup, group_size);
    }

    for(int i=0; i<repeats; i++){
        if(profs){
            Final_graph.insert(Final_graph.begin(), max_node+1, l);

            Prof_Alg(K_neighborhood, Final_graph, k);
        }
        else{
            my_algorithm(Adjacency_graph,  Final_graph, same_cliques);

            /*
            //edits results of my algorithm based on singleton edges, etc.
            list<int> temp;
            //vector<list<int> > New_final(Final_graph.size(),temp);
            for(int i=0; i<Final_graph.size(); i++){
                list<int>::iterator itr = Final_graph[i].begin();
                while(itr != Final_graph[i].end()){
                    if(i > *itr){itr++; continue;}
                    if((group_size[i]==1 || group_size[*itr]==1) and rand()%2==0 and false){
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
            for(int i=0; i<Final_graph.size(); i++){Final_graph[i].sort();}*/
        }

        //updates list of edge-frequencies based on Final_graph
        list<frequency_edge>::iterator freq_itr = edge_frequencies.begin();
        for(int j=1; j<Final_graph.size(); j++){
            Final_graph[j].sort(); // MAY BE INEFFICIENT!!!!!
            list<int>::iterator itr = Final_graph[j].begin();
            while(*itr < j && itr != Final_graph[j].end()){itr++;}
            while(itr != Final_graph[j].end()){
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
        Create_K_Graph(Final_graph, k, New_K_neighborhood);
        output_overlap(overlap_out, K_neighborhood, New_K_neighborhood, Final_graph, k);//note, the two may not be the same size
    }

    //outputs frequencies from edge_frequencies
    //calculates relevant statistics
    if(output_type == "both" || output_type == "edge_frequency"){
        ofstream freq_out("edge_frequencies.txt");
        vector<int> frequencies;
        while(frequencies.size() <= repeats){frequencies.push_back(0);}
        for(list<frequency_edge>::iterator itr=edge_frequencies.begin(); itr!=edge_frequencies.end(); itr++){
            //while(frequencies.size() <= itr->frequency){frequencies.push_back(0);}
            frequencies[itr->frequency]++;
        }

        //Types of "const-edges"
        //ofstream const_out("constant_edges.txt");
        int inner_edges = 0; //num edges between two nodes in the same group of cliques
        int complete_group_size = 0;
        int outer_edges = 0; //num edges between two nodes differing by at least one clique
        int full_singletons = 0;
        int constr_half_singletons = 0;
        int half_singletons = 0;
        int potential_fulls = 0;
        int double_groups = 0;

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
                if(itr -> frequency == repeats){
                    if(clique_lookup[itr->node1]==clique_lookup[itr->node2]){/*cout << "2.1" << endl;*/ inner_edges++; complete_group_size += group_size[itr->node1];}
                    else{ //cout << "2.2" << endl;
                        outer_edges++;
                        if(group_size[itr->node1]==1 ^ group_size[itr->node2]==1){constr_half_singletons++;} //^ = XOR
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

        freq_out << edge_frequencies.size() << " " << frequencies.back() << endl;
        freq_out << "repeats: " << repeats << endl;
        freq_out << "k-value: " << k << endl;
        freq_out << "# of edges: " << edge_frequencies.size() << endl;
        freq_out << "# of constant edges: " << frequencies.back() << endl;
        freq_out << "--------------------" << endl;
        if(!profs){
            freq_out << "inner edges: " << inner_edges << endl;
            freq_out << "double groups:  " << double_groups<< endl;
            freq_out << "average complete group size: " << (double)complete_group_size / inner_edges << endl;
            freq_out << "--------------------" << endl;
            freq_out << "outer edges: " << outer_edges << endl;
            freq_out << "potential full singletons " << potential_fulls << endl;
            freq_out << "full-singleton edges: " << full_singletons << endl;
            freq_out << "constrained half-singleton edges: " << constr_half_singletons << endl;
            freq_out << "half-singleton edges: " << half_singletons << endl;
        }
        for(int i=1; i<frequencies.size(); i++){
            freq_out << i << " " <<frequencies[i] << "\n";
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

