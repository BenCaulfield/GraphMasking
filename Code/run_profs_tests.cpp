#include <sstream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

string num_to_string(int num){
    stringstream convert;   // stream used for the conversion
    convert << num;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str();
    //return "";
}

string doub_to_string(double doub){
    stringstream strs;
    strs << doub;
    return strs.str();
}

int main(){
    int k_val = 2;
      while(k_val <= 4){
       int num_tests = 0;
       while(num_tests <= 1){
        string test_folder = "test_results" + num_to_string(k_val) + "_" + num_to_string(num_tests);
        string make_dir = "mkdir " + test_folder;
        system(make_dir.c_str());
        int tests = 1;
        int max_tests = 165;
        //string results = "my_alg_results";
        ofstream out("profs_alg_results");
        string s;
        while(tests <= max_tests){
            //cout << "test " << tests << " begun." << endl;
            string input = "./test.exe ./binary_networks/test_data/data_test" + num_to_string(tests) + " " + num_to_string(k_val) + " profs both 20";
            system(input.c_str());

            //cout << "A" << endl;
            ifstream in2(("./binary_networks/test_data/data_test" + num_to_string(tests)).c_str());
            in2 >> s; in2 >> s; out << s << " "; in2 >> s; in2 >> s; in2 >> s; out << s << " "; in2 >> s; in2 >> s; out << s << " ";
            ifstream in("overlap.txt"); double worst_overlap = 1; in >> s;
            while(s != "average_overlap"){worst_overlap = min(worst_overlap, atof(s.c_str())); in >> s;}
            in >> s;
            out << s << " " << worst_overlap << "\n";
            //cout << "B" << endl;

            ifstream in3("edge_frequencies.txt");
            in3 >> s; out << s << " ";
            in3 >> s; out << s << endl;
            //nodes average degree mu-value average_overlap worst_overlap total-edges constant-edges

            string command = "mv overlap.txt " + test_folder + "/overlap" + num_to_string(tests);
            system(command.c_str());
            command = "mv edge_frequencies.txt " + test_folder + "/edge_frequencies" + num_to_string(tests);
            system(command.c_str());
            //cout << "test " << tests << "complete." << endl;
            tests++;
        }
        string move_results = "mv profs_alg_results " + test_folder;
        system(move_results.c_str());

         num_tests++;
        }
       k_val++;
      }
}