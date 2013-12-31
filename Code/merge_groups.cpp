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

typedef list<vector<int> >::iterator adj_itr;

//finds total differences between k_neighborhoods of two nodes, ignoring the nodes themselves.
int find_group_difference(int n1, int n2, set<int>& n1_nei, set<int>& n2_nei){
    bool some_overlap = false;
    set<int>::iterator itr1 = n1_nei.begin();
    set<int>::iterator itr2 = n2_nei.begin();
    int diff = 0;
    while(itr1 != n1_nei.end() && itr2 != n2_nei.end()){
        if(*itr1 == n2){some_overlap = true; itr1++; continue;}
        if(*itr2 == n1){some_overlap = true; itr2++; continue;}
        if(*itr1 < *itr2){itr1++; diff++;}
        else if(*itr1 > *itr2){itr2++; diff++;}
        else{some_overlap = true; itr1++; itr2++;}
    }
    while(itr1 != n1_nei.end()){
         if(*itr1 != n2){diff++;}
         else{some_overlap = true;}
           itr1++;
    }
    while(itr2 != n2_nei.end()){
         if(*itr2 != n1){diff++;}
         else{some_overlap = true;}
         itr2++;
    }
    if(!some_overlap){return -1;}
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
bool Merge_two_groups(list<vector<int> >& adj_groups, vector<adj_itr>& adj_map, vector<bool>& altered_node, adj_itr n1_itr, adj_itr n2_itr){
    if(&(*n1_itr) == &(*n2_itr)){return false;}
    //cout << "--------------" << endl;
    cout << &(*n1_itr) << " " << &(*n2_itr) << endl;
    //.cout << (adj_map[(*n1_itr)[0]] << " " << adj_map[(*n2_itr)[0]] << endl;
    cout << "n1: ";
    for(int i=0; i < n1_itr->size();i++){
        cout << (*n1_itr)[i] << " ";
    }
    cout << endl;
        cout << "size: " << adj_groups.size() << endl;
    cout << "n2: ";
    for(int j=0; j<n2_itr->size(); j++){
        cout << (*n2_itr)[j] << " ";
        int adj_node = (*n2_itr)[j];
        (*n1_itr).push_back(adj_node);
        adj_map[adj_node] = n1_itr;
        altered_node[adj_node] = true;
    }
    cout << endl;

    sort(n1_itr -> begin(), n1_itr -> end());
    (*n2_itr).clear();
    cout << "size: " << adj_groups.size() << endl;
    //moves back vector to n2_itr and pops back;
    for(int j=0; j<adj_groups.back().size(); j++){
        int adj_node = (adj_groups.back())[j];
        (*n2_itr).push_back(adj_node);
        adj_map[adj_node] = n2_itr;
    }
    cout << "size: " << adj_groups.size() << endl;
    sort(n2_itr->begin(), n2_itr->end());
    adj_groups.pop_back();
    cout << "merged: ";
    for(int i=0; i<n1_itr -> size(); i++){
        cout << (*n1_itr)[i] << " ";
    }
    cout << endl;
    cout << "size: " << adj_groups.size() << endl;
    /*for(list<vector<int> >::iterator i=adj_groups.begin(); i!=adj_groups.end(); i++){
        cout << (*i).size() << " ";
    }*/
    cout << endl;
    string s;
    cin >> s;
    return true;
}










//merges two adjacency groups together
bool Merge_two_groups2(vector<vector<int>* >& adj_map, vector<int>* p1, vector<int>* p2){
    if(p1 == p2){return false;}
    //cout << "--------------" << endl;
    //cout << "yo" << endl;
    //cout << &(*p1) << " " << &(*p2) << endl;
    //.cout << (adj_map[(*n1_itr)[0]] << " " << adj_map[(*n2_itr)[0]] << endl;
    //cout << "n1: ";
    /*for(int i=0; i < p1->size();i++){
        cout << (*p1)[i] << " ";
    }*/
    //cout << endl;
    //cout << "n2: ";
    for(int j=0; j<p2->size(); j++){
        //cout << (*p2)[j] << " ";
        int adj_node = (*p2)[j];
        (*p1).push_back(adj_node);
        adj_map[adj_node] = p1;
    }
    //cout << endl;

    sort(p1 -> begin(), p1 -> end());
    delete p2;

    /*cout << "merged: ";
    for(int i=0; i<p1 -> size(); i++){
        cout << (*p1)[i] << " ";
    }
    cout << endl;*/

    //string s;
    //cin >> s;
    return true;
}


//can be made more efficient
void deterministic_merge2(vector<list<int> >& graph, vector<set<int> >& K_neighborhoods, list<vector<int> >& adj_groups, vector<adj_itr>& adj_map, int mergecap){
    cout << "-Z" << endl;
    vector<pair<int, int> > v;
    vector<bool> altered_node(graph.size(), false);
    for(list<vector<int> >::iterator itr = adj_groups.begin(); itr != adj_groups.end(); itr++){
        if(itr->size() == 0){continue;}
        for(list<vector<int> >::iterator itr2 = itr; itr2 != adj_groups.end(); itr2++){
            if(itr == itr2 || (itr2 -> size() == 0)){continue;}
            int diff = find_group_difference((*itr)[0], (*itr2)[0], K_neighborhoods[(*itr)[0]], K_neighborhoods[(*itr2)[0]]);
            if(diff == -1 || diff == 0){continue;}
            /*for(set<int> ::iterator setitr = K_neighborhoods[(*itr)[0]].begin(); setitr != K_neighborhoods[(*itr)[0]].end(); setitr++){cout << *setitr << " ";}
            cout << endl;
            for(set<int> ::iterator setitr = K_neighborhoods[(*itr2)[0]].begin(); setitr != K_neighborhoods[(*itr2)[0]].end(); setitr++){cout << *setitr << " ";}
            cout << endl;
            cout << diff << endl;*/
            if(diff <= mergecap){
                //cout << (*itr)[0] << " " << (*itr2)[0] << endl;
                v.push_back(pair<int, int>((*itr)[0], (*itr2)[0]));
            }
           // else{ cout << "fuu " << endl;}
        }
    }

    //HERES WHERE IT GETS WEIRD
    vector<vector<int>* > new_adj_map(graph.size(), NULL);
    vector<vector<int>* > new_adj_groups(adj_groups.size(), NULL);
    for(list<vector<int> >::iterator itr=adj_groups.begin(); itr!=adj_groups.end(); itr++){
        vector<int>* p = new vector<int>;
        new_adj_groups.push_back(p);
        for(int i=0; i<itr -> size(); i++){
            p -> push_back((*itr)[i]);
            new_adj_map[(*itr)[i]] = p;
        }
        sort(p->begin(), p->end());
    }

    cout << "A" << endl;
    for(int i=0; i<v.size(); i++){
        //cout << "-------------" << endl;
        //cout << v[i].first << " " << v[i].second << endl;
        Merge_two_groups2(new_adj_map, new_adj_map[v[i].first], new_adj_map[v[i].second]);
    }
    cout << "B" << endl;

    ofstream out("new_merged");
    adj_groups.clear();
    adj_map = vector<adj_itr>(graph.size(), adj_groups.end());
    for(int i=0; i<graph.size(); i++){
        if(new_adj_map[i] == NULL){continue;}
        if(new_adj_map[i] -> empty()){continue;}
        if((*new_adj_map[i])[0] != i){continue;}
        vector<int> t;
        adj_groups.push_back(t);
        adj_itr current_itr = adj_groups.end();
        current_itr--;
        for(int j=0; j<(new_adj_map[i])->size(); j++){
            int current_node = (*(new_adj_map[i]))[j];
            out << current_node << " ";
            (adj_groups.back()).push_back(current_node);
            adj_map[current_node] = current_itr;
        }
        out << endl;
    }
    cout << "C" << endl;
}














void merge_similar_adj_groups(int max_diff, int total_diff, vector<list<int> >& graph, vector<set<int> >& K_neighborhood, list<vector<int> >& adj_groups, vector<adj_itr> adj_map){
    vector<bool> altered_node(graph.size(), false);
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
            if(diff == -1){continue;}
            //LAST PART OF BOOL TEST SHOULD NOT BE NECESSARY FiX IT!!!!!!!!!!!!!!!
            if(diff < min_diff && diff != 0 && adj_map[i] != adj_map[*itr]){
                 min_diff = diff; min_pos = *itr;
            }
        }
        if(min_pos != -1){
            swaps << i << " " << min_pos << endl;
            total_changes++;
            altered_node[i] = true;
            altered_node[min_pos] = true;
            current_diff += min_diff;

            //merges two adj_groups
            Merge_two_groups(adj_groups, adj_map, altered_node, adj_map[i], adj_map[min_pos]);

        }
    }
}


//can be made more efficient
void deterministic_merge(vector<list<int> >& graph, vector<set<int> >& K_neighborhoods, list<vector<int> >& adj_groups, vector<adj_itr>& adj_map, int mergecap){
    vector<pair<int, int> > v;
    vector<bool> altered_node(graph.size(), false);
    for(list<vector<int> >::iterator itr = adj_groups.begin(); itr != adj_groups.end(); itr++){
        if(itr->size() == 0){continue;}
        for(list<vector<int> >::iterator itr2 = itr; itr2 != adj_groups.end(); itr2++){
            if(itr == itr2 || (itr2 -> size() == 0)){continue;}
            int diff = find_group_difference((*itr)[0], (*itr2)[0], K_neighborhoods[(*itr)[0]], K_neighborhoods[(*itr2)[0]]);
            if(diff == -1 || diff == 0){continue;}
            /*for(set<int> ::iterator setitr = K_neighborhoods[(*itr)[0]].begin(); setitr != K_neighborhoods[(*itr)[0]].end(); setitr++){cout << *setitr << " ";}
            cout << endl;
            for(set<int> ::iterator setitr = K_neighborhoods[(*itr2)[0]].begin(); setitr != K_neighborhoods[(*itr2)[0]].end(); setitr++){cout << *setitr << " ";}
            cout << endl;
            cout << diff << endl;*/
            if(diff <= mergecap){
                //cout << (*itr)[0] << " " << (*itr2)[0] << endl;
                v.push_back(pair<int, int>((*itr)[0], (*itr2)[0]));
            }
           // else{ cout << "fuu " << endl;}
        }
    }
    for(int i=0; i<v.size(); i++){
        cout << "-------------" << endl;
        cout << v[i].first << " " << v[i].second << endl;
        Merge_two_groups(adj_groups, adj_map, altered_node, adj_map[v[i].first], adj_map[v[i].second]);
    }
}




void wesley_merge(std::vector<std::list<int> >& graph, std::vector<std::set<int> >& K_neighborhoods, std::list<std::vector<int> >& adj_groups, vector<adj_itr> adj_map, int mergecap){
    //above is compatibility converting for making adj_groups a list instead of a vector and making adj_map contain iterators instead of ints
    std::map<int,std::vector<std::pair<int,int> > > pot_merges;
    vector<bool> altered_node(graph.size(), false);

    int temp_diff;
    std::vector<std::pair<int,int> > temp_vec;
    for(unsigned int i = 0; i < graph.size(); i++){
        for(std::list<int>::iterator itr = graph[i].begin(); itr != graph[i].end(); itr++){
            if(adj_map[i] == adj_map[*itr]){continue;}
            temp_diff = find_group_difference(i, *itr, K_neighborhoods[i], K_neighborhoods[*itr]);
            if(temp_diff == -1){continue;}
            if(temp_diff > mergecap){continue;}
            if(pot_merges.find(temp_diff) == pot_merges.end()){
                temp_vec.clear();
                temp_vec.push_back(std::pair<int,int>(i,*itr));
                pot_merges.insert(std::pair<int,std::vector<std::pair<int,int> > >(temp_diff,temp_vec));
            }
            else{
                pot_merges.find(temp_diff) -> second.push_back(std::pair<int,int>(i,*itr));
            }
        }
    }
    int mergecount = 0;
    for(std::map<int,std::vector<std::pair<int,int> > >::iterator itr = pot_merges.begin(); itr != pot_merges.end(); itr++){
        for(unsigned int i = 0; i < itr -> second.size(); i++){
            if(altered_node[itr -> second[i].first] == false && altered_node[itr -> second[i].second] == false /*&& mergecount <= mergecap*/){
                altered_node[itr -> second[i].first] = true;
                altered_node[itr -> second[i].second] = true;
                if(Merge_two_groups(adj_groups, adj_map, altered_node, adj_map[itr -> second[i].first], adj_map[itr -> second[i].second])){
                    mergecount++;
                }
            }
        }
    }
}
