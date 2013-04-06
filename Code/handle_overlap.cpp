#include "get_neighborhoods.cpp"

using namespace std;

//Finds amount of overlap between two sets using formula: |A intersect B| / |A union B|
double Overlap_Factor(set<int>& adj1, set<int>& adj2, int& problem){
    if(adj1.size() == adj2.size() && adj1.size() == 0){return 1;}
    set<int>::iterator itr1 = adj1.begin();
    set<int>::iterator itr2 = adj2.begin();
    int intersection_size = 0;
    while(itr1 != adj1.end() && itr2 != adj2.end()){
        if(*itr1 < *itr2){itr1++; }
        else if(*itr1 > *itr2){problem=*itr2; itr2++; /*cout << "!";*/ }
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
      //uncomment below if satisfied neighborhoods should be subgraph of actual neighborhoods
      //if(problem != -1){cout << i << " " << problem << endl; node_distance(i, problem, graph, k);}
      out << i << " " << overlap << endl;
      average += overlap;
    }
    average /= (K1.size() - 1);
    out << "average_overlap "<<  average << endl;
}


//a's are adjacent k_neighbors, b's are k+1 borders.
void Modified_Overlap_Factor(set<int>& a1, set<int>& a2, set<int>& b1, set<int>& b2, double& overlap, double& loose_overlap){
    if(a1.size() == a2.size() && a1.size() == 0){overlap = 1; loose_overlap = 1; return;}
    set<int>::iterator itr1 = a1.begin();
    set<int>::iterator itr2 = a2.begin();
    set<int>::iterator bitr1 = b1.begin();
    set<int>::iterator bitr2 = b2.begin();
    vector<int> missing_from_a1;
    vector<int> missing_from_a2;
    int intersection_size = 0;
    while(itr1 != a1.end() && itr2 != a2.end()){
        if(*itr1 < *itr2){itr1++; missing_from_a2.push_back(*itr1);} 
        else if(*itr1 > *itr2){itr2++; missing_from_a1.push_back(*itr2);}
        else {intersection_size++; itr1++; itr2++;}
    }
    //for(int i=0; i<missing_from_a1.size(); i++){cout << missing_from_a1[i] << " ";} cout << endl;
    //for(set<int>::iterator titr = b1.begin(); titr!=b1.end(); titr++){cout << *titr << " ";} cout << endl;
    //for(int i=0; i<missing_from_a2.size(); i++){cout << missing_from_a2[i] << " ";} cout << endl;
    //for(set<int>::iterator titr = b2.begin(); titr!=b2.end(); titr++){cout << *titr << " ";} cout << endl;
    int in_b1=0; int miss_ind=0;
    while(bitr1 != b1.end() && miss_ind != missing_from_a1.size()){
      if(missing_from_a1[miss_ind] > *bitr1){bitr1++;}
      else if(missing_from_a1[miss_ind] < *bitr1){miss_ind++;}
      else{in_b1++; miss_ind++; bitr1++;} 
    }
    int in_b2=0; miss_ind=0;
    while(bitr2 != b2.end() && miss_ind != missing_from_a2.size()){
      if(missing_from_a2[miss_ind] > *bitr2){bitr2++;}
      else if(missing_from_a2[miss_ind] < *bitr2){miss_ind++;}
      else{in_b2++; miss_ind++; bitr2++;} 
    }
    overlap = (double)intersection_size / (double)((a1.size() + a2.size()) - intersection_size);
    //cout << "in_borders: " << in_b1 << " " << in_b2 << endl;
    loose_overlap = (double)(intersection_size + in_b2 + in_b1)/(double)(a1.size() + a2.size() - intersection_size); //+ in_b1 + in_b2);
}

//compares the overlap between k+1 neighborhoods, used for testing adjacency merging
void modified_output_overlap(ofstream& out, vector<set<int> >& K1, vector<set<int> >& K2, vector<set<int> >& K1_border, vector<set<int> >& K2_border, int k){
  double average; double loose_average;
  for(int i=1; i<K1.size(); i++){
      double overlap; double loose_overlap; //loose overlap allows for nodes to be in/from k+1 neighborhoods 
      int d;
     // overlap = Overlap_Factor(K1[i], K2[i], d);
      Modified_Overlap_Factor(K1[i], K2[i], K1_border[i], K2_border[i], overlap, loose_overlap);
      out << i << " " << overlap << " " << loose_overlap << endl;
      average += overlap;
      loose_average += loose_overlap;
    }
    average /= (K1.size() - 1); //add zero in later
    out << "average_overlap "<<  average << endl;
    loose_average /= (K1.size() - 1);
    out << "loose_average " << loose_average << endl;
}