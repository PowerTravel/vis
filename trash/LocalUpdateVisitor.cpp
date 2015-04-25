#include "LocalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

#include <iostream>

LocalUpdateVisitor::LocalUpdateVisitor()
{

}

LocalUpdateVisitor::~LocalUpdateVisitor(){}

void LocalUpdateVisitor::apply(Geometry* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(ParticleSystem* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(Group* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(Transform* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(Camera* n)
{
	n->update();
}
