#include <vector>
#include <algorithm>

std::vector<std::vector<int> > combine(std::vector<std::vector<int> > vec1, std::vector<std::vector<int> > vec2);

class kNode
{
  public:
		kNode()
		{
			diff = -1;
			mult = 1;
			left = NULL;
			right = NULL;
		}
		kNode(std::vector<int> k_neighborhood, int val, int num)
		{
			value = k_neighborhood;
			diff = val;
			mult = 1;
			left = NULL;
			right = NULL;
			index = num;
		}
		void insert(std::vector<int> k_neighborhood, int new_diff, int num)//inserts a k-neighborhood into a node or a child node
		{
			if(diff == -1)
			{
				value = k_neighborhood;
				diff = 0;
				index = num;
				return;
			}
			
			for(unsigned int i = std::min(new_diff, diff); i < std::max(k_neighborhood.size(),value.size()); i++)
			{
				if(i >= k_neighborhood.size() || (i < value.size() && k_neighborhood[i] < value[i]))//if the new k-neighborhood is a subset of the current node's neighborhood or the current element of the k-neighborhood is less than the current node's current element
				{
					//insert left
					if(left == NULL)
					{
						left = new kNode(k_neighborhood, i, num);
					}
					else
					{
						left -> insert(k_neighborhood, i, num);
					}
					return;
				}
				else if(i >= value.size() || (i < k_neighborhood.size() && k_neighborhood[i] > value[i]))//if the opposite is true
				{
					//insert right
					if(right == NULL)
					{
						right = new kNode(k_neighborhood, i, num);
					}
					else
					{
						right -> insert(k_neighborhood, i, num);
					}
					return;					
				}
			}

			//if they are equal increment the multiplier
			mult++;
			
			return;
			
		}
		std::vector<std::vector<int> > sort()//returns the sorted vector of k-neighborhoods
		{
			std::vector<std::vector<int> > leftv;
			std::vector<std::vector<int> > rightv;
			leftv.clear();
			rightv.clear();
			
			if(left != NULL)
			{
				leftv = left -> sort();
			}
			
			if(right != NULL)
			{
				rightv = right -> sort();
			}
			
			return combine(leftv,value,rightv,mult);
		}
		std::vector<int> isort()//returns the sorted vector of k-neighborhoods
		{
			std::vector<int> leftv;
			std::vector<int> rightv;
			leftv.clear();
			rightv.clear();
			
			if(left != NULL)
			{
				leftv = left -> isort();
			}
			
			if(right != NULL)
			{
				rightv = right -> isort();
			}
			
			return icombine(leftv,index,rightv);
		}
		std::vector<std::vector<int> > combine(std::vector<std::vector<int> > vec1, std::vector<int> middle, std::vector<std::vector<int> > vec2, int copies)
		{
			for(unsigned int i = 0; i < copies; i++)
			{
				vec1.push_back(middle);
			}
			for(unsigned int i = 0; i < vec2.size(); i++)
			{
				vec1.push_back(vec2[i]);
			}
	
			return vec1;
		}
		std::vector<int> icombine(std::vector<int> vec1, int middle, std::vector<int> vec2)
		{
			vec1.push_back(middle);
			for(unsigned int i = 0; i < vec2.size(); i++)
			{
				vec1.push_back(vec2[i]);
			}
	
			return vec1;
		}
	private:
		std::vector<int> value;//k-neighborhood vector held in node
		int index;//index of k-neighborhood node
		int diff;//index at which it differs from its parent node, 0 if head
		kNode* left;//pointer to left child
		kNode* right;//pointer to right child
};

class kTree
{
	public:
		kTree()
		{
			head = new kNode();
		}
		void insert(std::vector<int> k_neighborhood)//inserts a k-neighborhood into the tree
		{
			head -> insert(k_neighborhood, 0);//calls the kNode insert member function on the head
		}
		std::vector<std::vector<int> > sort()//returns the sorted vector of k-neighborhoods from the tree
		{
			return head -> sort();//calls the kNode sort member function on the head
		}
		
	private:
		kNode* head;		
};

