#include <vector>
#include <set>
#include <iostream>
#include "kSort.h"

std::vector<std::set<int> > generate();
std::vector<std::vector<int> > kSort(std::vector<std::set<int> > k_neighborhood_sets);



int main()
{
  std::vector<std::set<int> > nodes = generate();
	
	kSort(nodes);
	
	return 0;
}

std::vector<std::set<int> > generate()
{
	std::set<int> s;
	std::vector<std::set<int> > v;
	
	s.insert(1);
	s.insert(9);
	s.insert(53);
	v.push_back(s);
	s.clear();
	
	s.insert(1);
	s.insert(2);
	s.insert(3);
	s.insert(4);
	s.insert(5);
	v.push_back(s);
	s.clear();
	
	s.insert(9);
	s.insert(15);
	s.insert(2);
	s.insert(723);
	s.insert(9);
	v.push_back(s);
	s.clear();
	
	s.insert(7);
	s.insert(5);
	s.insert(52);
	s.insert(34);
	s.insert(11);
	s.insert(41);
	s.insert(3);
	v.push_back(s);
	s.clear();
	
	return v;
}

std::vector<std::vector<int> > kSort(std::vector<std::set<int> > k_neighborhood_sets)
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
		tree.insert(k_neighborhood_vecs[i]);
	}
	
	std::vector<std::vector<int> > sorted = tree.sort();
	
	for(unsigned int i = 0; i < sorted.size(); i++)
	{
		for(unsigned int j = 0; j < sorted[i].size(); j++)
		{
			std::cout << i << "," << j << ":" << sorted[i][j] << std::endl;

			//std::cout << sorted[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	return sorted;
	
}
