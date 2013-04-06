#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

//finds total differences between k_neighborhoods of two nodes, ignoring the nodes, themselves.
int find_group_difference(int n1, int n2, set<int>& n1_nei, set<int>& n2_nei){
    set<int>::iterator itr1 = n1_nei.begin();
    set<int>::iterator itr2 = n2_nei.begin();
    int diff = 0;
    //cout << n1 << "//" << n2 << endl;
    while(itr1 != n1_nei.end() && itr2 != n2_nei.end()){
        //cout << *itr1 << ":" << *itr2 << " ";
        if(*itr1 == n2){itr1++; continue;}
        if(*itr2 == n1){itr2++; continue;}
        if(*itr1 < *itr2){itr1++; diff++;}
        else if(*itr1 > *itr2){itr2++; diff++;}
        else{itr1++; itr2++;}
    }
    //cout << "D" << diff << endl;
    while(itr1 != n1_nei.end()){
        //cout << *itr1 << ":" << *itr2 << " ";
         if(*itr1 != n2){diff++;}  itr1++;}
    while(itr2 != n2_nei.end()){
        //cout << *itr1 << ":" << *itr2 << " ";
         if(*itr2 != n1){diff++;}  itr2++;}
    return diff;
    
}

void sample_adj_dif(vector<list<int> >& graph, vector<set<int> >& K_neighborhood, vector<vector<int> >& same_cliques, ofstream& adj_dif){
    vector<int> adj_map(graph.size(), -1); //adj_map[i] = j means that the adj_group for i is in same_cliques[j]
    for(int i=0; i<same_cliques.size(); i++){
        for(int j=0; j<same_cliques[i].size(); j++){
            adj_map[same_cliques[i][j]] = i;
        }
    }
    int ind = 0;
    int dif_sum = 0;
    while(ind < 100){
        int n1 = rand() % graph.size();
        int n2 = graph[n1].front();
        int diff = find_group_difference(n1, n2, K_neighborhood[n1], K_neighborhood[n2]);
        dif_sum += diff;
        adj_dif << n1 << " " << n2 << " " << diff << endl;
        ind++;
    }
    adj_dif << "average: " << (double)dif_sum/100 << endl;
}

void merge_similar_adj_groups(int max_diff, int total_diff, vector<list<int> >& graph, vector<set<int> >& K_neighborhood, vector<vector<int> >& same_cliques){
    vector<bool> altered_node(graph.size(), false);
    //cout <<"ksize: " << K_neighborhood.size() << endl; 

  /*  for(int i=0; i<same_cliques.size(); i++){
        cout << i << ": ";
        for(int j=0; j<same_cliques[i].size(); j++){
            cout << same_cliques[i][j] << " ";
        }
        cout << endl;
    }*/
    
    vector<int> adj_map(graph.size(), -1); //adj_map[i] = j means that the adj_group for i is in same_cliques[j]
    for(int i=0; i<same_cliques.size(); i++){
        for(int j=0; j<same_cliques[i].size(); j++){
            //cout << same_cliques[i][j] << " ";
            adj_map[same_cliques[i][j]] = i;
        }
       // cout << endl;
    }

    //this is a  merging of similar groups. finds n1 & n2
    int current_diff=0;
    int total_changes=0;
    for(int i=1; i<graph.size(); i++){
        if(altered_node[i]){continue;}
        if(current_diff >= total_diff){break;}
        int min_diff = max_diff + 1; int min_pos = -1; int min_diff2 = max_diff + 1; int min_pos2 = -1;
        for(list<int>::iterator itr=graph[i].begin(); itr!=graph[i].end(); itr++){
            if(*itr < i || altered_node[*itr]){continue;}
            if(current_diff >= total_diff){break;}
            int diff = find_group_difference(i, *itr, K_neighborhood[i], K_neighborhood[*itr]);
            //LAST PART OF BOOL TEST SHOULD NOT BE NECESSARY FiX IT!!!!!!!!!!!!!!!
            if(diff < min_diff && diff != 0 && adj_map[i] != adj_map[*itr]){
                //cout << "new_min: " << diff << " " << adj_map[i] << " " << adj_map[*itr] << endl;
                 min_diff = diff; min_pos = *itr;}
           /* if(diff < min_diff2 && diff != 0){
                if(diff < min_diff){min_diff = diff; min_pos = *itr;}
                else{min_diff2 = diff; min_pos2 = *itr;}
            }      */
        }
       // cout << min_diff << "::" << min_diff2 << " " << endl;
        if(min_pos2 != -1 && rand() % 2 == 1){min_pos = min_pos2; min_diff = min_diff2;} //randomly uses second-most similar node instead of first;
        if(min_pos != -1){
            total_changes++;
            altered_node[i] = true;
            altered_node[min_pos] = true;
            cout << i << " " << min_pos << " " << min_diff << endl;
            current_diff += min_diff;
            //cout << "G" << endl;
            //merges two adj_groups
            int former_adj_pos = adj_map[min_pos];
            //c//out << "size: " << same_cliques[former_adj_pos].size() << endl;
            //cout << adj_map[i] << " " << former_adj_pos << endl;
            for(int j=0; j<same_cliques[former_adj_pos].size(); j++){
                int adj_node = same_cliques[former_adj_pos][j];
                same_cliques[adj_map[i]].push_back(adj_node);
                adj_map[adj_node] = adj_map[i];
                altered_node[adj_node] = true;
            }
            sort(same_cliques[adj_map[i]].begin(), same_cliques[adj_map[i]].end());
            same_cliques[former_adj_pos].clear();
            //cout << "H" << endl;
            //moves back vector to former_adj_pos and pops back;
            for(int j=0; j<same_cliques.back().size(); j++){
                int adj_node = same_cliques.back()[j];
                same_cliques[former_adj_pos].push_back(adj_node);
                adj_map[adj_node] = former_adj_pos;
            }
            sort(same_cliques[former_adj_pos].begin(), same_cliques[former_adj_pos].end());
            same_cliques.pop_back();
            return;
        }   
    }
   // cout << current_diff << endl;   
   // cout << "changes: " << total_changes << endl;         

   /* cout << "________________________________________" << endl;
    for(int i=0; i<same_cliques.size(); i++){
        cout << i << ": ";
        for(int j=0; j<same_cliques[i].size(); j++){
            cout << same_cliques[i][j] << " ";
        }
        cout << endl;
    }*/

}