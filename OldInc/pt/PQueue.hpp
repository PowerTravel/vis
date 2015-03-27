#ifndef _PQUEUE_H
#define _PQUEUE_H

#include <vector>
#include <cmath>

// Heap/Priority queue implemented as a Binary Tree
template <typename T>
class PQueue{
	public:
		PQueue();
		virtual ~PQueue();

		void push( T data, int p);
		void push(std::shared_ptr<T> data, int p);
		bool isEmpty();
		std::shared_ptr<T> pop();
		std::weak_ptr<T> peak();

	private:

		struct node{
			std::shared_ptr<T> data;
			int p; // priority
		};

		std::vector< std::shared_ptr<node> > v;

		void swim();
		bool tryToSwim(int pos, int next);
		void sink();
		int getSinkDirection(int pos, int left, int right);
		void swap(int idx1, int idx2);
		int getLeftChildIdx(int n);
		int getRightChildIdx(int n);
		int getParentIdx(int n);
};

template <typename T>
PQueue<T>::PQueue()
{
	v = std::vector< std::shared_ptr<node> >();
}

template <typename T>
PQueue<T>::~PQueue()
{
	
}

template <typename T>
bool PQueue<T>::isEmpty()
{
	return v.empty();
}


template <typename T>
std::weak_ptr<T> PQueue<T>::peak()
{
	return v.front()->data;
}

template <typename T>
void PQueue<T>::push( T data, int p)
{
	// Construct the new node
	std::shared_ptr<node> new_node = std::shared_ptr<node>(new node);
	new_node->data = std::shared_ptr<T>(new T);
	*new_node->data = data;
	new_node->p = p;

	v.push_back(new_node);
	
	// The new_node "swims" up to correct priority.
	swim();
}

template <typename T>
void PQueue<T>::push(std::shared_ptr<T> data, int p)
{
	// Construct the new node
	std::shared_ptr<node> new_node = std::shared_ptr<node>(new node);
	new_node->data = data;
	new_node->p = p;	
	
	v.push_back(new_node);
	
	// The new_node "swims" up to correct priority.
	swim();
}

template <typename T>
std::shared_ptr<T> PQueue<T>::pop()
{
	// Swap the last leaf with root and shrink the tree
	std::shared_ptr<T> return_data = v.front()->data;
	v.front() = v.back();
	v.pop_back();

	// sinks the top node down to correct priority.
	sink();

	// Return the removed top node.
	return return_data;
}

template <typename T>
void PQueue<T>::swim()
{
	int pos = v.size()-1;	// Current position in tree

	// This swaps pos with next if priority of pos is greater than next
	while( pos > 0 ){

		int next = getParentIdx(pos);

		if( tryToSwim(pos, next) )
		{
			swap(pos, next);
			pos = next;
		}else{	
			return;
		}
	}
}

template <typename T>
bool PQueue<T>::tryToSwim(int pos, int next)
{
	if( next >= 0 )
	{
		int priodiff = v.at(pos)->p - v.at(next)->p;
		if( priodiff > 0 ){
			return true;
		}
	}
	return false;
}

template <typename T>
void PQueue<T>::sink()
{
	if(!isEmpty())
	{
		int pos = 0;			// Current position in tree
		
		while( true )
		{	
			// Get the index of both children
			int next = 0;	// Next position in tree
			
			int l_child_idx = getLeftChildIdx(pos);
			int r_child_idx = getRightChildIdx(pos);
			
			int swap_direction = getSinkDirection(pos, l_child_idx, r_child_idx);
			
			// Left
			if( swap_direction == 1 ){
				next = l_child_idx;
			// Right
			}else if( swap_direction == 2 ){
				next = r_child_idx;
			}else{
				return;
			}
			
			swap(pos, next);
			pos = next;
		}
	}
}

template <typename T>
int PQueue<T>::getSinkDirection(int pos, int left, int right)
{
	int end = v.size();	
	int left_prio_diff = -1;
	int right_prio_diff = -1;
	if(left < end)
	{	
		left_prio_diff = v.at( left )->p - v.at(pos)->p;
	}
	if(right < end )
	{	
		right_prio_diff = v.at( right )->p - v.at(pos)->p;
	}	

	// If the left priority difference is highest we swap the left child
	if( (left_prio_diff > 0) && (left_prio_diff >= right_prio_diff) )
	{
		// left
		return 1;
		
	// Else if the right priority difference is highest we swap the left right
	}else if( (right_prio_diff > 0) && (right_prio_diff > left_prio_diff) )
	{
		// right
		return 2;
	}else{
		// stay
		return 0;
	}
}

template <typename T>
void PQueue<T>::swap(int idx1, int idx2)
{
	std::shared_ptr<node> tmp = v.at(idx1);
	v.at(idx1) = v.at(idx2);
	v.at(idx2) = tmp;
}

template <typename T>
int PQueue<T>::getLeftChildIdx(int n)
{
	return 2*n+1;
}

template <typename T>
int PQueue<T>::getRightChildIdx(int n)
{
	return 2*n+2;
}

template <typename T>
int PQueue<T>::getParentIdx(int n)
{
	return floor((n-1)/2);
}


#endif // PQUEUE
