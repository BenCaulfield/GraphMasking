#include <vector>
#include <list>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "enumerate_cliques.cpp"

using namespace std;

typedef std::list<std::vector<int> >::iterator adj_itr;


void print_cliques(vector<list<int> >& cliques){
    ofstream out("cliques.txt");
    out << "Number of cliques: " << cliques.size() << endl;
    for(int i=0; i<cliques.size(); i++){
        for(list<int>::iterator itr2 = cliques[i].begin(); itr2 != cliques[i].end(); itr2++){
            out << *itr2 << " ";
        }
        out << endl;
    }
}

bool operator<(vector<int>& v1, vector<int>& v2){
    for(int i=0; i<v1.size() && i<v2.size(); i++){
        if(v1[i]<v2[i]){return true;}
        if(v1[i]>v2[i]){return false;}
    }
    return v1.size() < v2.size();
}

bool operator==(vector<int>& v1, vector<int>&v2){
    if(v1.size() != v2.size()){return false;}
    for(int i=0; i<v1.size(); i++){
        if(v1[i]!=v2[i]){return false;}
    }
    return true;
}

void randomize_vector(vector<int>& v){
    for(int i=v.size()-1; i>0; i--){
        int pos = rand() % (i+1); //WAS JUST i BEFORE, WHY?
        //cout << "um" <<  rand() << endl;
        int val = v[pos];
        v[pos] = v[i];
        v[i] = val;
    }
}

void my_algorithm(vector<list<int> >& Original_Graph, vector<list<int> >& Final_Graph, list<vector<int> >& adj_groups){
 long int current_time = clock();

 long int t1 = clock() - current_time;
 current_time = clock();


 //assembles name_switches. name_switches[i] = j means that node i will be changed to node j in the new graph
 vector<int> name_switches(Original_Graph.size(), 0);
 for(adj_itr i=adj_groups.begin(); i!=adj_groups.end(); i++){
     vector<int> randomized = *i;
     randomize_vector(randomized);
     for(int j=0; j<randomized.size(); j++){
         name_switches[(*i)[j]] = randomized[j];
     }
 }

 long int t4 = clock() - current_time;
 current_time = clock();

 //assembles final graph
 list<int> temp_list;
 Final_Graph.insert(Final_Graph.end(), Original_Graph.size(), temp_list);
 for(int i=0; i<Original_Graph.size(); i++){
     for(list<int>::iterator itr = Original_Graph[i].begin(); itr != Original_Graph[i].end(); itr++){
         if(i == *itr){continue;} // WHAT IS GOING ON?
         Final_Graph[name_switches[i]].push_back(name_switches[*itr]);
        // if(name_switches[i] == name_switches[*itr]){cout << "AAA" << i << " " << *itr << " " << name_switches[i] << endl;};
     }
 }

 long int t5 = clock() - current_time;
 //cout << t1 << " " << t4 << " " << t5 << endl;
}


void erase_full_singletons(vector<list<int> >& Final_graph, vector<int>& group_size){
            //SUPER EXPERIMENT !!!!! erases full-singletons
            list<pair<int,int> > erasing_edges;
            for(int i=0; i<Final_graph.size(); i++){
                //continue;
               // if(group_size[i]!=1){continue;}
                list<int>::iterator itr=Final_graph[i].begin();
                while(group_size[i]!=1 && itr != Final_graph[i].end()){
                    if(group_size[*itr]==1 && rand()%20==0){
                        pair<int,int> temp(*itr, i);
                        pair<int,int> back_temp(i, *itr);
                        itr = Final_graph[i].erase(itr);
                        erasing_edges.push_back(temp);
                        //erasing_edges.push_back(back_temp);
                        continue;
                    }
                    itr++;
                }
            }
            erasing_edges.sort();
            cout << "erasing edges: " << erasing_edges.size() << endl;
            //for(list<pair<int,int> >::iterator itr=erasing_edges.begin(); itr!=erasing_edges.end(); itr++){cout << itr->first << " " <<itr->second << endl;}
            list<pair<int,int> >::iterator erase_itr = erasing_edges.begin();
            for(int i=0; i<Final_graph.size(); i++){
                list<int>::iterator itr=Final_graph[i].begin();
                while(itr!=Final_graph[i].end() && erase_itr != erasing_edges.end()){
                    pair<int,int> temp2(i, *itr);
                    if(erase_itr->first==i && erase_itr->second==*itr){erase_itr = erasing_edges.erase(erase_itr);}
                    else if(*erase_itr < temp2){erase_itr++;}
                    else {itr++;}
                }
                if(erase_itr == erasing_edges.end()){break;}
            }
}

/*
//NOTE THAT CURRENT CLIQUE IS (NOT) PASSED BY REFERENCE!!!!!
void cliques_helper(const vector<list<int> >& K_neighborhood, list<int>& current_clique, list<int>::iterator current_position, list<list<int> >& cliques){
    for(list<int>::iterator itr = current_clique.begin(); itr != current_clique.end(); itr++){cout << *itr << " ";} cout << ": " << *current_position << endl;
    if(current_position == current_clique.end()){cliques.push_back(current_clique); return;}
    //if(current_clique.size() < k+1){cout << "it has happened." << endl; return;}
    list<int>::const_iterator adj_itr = (K_neighborhood[*current_position]).begin();
    list<int>::iterator clique_itr = current_clique.begin();

    //Checks if adjacency list is exactly the same as current_clique for all nodes before current_position
    while(*adj_itr < *current_position){
        assert(*adj_itr <= *clique_itr); //ADD >= possibility before deleting this!!!!!
        if(*adj_itr < *clique_itr){cout << "X" << endl; return;}
        else {adj_itr++; clique_itr++;}
    }
    assert(*adj_itr >= *current_position); assert(clique_itr == current_position);
    //removes elements from current_clique that are not adjacent to the node pointed to by current_position
    clique_itr++;
    while(adj_itr != K_neighborhood[*current_position].end() && clique_itr != current_clique.end()){
        if(*adj_itr < *clique_itr){adj_itr++;}
        else if(*adj_itr == *clique_itr){adj_itr++; clique_itr++;}
        else {clique_itr = current_clique.erase(clique_itr);} //the clique_itr is not adjacent to the node at current_position
    }
    while(clique_itr != current_clique.end()){clique_itr = current_clique.erase(clique_itr);}
    // recursively calls cliques_helper on the remainder of this clique
    clique_itr = current_position; clique_itr++;
    while(clique_itr != current_clique.end()){
        list<int> next_clique = current_clique;
        list<int>::iterator next_itr = next_clique.begin();
        while(*next_itr != *clique_itr){next_itr++;}
        cliques_helper(K_neighborhood, next_clique, next_itr, cliques);
        clique_itr = current_clique.erase(clique_itr);
    }
   //calls once more for iterator pointing to end of clique
    list<int> next_clique = current_clique;
    list<int>::iterator next_itr = next_clique.end();
    cliques_helper(K_neighborhood, next_clique, next_itr, cliques);
    return;
}




list<list<int> > get_Cliques(vector<list<int> >& K_neighborhood, int k){
    list<list<int> > cliques;
    for(int i=1; i+k < K_neighborhood.size(); i++){
        list<int> l = K_neighborhood[i];
        //inserts value i into l
         list<int>::iterator itr = l.begin();
         while(itr != l.end() && *itr < i){itr++; l.pop_front();}
         l.insert(itr, i);
        cliques_helper(K_neighborhood, l, l.begin(), cliques); cout << "Y" << endl;
    }
    cout << cliques.size() << endl;
    print_cliques(cliques);
    cout << "ho" << endl;
    return cliques;
}



//bad one
//list<list<int>* > cliques_helper(const vector<list<int> >& K_neighborhood, list<int>* current_clique, list<int>::iterator current_position){
    list<list<int>* > cliques;

    //removes nodes from current clique that are not in adj_list at current_position
    list<int>::iterator clique_itr = current_position;
    list<int>& current_adj = K_neighborhood[*clique_itr];
    list<int>::iterator adj_itr = current_adj.begin();
    while(adj_itr != current_adj.end() && clique_itr != current_clique -> end()){
        if(*clique_itr == *adj_itr){clique_itr++; adj_itr++;}
        else if(*clique_itr < *adj_itr){clique_itr = current_clique -> remove(clique_itr);}
        else {adj_itr++;}
    }
    while(clique_itr != current_clique.end()){

    }
}*/


/*
//NOTE THAT CURRENT CLIQUE IS (NOT) PASSED BY REFERENCE!!!!!
void cliques_helper(const vector<list<int> >& K_neighborhood, list<int>& current_clique, list<int>::iterator current_position, vector<list<int> >& cliques){
    for(list<int>::iterator itr = current_clique.begin(); itr != current_clique.end(); itr++){cout << *itr << " ";} cout << ": " << *current_position << endl;
    if(current_position == current_clique.end()){cliques.push_back(current_clique);cout << "RETURN" << endl;return;}

    list<int>::const_iterator adj_itr = (K_neighborhood[*current_position]).begin();

    //Moves adj_itr and clique_itr to proper positions
    while(*adj_itr < *current_position && adj_itr != K_neighborhood[*current_position].end()){ adj_itr++;}
    list<int>::iterator clique_itr = current_position;

    //assert(*adj_itr >= *current_position);

    //removes elements from current_clique that are not adjacent to the node pointed to by current_position
    clique_itr++;
    bool violates_edge = false; //true if there is an node in current_current clique not in the k_neighborhood of the node at current_position
    list<int> new_clique;
    while(adj_itr != K_neighborhood[*current_position].end() && clique_itr != current_clique.end()){
        if(*adj_itr < *clique_itr){adj_itr++;}
        else if(*adj_itr == *clique_itr){adj_itr++; clique_itr++;}
        else {//the clique_itr is not adjacent to the node at current_position
            if(!violates_edge){
                violates_edge = true;
                new_clique = current_clique;
            }
            clique_itr = current_clique.erase(clique_itr);
            assert(new_clique != current_clique);
        }
    }
    while(clique_itr != current_clique.end()){
        if(!violates_edge){
            violates_edge = true;
            new_clique = current_clique;
        }
        clique_itr = current_clique.erase(clique_itr);
    }

   // for(list<int>::iterator itr = current_clique.begin(); itr != current_clique.end(); itr++){cout << *itr << " ";} cout << ": " << *current_position << endl;


    //calls itself on clique with node at current_position removed
    if(violates_edge){
        cout << "JUMP" << endl;
        list<int>::iterator new_itr = new_clique.begin();
        while(*new_itr != *current_position){new_itr++; assert(new_itr != new_clique.end());}
        new_itr = new_clique.erase(new_itr);
        cliques_helper(K_neighborhood, new_clique, new_itr, cliques);
    }

    //calls itself on clique with all violators of current_position removed
    current_position++;
    cliques_helper(K_neighborhood, current_clique, current_position, cliques);
}




void get_Cliques(vector<list<int> >& K_neighborhood, int k, vector<list<int> >& cliques){
    list<int> l;
    for(int i=1; i < K_neighborhood.size(); i++){l.push_back(i);}
    cliques_helper(K_neighborhood, l, l.begin(), cliques);
    cout << cliques.size() << endl;
    cout << "ho" << endl;
}*/
