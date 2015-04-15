#ifndef RENDER_NODE_HPP
#define RENDER_NODE_HPP

#include "Node.hpp"
#include "MatLib.hpp"
class Geometry;

class RenderNode : public VirtualRenderNode{
	
	public:

		RenderNode();
		virtual ~RenderNode();

		void acceptVisitor(NodeVisitor& v);

		void setM(mat4 m);
		void setV(mat4 m);
		void setP(mat4 m);
		mat4 getM();
		mat4 getV();
		mat4 getP();

		void setGeometry(Geometry* g);
		Geometry* getGeometry();

		void draw();

	private:
		mat4 _M,_V,_P;
		Geometry* _g;
		
		void send_data_to_shader();

};


#endif // RENDER_NODE_HPP
