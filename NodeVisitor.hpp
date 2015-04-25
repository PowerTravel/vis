#ifndef NODE_APPLIER_HPP
#define NODE_APPLIER_HPP


class Node;
class RenderNode;
class Geometry;
class Group;
class Transform;
class Camera;
class ParticleSystem;

class NodeApplier{

	public:

		virtual void apply(Geometry* n) = 0;
		virtual void apply(RenderNode* n) = 0;
		virtual void apply(Group* n) = 0;
		virtual void apply(Transform* n) = 0;
		virtual void apply(Camera* n) = 0;
		virtual void apply(ParticleSystem* n) = 0;

};

#endif // NODE_APPLIER_HPP
