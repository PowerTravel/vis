#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Node.hpp"
#include <assimp/scene.h>

#include <GL/glew.h>
#include <vector>


#ifndef GEOMETRY_VEC
#define GEOMETRY_VEC
class Geometry;
typedef std::vector< std::shared_ptr<Geometry> > geometry_vec;
#endif //GEOMETRY_VEC


#ifndef GEOMETRY_PTR
#define GEMOETRY_PTR
class Geometry;
typedef std::shared_ptr<Geometry> geometry_ptr;
#endif // GEOMETRY_PTR

/*
 * Class:	Geometry
 * Purpose:	Loads geometries
 * Misc:
 */
class Geometry : public VirtualRenderNode{
	public:
		enum DataType{
			VERTEX,
			TEXTURECOORDINATE,
			NORMAL,
			FACE,
			STREAM
		};

		Geometry();
		Geometry(const aiMesh* mesh);
		virtual ~Geometry();

		void acceptVisitor(NodeVisitor& v);
		void draw();

		unsigned int getVAO();
		int getNrFaces();
		int getNrVertives();

		// Helperfunction that loads a file
		static geometry_vec loadFile(const char* filePath);
		// Create a geometry directly from arrays of floats and ints.
		void createGeom(int nVerts, int nFaces, float* verts, float* norm, int* face, float* texCoords);

//		void getBoundingBoxCorners(double* points);
	protected:

		int nrVertices;
		int nrFaces;
		
		GLuint VAO;
		
	private:

		bool loaded;

		GLuint vertexBuffer;
		GLuint textureBuffer;
		GLuint normalBuffer;
		GLuint faceBuffer;
		
		void createGeom( const aiMesh* mesh );
		void loadVertices(int nrVertices, float* vertices);
		void loadTextureCoordinates(int nrTexCoords, float* coords);
		// Assumes they are triangles
		void loadFaces(int nrFaces, int* faces); 
		void loadNormals(int nrNormals, float* normals);
};

#endif // GEOMETRY_HPP
