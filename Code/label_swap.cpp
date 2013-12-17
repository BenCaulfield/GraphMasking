#include <vector>
#include <list>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

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
        int pos = rand() % (i+1);
        int val = v[pos];
        v[pos] = v[i];
        v[i] = val;
    }
}

void Label_Swap(vector<list<int> >& Original_Graph, vector<list<int> >& Final_Graph, list<vector<int> >& adj_groups){

     //assembles name_switches. name_switches[i] = j means that node i will be changed to node j in the new graph
     vector<int> name_switches(Original_Graph.size(), 0);
     for(adj_itr i=adj_groups.begin(); i!=adj_groups.end(); i++){
         vector<int> randomized = *i;
         randomize_vector(randomized);
         for(int j=0; j<randomized.size(); j++){
             name_switches[(*i)[j]] = randomized[j];
         }
     }

     //assembles final graph
     list<int> temp_list;
     Final_Graph.insert(Final_Graph.end(), Original_Graph.size(), temp_list);
     for(int i=0; i<Original_Graph.size(); i++){
         for(list<int>::iterator itr = Original_Graph[i].begin(); itr != Original_Graph[i].end(); itr++){
             if(i == *itr){continue;}
             Final_Graph[name_switches[i]].push_back(name_switches[*itr]);
         }
     }
}
