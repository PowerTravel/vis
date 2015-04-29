#include "RenderNode.hpp"
#include "NodeVisitor.hpp"
#include "PhysicsVisitor.hpp"
#include "Geometry.hpp"

RenderNode::RenderNode()
{
	clear();
}

RenderNode::~RenderNode(){};

void RenderNode::clear()
{
	_pn = NULL;
	_M = mat4(1.0f);
	_V = mat4(1.0f);
	_P = mat4(1.0f);

}

void RenderNode::acceptVisitor(NodeVisitor& v)
{	
	v.apply(this);
}
#include <iostream>
void RenderNode::setM(mat4 m)
{
	_M =_M * m;
	updateBoundingBox();
}

void RenderNode::updateBoundingBox()
{
	if(_pn!= NULL )//&& _M != mat4(1.0))
	{
		_bb = _pn->getBoundingBox();
		_bb.transform(_M);
	}
}

void RenderNode::setV(mat4 m)
{
	_V = m;
}
void RenderNode::setP(mat4 m)
{
	_P = m;
}
mat4 RenderNode::getM()
{
	return _M;
}
mat4 RenderNode::getV()
{
	return _V;
}
mat4 RenderNode::getP()
{
	return _P;
}

void RenderNode::setPhysicsNode(PhysicsInterface* n)
{
	_pn = n;
	updateBoundingBox();
}

PhysicsInterface* RenderNode::getPhysicsNode()
{
	return _pn;
}

void RenderNode::send_data_to_shader()
{
	// Apply the state to openGL
	_state.apply();

	// Send the Model View and projection matrices to the shader
	shader_ptr shader;
	_state.get(State::Attribute::SHADER, shader);
	shader->setUniformMatrix("M", 1, &_M[0][0]);
	shader->setUniformMatrix("V", 1, &_V[0][0]);
	shader->setUniformMatrix("P", 1, &_P[0][0]);
}

void RenderNode::draw()
{
	if(_pn != NULL)
	{
		send_data_to_shader();
		_pn -> draw();
	}
}

void RenderNode::acceptPhysicsVisitor(PhysicsVisitor& v)
{
	if(_pn != NULL)
	{
		_pn->acceptPhysicsVisitor(v);
	}
}
