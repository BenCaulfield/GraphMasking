#include <sstream>
#include <stdlib.h>
#include <string>
#include <fstream>

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
       while(num_tests <= 0){
        string test_folder = "test_results" + num_to_string(k_val) + "_" + num_to_string(num_tests);
        string make_dir = "mkdir " + test_folder;
        system(make_dir.c_str());
        int tests = 166;
        int max_tests = 405;
        //string results = "my_alg_results";
        ofstream out("my_alg_results");
        string s;
        while(tests <= max_tests){
            string input = "./test.exe ./binary_networks/test_data_2/data_test" + num_to_string(tests) + " " + num_to_string(k_val) + " my edge_frequency 20";
            system(input.c_str());

            ifstream in2(("./binary_networks/test_data_2/data_test" + num_to_string(tests)).c_str());
            in2 >> s; in2 >> s; out << s << " "; in2 >> s; in2 >> s; in2 >> s; out << s << " "; in2 >> s; in2 >> s; out << s << " ";
            ifstream in("edge_frequencies.txt");
            in >> s; out << s << " "; in >> s; out << s << " "; out << endl;

            string command = "mv edge_frequencies.txt " + test_folder + "/edge_frequencies" + num_to_string(tests);
            system(command.c_str());
            tests++;
        }
        string move_results = "mv my_alg_results " + test_folder;
        system(move_results.c_str());

         num_tests++;
        }
       k_val++;
      }
}