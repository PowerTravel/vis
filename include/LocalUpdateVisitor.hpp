#ifndef LOCAL_UPDATE_VISITOR_HPP
#define LOCAL_UPDATE_VISITOR_HPP

#include "NodeVisitor.hpp"


/*	
 *	Class: 		LocalUpdateVisitor
 *	Purpose: 	Traverses the scenegraph and calls the update-method
 *				of all the nodes which if they carry a callback
 *				will call that.
 *	Misc:		Derived from NodeVisitor
 */
class LocalUpdateVisitor : public NodeVisitor{
	
	public:	
		LocalUpdateVisitor();
		virtual ~LocalUpdateVisitor();


		void apply(Geometry* n);
		void apply(ParticleSystem* n);
		void apply(Group* n);
		void apply(Transform* n);
		void apply(Camera* n);

};

#endif // LOCAL_UPDATE_VISITOR_HPP
