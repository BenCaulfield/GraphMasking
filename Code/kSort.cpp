#include <vector>
#include <set>
#include <iostream>
#include "kSort.h"

std::vector<std::set<int> > generate();
std::vector<std::vector<int> > kSort(std::vector<std::set<int> > k_neighborhood_sets);


/*std::vector<std::set<int> > generate()
  std::set<int> s;
	std::vector<std::set<int> > v;
	
	s.insert(1);
	s.insert(2);
	s.insert(3);
	v.push_back(s);
	s.clear();
	
	s.insert(9);
	s.insert(10);
	s.insert(11);
	s.insert(12);
	s.insert(13);
	v.push_back(s);
	s.clear();
	
	s.insert(4);
	s.insert(5);
	s.insert(6);
	s.insert(7);
	s.insert(8);
	v.push_back(s);
	s.clear();
	
	return v;
}*/

void kSort(std::vector<std::set<int> > k_neighborhood_sets, std::vector<std::vector<int> >& adj_groups)
{
	std::vector<std::vector<int> > k_neighborhood_vecs;
	std::vector<int> tempvec;
	std::set<int>::iterator setitr;
	
	for(unsigned int i = 0; i < k_neighborhood_sets.size(); i++)//moves all k-neighborhoods from sets to vectors
	{
		tempvec.clear();
		for(setitr = k_neighborhood_sets[i].begin(); setitr != k_neighborhood_sets[i].end(); setitr++)//moves all elements in each set into a temporary vector
		{
			tempvec.push_back(*setitr);
		}
		k_neighborhood_vecs.push_back(tempvec);//pushes the temporary vector to the vector vector
	}
	
	kTree tree;
	for(unsigned int i = 0; i < k_neighborhood_vecs.size(); i++)
	{
		tree.insert(k_neighborhood_vecs[i], i);
	}
	
	std::vector<std::vector<int> > sorted = tree.sort();
	
	std::vector<int> node_lookup = tree.isort();
	
	int sort_ind = 0;
	std::vector<int> current_group; current_group.push_back(node_lookup[0]);
	while(sort_ind+1 < sorted.size()){
		bool equal = true;
		if(sorted[sort_ind].size() != sorted[sort_ind+1].size()){equal = false;}
		else{
			int ind1 = 0; int ind2 = 0;
			while(ind1 != sorted[sort_ind].size() && ind2 != sorted[sort_ind+1].size()){
				if(sorted[sort_ind][ind1] == node_lookup[sort_ind+1]){ind1++;}
				else if(sorted[sort_ind+1][ind2] == node_lookup[sort_ind]){ind2++;}
				else if(sorted[sort_ind][ind1] == sorted[sort_ind+1][ind2]){ind1++; ind2++;}
				else{equal = false; break;}
			}
			if(ind1 < sorted[sort_ind].size() && sorted[sort_ind][ind1]!=node_lookup[sort_ind+1]){equal = false;}
			else if(ind2 < sorted[sort_ind+1].size() && sorted[sort_ind+1][ind2]!=node_lookup[sort_ind]){equal = false;}
		}
		if(!equal){
			adj_groups.push_back(current_group);
			current_group.clear();
		}
		current_group.push_back(node_lookup[sort_ind+1]);
		sort_ind++;
	}
	adj_groups.push_back(current_group);
}
