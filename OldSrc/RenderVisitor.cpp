#include <GL/glew.h>
#include "RenderVisitor.hpp"
#include "RenderToTexture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"
#include <iostream>
#include "Scene.hpp"


RenderVisitor::RenderVisitor()
{
	std::list<aggregate_data> aList;
	_isModelviewSet = false;
	_t = Timer();
	_t.start();
	_lastTime = _t.getTime();
}

RenderVisitor::~RenderVisitor()
{

}

void RenderVisitor::apply(RenderToTexture* tex)
{
	_rtt = tex;
	tex->bindBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	tex->clearBuffer();
	modify_aList(tex->childList.size(), Hmat(), tex->getState(), tex);
}



/*
 * Name:	apply (Geometry)
 * Purpose: Renders a geometry with the accumulated transformation
 *			matrix and the accumulated state.
 * Input:	Geometry* g: The geometry that is to be rendered
 * Output:	-	
 * Misc:	-
 */
void RenderVisitor::apply(Geometry* g)
{
	// this matrix contain the accumulated ModelMatrix.
	aList.front().m.get(_M);

	// We sync the geometries state with the lates entry in the aList.
	state_ptr state = syncStates(aList.front().s.get(),g->getState() );
	if( state->isShaderSet() )
	{

		float lView[16], lProj[16];
		if(state->getNrLights() != 0)
		{
			getLightViewMat(Vec3(0,0,0),state.get(),lView, lProj );
		}
	
		// We render to depth map if we have lights and passed through
		// a Render to Texture node
		RenderToTexture* rtt = aList.front().rtt;
		if(rtt != NULL && state->getNrLights() != 0)
		{

			glEnable(GL_CULL_FACE);
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			Shader* shader = rtt->getShader();
			shader->use();
			

			shader->setUniformMatrix("M", 1, _M);
			shader->setUniformMatrix("V", 1, lView);
			shader->setUniformMatrix("P", 1, lProj);

			rtt->bindBuffer();
			g->draw();
			rtt->clearBuffer();
		}else{

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// If a material is set previously it overrides
			// any material that may have been loaded to the 
			// geometry from a file.
			if(aList.front().s->isMaterialSet())
			{
				state->setMaterial(aList.front().s->getMaterial());
			}
			if(aList.front().s->hasTexture(State::DIFFUSE))
			{
				state->addTexture(State::DIFFUSE ,aList.front().s->getTexture(State::DIFFUSE));
			}

			// Apply the state to openGL
			state->apply();
		
			initTextures(state.get());

			// Send the Model View and projection matrices to the shader
			sendPVMtoGPU(state->getShader());
			
			if(rtt == NULL && state->getNrLights()!=0){

				Hmat mtmp = Hmat(_M);
				Hmat vtmp = Hmat(lView);
				Hmat ptmp = Hmat(lProj);

//				float biasf[] = { 0.5,0,0,0.5,
//								 0,0.5,0,0.5,
//								 0,0,0.5,0.5,
//								 0,0,0, 1};
				float biasf[] = { 0.5,0,0,0.5,
								 0,0.5,0,0.5,
								 0,0,0.5,0.5,
								 0,0,0, 1};
//				Hmat bias = Hmat(biasf);
//				Hmat pvmTmp;
//				pvmTmp = vtmp*mtmp;			// Viev * Model
//				pvmTmp = ptmp*pvmTmp;		// Project * Viev * Model
//				pvmTmp = bias*pvmTmp;		// Bias * Project * View * Model

//				float shadowBiasPVM[16];
//				pvmTmp.get(shadowBiasPVM);
				//state->getShader()->setUniformMatrix("BiasLightPVM", 1, shadowBiasPVM);

				state->getShader()->setUniformMatrix("B", 1, biasf);
				state->getShader()->setUniformMatrix("Ml", 1, _M);
				state->getShader()->setUniformMatrix("Vl", 1, lView);
				state->getShader()->setUniformMatrix("Pl", 1, lProj);
			}
			
			// draw the geometry
			g->draw();
		}
	}else{
		std::cerr<<"No shader present. Not rendering geometry."<<std::endl;
	}
	
	// Since we are in a leaf we now want to erase all entries
	// in these lists up to the entry that has unvisited children.
	decrease_aList();
}

void RenderVisitor::getLightViewMat(Vec3 at, State* s, float* V, float* P)
{
	// Object position, Light poistion
	Vec3 lp, zn;
	// Get the position of the object being illuminated
	lp = s->getLight(0).getPosition();

	zn = at-lp;

	Vec3 up1, up2;
	up1 = Vec3(0,1,0);
	up2 = Vec3(1,0,0);

	float lenUp1 = up1.norm();
	float lenUp2 = up2.norm();
	float znLen = zn.norm();

	Camera lc;
	if( abs(zn * up1) <= abs(znLen*lenUp1) )
	{
		lc = Camera(lp, at, up1 );
	}else{
		lc = Camera(lp, at, up2 );
	}
   	
	GLint portSize[4];
   	glGetIntegerv( GL_VIEWPORT, portSize );
	float w = (float) portSize[2]-portSize[0];
	float h = (float) portSize[3]-portSize[1];
	lc.setAspect(w/h);
	lc.setPerspectiveProjection();
	lc.update();

	lc.getViewMat().get(V);
	lc.getProjectionMat().get(P);
}

void RenderVisitor::initTextures(State* s)
{
	Shader* shade = s->getShader().get();
	if(s->hasTexture(State::DIFFUSE) && !s->getColorMaterial()){
		// Tell shader we are using a diffuseTexture
		int useDiffuse = 1;
		shade->setUniform1i("usingDiffTexture",1, &useDiffuse);
		s->getTexture(State::DIFFUSE).bind(GL_TEXTURE0);
		int tex = 0;
		shade->setUniform1i("diffuseTextureID",1, &tex);
	}else{
		// unbind any loaded textures
		int useDiffuse = 0;
		shade->setUniform1i("usingDiffTexture",1, &useDiffuse);
	}

	if(_rtt != NULL)
	{
		int useShadow= 1;
		shade->setUniform1i("usingShadowMap",1, &useShadow);
		_rtt->bindTexture(GL_TEXTURE1);
		int shd = 1;
		shade->setUniform1i("shadowMapID",1, &shd);
	}else{
		int useShadow = 0;
		shade->setUniform1i("usingShadowMap",1, &useShadow);
	}
}


void RenderVisitor::sendPVMtoGPU(shader_ptr s)
{
		s->setUniformMatrix("M",1, _M);
		if(_isModelviewSet)
		{
			s->setUniformMatrix("V", 1, _V);
			s->setUniformMatrix("P", 1, _P);
		}else{
			std::cerr<<"No CameraMatrix active. Probably not rendering properly."<<std::endl;
		}
}

/*
 * Name:	Apply (Group)	
 * Purpose: Add entries to the aggregate lists. 
 * Input:	Group* grp: The group to be visited
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::apply(Group* grp)
{
	// This node will never have transformation matrices,
	// But it may have children and a state, thus we add a unit matrix
	// together with the number of children it has.
	modify_aList(grp->childList.size(), Hmat(), grp->getState(), NULL);
}

/*
 * Name:	apply (Camera)
 * Purpose: Adds entries to the aggregate lists
 * Input:	Camera* cam: The camera to be visited
 * Output:	-	
 * Misc:	Assumes we only have one camera. The latest camera
 *			is the only one that will be rendered.
 */
void RenderVisitor::apply(Camera* cam)
{
	// We save the View and Projection Matrices
	cam->getViewMat().get(_V);
	cam->getProjectionMat().get(_P);
	_isModelviewSet = true;

	// See apply (Group)
	modify_aList(cam->childList.size(), Hmat(), cam->getState(), NULL);
}


/*
 * Name:	apply (Transform)	
 * Purpose:	adds its transformationMatrix to the mList as well as
 *			its state.
 * Input:	Transform* t: The transformation node to be visited.	
 * Output:	-	
 * Misc:	-
 */
void RenderVisitor::apply(Transform* t)
{	
	modify_aList(t->childList.size(), t->getM(), t->getState(), NULL);
}

/*
 * Name:	modify_aList	
 * Purpose: Adds or removes entries in the aList
 * Input:	-int count: The number of children the added Hmat
 *			will be relevant for.
 *			-Hmat m: The matrix to be summed up.
 *			-State* s: The state that should be accumulated
 *			-RenderToTexture* t: If we want to render to a texture
 *			instead of to the screen
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::modify_aList(int count, Hmat m, State* s, RenderToTexture* t )
{
	// If the node is a leaf
	if(count == 0)
	{	
		// If we are in a leaf we have to popp all entries
		// that corresponds to already visited nodes with
		// no children. Those entries are the ones where 
		// 'count' has been reduced to 1; 
		decrease_aList();

	// If the node has children we push the matrix
	}else{
		aggregate_data result;
		result.count = count;	
		
		if(aList.empty()){
			// or if this is the first entry we add it as is
			result.m = m;
			
			result.s = state_ptr(new State());
			result.s->merge(s);

			result.rtt = t;
		}else{
			// sum the two latest matrices
			result.m = aList.front().m*m ;
			
			state_ptr oldState = aList.front().s;
			result.s = syncStates(oldState.get(), s);

			if(t != NULL){
				result.rtt = t;	
			}else{
				result.rtt = aList.front().rtt;
			}
		}	

		aList.push_front(result);
	}
}


/*
 * Name:	decrease_aList	
 * Purpose:	Pops entries that corresponds to already visited nodes.
 * 			Those are the entries that have had their 'count' reduced
 *	 		to 1. Also, reduce the next remaining entrys count 
 *			by 1 indicating that one of its childs subtree has been
 *			processed.
 * Input:	-
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::decrease_aList()
{	
	std::list<aggregate_data>::iterator it=aList.begin();
	// pop all entries with count reduced to 1.
	while( it->count <= 1 && it!= aList.end() )
	{
		it++;
		aList.pop_front();
	}
	// Reduce the count of the remaing top with one
	aList.front().count--;
}

/*
 * Name:	syncStates
 * Purpose: merges two states into a new state without affecting the
 *	 		input states.
 * Input:	State* lastState: The state that gets its attributes
 *			overridden.
 *			State* newState: The state that gets to override lastState
 * Output:	std::shared_ptr<State>: The sum of the previous states.
 * Misc:	-	
 */
state_ptr RenderVisitor::syncStates(State* lastState ,State* newState)
{
	state_ptr result = state_ptr(new State());

	result->merge(lastState);
	result->merge(newState);
	
	return result;
	
}
