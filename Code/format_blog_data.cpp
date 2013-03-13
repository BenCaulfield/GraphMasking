#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <cstdlib>

using namespace std;

int shorten(map<long int, int>& shortener, long int long_int){
    map<long int, int>::iterator itr = shortener.find(long_int);
    if(itr == shortener.end()){shortener[long_int] = shortener.size(); return shortener.size();}
    else{return itr -> second;}
}

bool operator<(const pair<int, int>& p1, const pair<int, int>& p2){
    if(p1.first < p2.first){return true;}
    else if(p1.first > p2.first){return false;}
    else {return p1.second <= p2.second;}
}

int main(int argc, char* argv[]){
    if(argc != 3){cout << "input-file and output-file" << endl; return 0;}
    ifstream in_str(argv[1]);
    ofstream out_str(argv[2]);
    string s;
    map<long int, int> shortener;
    list<pair<int, int> > edges;
    pair<int, int> temp;
    pair<int, int> temp2;

    while(in_str >> s){
        s.resize(s.size() - 1);
        temp.first = shorten(shortener, atoi(s.c_str()));
        in_str >> s; s.resize(s.size() - 1);
        temp.second = shorten(shortener, atoi(s.c_str()));
        in_str >> s;
        edges.push_back(temp);
        temp2.first = temp.second;
        temp2.second = temp.first;
        edges.push_back(temp2);
    }
    edges.sort();
    for(list<pair<int, int> >::iterator itr = edges.begin(); itr != edges.end(); itr++){
        out_str << itr -> first << " " << itr -> second << "\n";
    }
}