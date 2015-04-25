#ifndef RENDER_NODE_HPP
#define RENDER_NODE_HPP

#include "Node.hpp"
#include "MatLib.hpp"
class Geometry;

#ifndef RENDER_NODE_PTR
#define RENDER_NODE_PTR
class RenderNode;
typedef std::shared_ptr<RenderNode> render_node_ptr;
#endif // RENDER_NODE_PTR

class RenderNode : public VirtualRenderNode{
	
	public:

		RenderNode();
		virtual ~RenderNode();

		RenderNode& operator=(RenderNode& r);

		void acceptVisitor(NodeVisitor& v);

		void setM(mat4 m);
		void setV(mat4 m);
		void setP(mat4 m);
		mat4 getM();
		mat4 getV();
		mat4 getP();

		void clear();

		void setGeometry(Geometry* g);
		VirtualRenderNode* getGeometry();

		void draw();

	private:
		mat4 _M,_V,_P;
		VirtualRenderNode* _g;
	
		void send_data_to_shader();
		void updateBoundingBox();

};


#endif // RENDER_NODE_HPP
