#ifndef UPDATE_VISITOR_HPP
#define UPDATE_VISITOR_HPP

#include "NodeVisitor.hpp"


/*	
 *	Class: 		UpdateVisitor
 *	Purpose: 	Traverses the scenegraph and calls the update-method
 *				of all the nodes which if they carry a callback
 *				will call that.
 *	Misc:		Derived from NodeVisitor
 */
class UpdateVisitor : public NodeVisitor{
	
	public:	
		UpdateVisitor();
		virtual ~UpdateVisitor();

		void windowChanged();

		void apply(Geometry* n);
		void apply(ParticleSystem* n);
		void apply(Group* n);
		void apply(Transform* n);
		void apply(Camera* n);

	protected:

		virtual void init();
		virtual void cleanup();

	private:
		bool _windowChanged;
};

#endif // UPDATE_VISITOR_HPP
