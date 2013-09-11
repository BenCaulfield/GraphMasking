#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <utility>
using namespace std;

typedef std::list<std::vector<int> >::iterator adj_itr; 

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

void sample_adj_dif(vector<list<int> >& graph, vector<set<int> >& K_neighborhood, vector<vector<int> >& adj_groups, ofstream& adj_dif){
    vector<int> adj_map(graph.size(), -1); //adj_map[i] = j means that the adj_group for i is in adj_groups[j]
    for(int i=0; i<adj_groups.size(); i++){
        for(int j=0; j<adj_groups[i].size(); j++){
            adj_map[adj_groups[i][j]] = i;
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

//merges two adjacency groups together
void Merge_two_groups(list<vector<int> >& adj_groups, vector<adj_itr>& adj_map, vector<bool>& altered_node, adj_itr n1_itr, adj_itr n2_itr){
    for(int j=0; j<n2_itr->size(); j++){
        int adj_node = (*n2_itr)[j];
        n1_itr -> push_back(adj_node);
        adj_map[adj_node] = n1_itr;
        altered_node[adj_node] = true;
    }
    sort(n1_itr -> begin(), n1_itr -> end());
    n2_itr->clear();
    //moves back vector to n2_itr and pops back;
    for(int j=0; j<adj_groups.back().size(); j++){
        int adj_node = adj_groups.back()[j];
        n2_itr->push_back(adj_node);
        adj_map[adj_node] = n2_itr;
    }
    sort(n2_itr->begin(), n2_itr->end());
    adj_groups.pop_back();
}

void merge_similar_adj_groups(int max_diff, int total_diff, vector<list<int> >& graph, vector<set<int> >& K_neighborhood, list<vector<int> >& adj_groups, vector<adj_itr> adj_map){
    vector<bool> altered_node(graph.size(), false);
    //cout <<"ksize: " << K_neighborhood.size() << endl; 
  
    /*vector<int> adj_map(graph.size(), -1); //adj_map[i] = j means that the adj_group for i is in adj_groups[j]
    for(int i=0; i<adj_groups.size(); i++){
        for(int j=0; j<adj_groups[i].size(); j++){
            //cout << adj_groups[i][j] << " ";
            adj_map[adj_groups[i][j]] = i;
        }
    }*/

    ofstream swaps("Swaps");

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
        //if(min_pos2 != -1 && rand() % 2 == 1){min_pos = min_pos2; min_diff = min_diff2;} //randomly uses second-most similar node instead of first;
        if(min_pos != -1){
            swaps << i << " " << min_pos << endl;
            total_changes++;
            altered_node[i] = true;
            altered_node[min_pos] = true;
           //cout << i << " " << min_pos << " " << min_diff << endl;
            current_diff += min_diff;

            //merges two adj_groups
            Merge_two_groups(adj_groups, adj_map, altered_node, adj_map[i], adj_map[min_pos]);

        }   
    }
}

/*
void wesley_merge(vector<list<int> >& graph, vector<set<int> >& K_neighborhoods, vector<vector<int> >& adj_groups)
{
    std::map<int,std::pair<int,int> > swap;
    for(unsigned int i = 0; i < 10; i++)
    {
        vector<bool> altered_node(graph.size(), false);
        vector<int> adj_map(graph.size(), -1); //adj_map[i] = j means that the adj_group for i is in adj_groups[j]
        for(int i=0; i<adj_groups.size(); i++){
            for(int j=0; j<adj_groups[i].size(); j++){
                //cout << adj_groups[i][j] << " ";
                adj_map[adj_groups[i][j]] = i;
            }
        }
            
        swap.clear();
        for(unsigned int j = 0; j < K_neighborhoods.size(); j++)
        {
            swap.insert(std::pair<int,std::pair<int,int> >(find_group_difference(j, k_neighborhoods[j].begin()*, k_neighborhoods[j], k_neighborhoods[k_neighborhoods[j].begin()*]),std::pair<int,int>(j,k_neighborhoods[j].begin()*));        
        }
        Merge_two_groups(adj_groups,adj_map,altered_node, i, adj_map[i]);
    }
}*/
