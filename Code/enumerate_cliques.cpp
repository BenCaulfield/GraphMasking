#include <vector>
#include <list>

using namespace std;

struct clique_edge{
    clique_edge(int n1, int n2): node1(n1), node2(n2), used(false){}
    int node1;
    int node2;
    bool used;
    //list<int>::iterator iter1;
   // list<int>::iterator iter2;
};

bool cliques_less(const pair<int, vector<int> >&v1, const pair<int, vector<int> >& v2){
    return v1.second < v2.second;
}

void get_cliques(vector<list<int> >& K_neighborhood, int k, vector<vector<int> >& same_cliques){
    long int current_time = clock();
    vector<list<int> > cliques;

   //assembles helper_K and clique_edges
    list<int> temp_list;
    vector<list<int> > helper_K(K_neighborhood.size(), temp_list); //each int in this list points to the position of the corresponding clique_edge in clique_edges
    vector<clique_edge> clique_edges;
    for(int i=0; i<K_neighborhood.size(); i++){
        for(list<int>::iterator itr = K_neighborhood[i].begin(); itr!=K_neighborhood[i].end(); itr++){
            if(i < *itr){ //ensures that edges are only added once to clique_edges
                helper_K[i].push_back(clique_edges.size());
                helper_K[*itr].push_back(clique_edges.size());
                clique_edge c(i, *itr);
                clique_edges.push_back(c);
            }
        }
    }

    long int t1 = clock() - current_time;
    long int t2 = 0; long int t3 = 0; long int t4 = 0;

    //randomly picks an edge, if the edge is unused, finds a clique it is in.
    for(int i=0; i<clique_edges.size(); i++){
        if(clique_edges[i].used){continue;}
        //cout << clique_edges[i].node1 << " : " << clique_edges[i].node2 << endl;

        current_time = clock();
        //Assembles initial clique
        list<int> current_clique;
        clique_edge& c = clique_edges[i];
        list<int>::iterator itr1=K_neighborhood[c.node1].begin();
        list<int>::iterator itr2=K_neighborhood[c.node2].begin();
        while(itr1!=K_neighborhood[c.node1].end() && itr2!=K_neighborhood[c.node2].end()){
            if(*itr2==c.node1){current_clique.push_back(*itr2); itr2++; continue;}
            if(*itr1==c.node2){current_clique.push_back(*itr1); itr1++; continue;}


            if(*itr1==*itr2){current_clique.push_back(*itr1); itr1++; itr2++;}
            else if(*itr1<*itr2){itr1++;}
            else{itr2++;}
        }
        if(current_clique.back() < c.node1){current_clique.push_back(c.node1);}
        if(current_clique.back() < c.node2){current_clique.push_back(c.node2);}
        t2 += clock() - current_time;
        current_time = clock();

        //removes nodes from clique that do not share an edge with other nodes in the clique
        itr1=current_clique.begin();
        while(true){
            if(*itr1!=c.node1 && *itr1!=c.node2){
                list<int>::iterator itr3 = itr1; itr3++;
                itr2 = K_neighborhood[*itr1].begin();
                while(*itr2 < *itr3 && itr2 != K_neighborhood[*itr1].end()){itr2++;}
                while(itr3!=current_clique.end() && itr2!=K_neighborhood[*itr1].end()){
                    if(*itr2 < *itr3){itr2++;}
                    else if(*itr3 < *itr2){itr3 = current_clique.erase(itr3);}
                    else {itr2++; itr3++;}
                }
                while(itr3!=current_clique.end()){
                    itr3=current_clique.erase(itr3);
                }
            }
            if(*itr1 != current_clique.back()){itr1++;}
            else{break;}
        }
        t3 += clock() - current_time;
        current_time = clock();

        //marks edges that are in this clique as "used" in clique_edges
        itr1=current_clique.begin();
        while(itr1!=current_clique.end()){
            itr2 = helper_K[*itr1].begin();
            list<int>::iterator itr3=current_clique.begin();
            while(itr3!=current_clique.end() && itr2!=helper_K[*itr1].end()){
                if(clique_edges[*itr2].node1 < *itr3){itr2++;}
                else if(clique_edges[*itr2].node1 > *itr3){itr3++;}
                else {clique_edges[*itr2].used = true; itr2++; itr3++;}
            }
            itr1++;
        }
        t4 += clock() - current_time;
        cliques.push_back(current_clique);
    }

     //Assembles and sorts clique_map
     ofstream out("cliques"); int border_nodes = 0;
     pair<int, vector<int> > temp_pair;
     vector<pair<int, vector<int> > > clique_map; //clique_map[i][j] is the jth clique that node i is in
     for(int i=0; i< K_neighborhood.size(); i++){temp_pair.first = i; clique_map.push_back(temp_pair);}
     for(int i=0; i < cliques.size(); i++){
         out << i << ": "; if(cliques[i].size()==1){border_nodes++;}
        for(list<int>::iterator itr=cliques[i].begin(); itr!=cliques[i].end(); itr++){
            out << *itr << " ";
            clique_map[*itr].second.push_back(i);
        }
        out << "\n";
     }
     out << "border nodes: " << border_nodes << endl;
     sort(clique_map.begin(), clique_map.end(), cliques_less);

     //finds nodes that are in all the same cliques.
     //same_cliques[i][j] is in all the same cliques as same_cliques[i][l] for any i,j,l in bounds
     vector<int> temp_vec;
     int index1 = 0; int index2 = 1;
     while(index2!=clique_map.size()){
        same_cliques.back().push_back(clique_map[index1].first);
        if(!(clique_map[index1].second==clique_map[index2].second)){same_cliques.push_back(temp_vec);}
        index1++; index2++;
     }
     same_cliques.back().push_back(clique_map[index1].first);

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
    same_out << "total cliques: " << cliques.size() << endl;

    cout << "get cliques: " << t1 << " " << t2 << " " << t3 << " " << t4 << endl;

}
