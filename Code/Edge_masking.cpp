#include<map>

using namespace std;

vector<float> Create_Histogram(vector<vector<list<int> > >& final_graphs){
    map<pair<int, int>, int> edge_frequencies;
    pair<int, int> temp;
    for(int i=0; i<final_graphs.size(); i++){
        for(int j=0; j<final_graphs[i].size(); j++){
            temp.first = j;
            for(list<int>::iterator itr = final_graphs[j].begin(); itr != final_graphs[j].end(); itr++){
                temp.second = *itr;
                int map_size = edge_frequencies.size();
                int& map_value = map[temp];
                if(map_size ==edge_frequencies.size()){map_value++;}//the edge 'temp' was already in the map
                else {map_value = 1;}
            }
        }
    }

    vector<int> frequencies;
    for(map<pair<int, int>, int>::iterator itr = edge_frequencies.begin(); itr != edge_frequencies.end(); itr++){
        while(frequencies <= itr -> second){frequencies.push_back(0);}
        frequencies[itr -> second]++;
    }
    vector<float> histogram;
    for(int i=0; i<frequencies.size(); i++){
        histogram.push_back(frequencies[i]/(double)edge_frequencies.size());
    }
    return histogram;
}