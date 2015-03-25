#ifndef RENDER_VISITOR_HPP
#define RENDER_VISITOR_HPP

#include "NodeVisitor.hpp"
#include "Hmat.hpp"
#include "Timer.hpp"
#include <list>

class RenderToTexture;


/*	
 *	Class: 		RenderVisitor
 *	Purpose: 	Traverses the scenegraph and renders it.
 *	Misc:		Derived from nodeVisitor. This is quite a 
 *				complex class that does very different things
 *				depending on which node it is visiting
 *				and the different nodes interact with eachother.
 */
class RenderVisitor : public NodeVisitor{

	public:

		RenderVisitor();
		virtual ~RenderVisitor();
		
		void apply(RenderToTexture* tex);
		void apply(Geometry* g);
		void apply(Group* grp);
		void apply(Transform* t);
		void apply(Camera* t);

	
	private:

		// The struct that carries the data gatherd from all
		// the nodes the RenderVisitor visits.
		struct aggregate_data{
			Hmat m;		// The composite model matrix
			state_ptr s;// The composite state 
			RenderToTexture* rtt; // If we want to render to texture
			int count;	// The number of unvisited children;
		};

		// The aList is a list of composite model matrices, the
		// merged states and if we want to render to a texture.
		// Hmat m is the product of the
		// total model matrices above it in the tree.
		// Hmat s is the accumulation of all the other states above it
		// in the tree.
		// So when we reach a leaf, the first entry is the sum of all
		// its parents and grandparents
		std::list<aggregate_data> aList;

		// Using vectors since Hmat is acting weird, see below
		float _M[16]; // Model matrix
		float _V[16]; // View matrix
		float _P[16]; // Projection matrix
		bool _isModelviewSet;
		
		// DET ÄR NÅGOT SKUMT MED MODELMAT. Om jag deklarerar
		// deklarerar Hmat MODELVIEW; här och i apply(geometry)
		// anropar MODELVIEW = mList.front().m så får jag segfault
		// Om jag deklarerar en lokal Hmat i apply(geometry) så
		// får jag segfault när jag stänger av programmet.
//		Hmat _V; // View Mat
//		Hmat _P; // Projection Mat


		// Manipulates the aList
		void modify_aList(int count, Hmat m, State* s, RenderToTexture* t );
		void decrease_aList();
	
		//void makeStateCurrent(State* s);
		state_ptr syncStates(State* lastState, State* newState);
		
		void sendPVMtoGPU(shader_ptr s);
		void initTextures(State* s);
		void getLightViewMat(Vec3 at, State* s, float* V, float* P);
		
		// Debug temp members
		RenderToTexture* _rtt;
		Timer _t;
		float _lastTime;
};

#endif // RENDER_VISITOR_HPP
