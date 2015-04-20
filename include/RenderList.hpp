#ifndef RENDER_LIST
#define RENDER_LIST

#include "RenderNode.hpp"
//#include <list>

class RenderList{

	public:
		RenderList();
		virtual ~RenderList();
		void draw();

		void build(Node* node);
		RenderNode* get();

		void begin();
		void next();
		int size();
		bool isEnd();



		void print();
	private:
		std::list<RenderNode> _list;
		std::list<RenderNode>::iterator _it;

		void grow(int n);
		void reset();
		void traverse(Node* node);

};


#endif // DRAW_LIST_HPP
