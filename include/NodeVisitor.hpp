#ifndef NODEVISITOR_HPP
#define NODEVISITOR_HPP

#include "Node.hpp"
#include "RenderNode.hpp"
class Node;
class Geometry;
class Group;
class Transform;
class Camera;
class ParticleSystem;
//class RenderNode;
//class RenderToTexture;

struct RenderList{

	std::list<RenderNode> list;

	RenderList(){ list = std::list<RenderNode>(); };
	virtual ~RenderList(){};


	//void draw();
	void draw()
	{
		for(auto it = list.begin(); it != list.end(); it++)
		{
			it->draw();
		}
	};
};
/*	
 *	Class: 		NodeVisitor
 *	Purpose: 	BaseClass for the visitor classes. 
 *	Misc:		All other visitors inherit from this one.
 */
class NodeVisitor{

	public:


		NodeVisitor();
		virtual ~NodeVisitor();

		// A method for traversing the scene graph
		virtual void traverse( Group* node );

		// Each node has their own apply function
		virtual void apply(Geometry* n);
		virtual void apply(RenderNode* n);
		virtual void apply(Group* n);
		virtual void apply(Transform* n);
		virtual void apply(Camera* n);
		virtual void apply(ParticleSystem* n);
//		virtual void apply(RenderToTexture* n);

		RenderList* getRenderList();

	protected:
		static std::shared_ptr<RenderList> _rList;
		std::list<RenderNode>::iterator _rit;

		virtual void init(Group* node);
		virtual void reset(Group* node);
	private:
		void doTraverse(Node* node);
};


#endif // NODEVISITOR_HPP
