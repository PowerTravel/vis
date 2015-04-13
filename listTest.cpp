#include <iostream>
#include <list>

int main()
{
	std::list<int> l = std::list<int>();
	std::list<int>::iterator it = l.begin();


	l.push_front(1);
	it = l.begin();	
	std::cout << *it << std::endl;
	it++;	
	it++;	
	if( it == l.end() )
	{
		std::cout << "list at end" << std::endl;
	}
	return 0;
}
