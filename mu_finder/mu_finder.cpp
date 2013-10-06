#include <fstream>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 3)//makes sure there is one command line argument
	{
		cout << "Error!\nTry: ./mu_finder [filename] [num_nodes]" << endl;//error
		
		return 0;
	}
	else
	{
		ifstream infile;
		infile.open(argv[1]);//opens the file
		if(!infile.is_open())//makes sure the file is open
		{
			cout << "Error! The file is not open." << endl;//error
			
			return 0;
		}
		else
		{
			string temp;
			int num_nodes;
			int first,second;
			int total = 0;
			int shared = 0;
			vector<set<int> > edges;
			//alternate begin
			vector<double> mus;
			//alternate end
			
			num_nodes = atoi(argv[2]);
			set<int> templist;
			for(int i = 0; i < num_nodes + 1; i++)//create vector of desired length
			{
				edges.push_back(templist);
				//alternate being
				mus.push_back(0);
				//alternate end
			}
			for(int i = 0; i < 6; i++)//skip useless header information
			{
				infile >> temp;
			}
			
			while(infile >> first)//populate lists
			{
				if(infile >> second)
				{
					edges[first].insert(second);
				}				
			}
			
			for(int i = 1; i < edges.size(); i++)//count up number of pairs of adjacent nodes and number of pairs who share a neighbor
			{
				//alternate begin
				shared = 0;
				total = 0;
				//alternate end
				for(set<int>::iterator itr = edges[i].begin(); itr != edges[i].end(); itr++)
				{
					total++;
					for(set<int>::iterator jtr = edges[i].begin(); jtr != edges[i].end(); jtr++)
					{
						if(edges[*itr].find(*jtr) != edges[*itr].end() && *jtr != *itr && *jtr != i && i != *itr)
						{
							shared++;
					
							//break;
						}
					}
				}
				//alternate begin
				mus[i] = (double)shared/total;
				//alternate end
			}
			
			//alternate begin
			double mu = 0;
			for(int i = 0; i < mus.size(); i++)
			{
				mu+=mus[i];
			}
			mu/=num_nodes;
			//alternate end
	
			//changed for alternate
			cout << "μ=" << mu << endl;
		}
	}
	
	return 0;
}
