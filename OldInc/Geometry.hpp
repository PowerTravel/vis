#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include "Node.hpp"
#include "Vec3.hpp"
#include <assimp/scene.h>

#include <GL/glew.h>
#include <vector>

class RenderVisitor;

#ifndef GEOMETRY_PTR
#define GEOMETRY_PTR
class Geometry;
typedef std::shared_ptr<Geometry> geometry_ptr;
#endif //GEOMETRY_PTR

#ifndef GEOMETRY_VEC
#define GEOMETRY_VEC
class Geometry;
typedef std::vector< std::shared_ptr<Geometry> > geometry_vec;
#endif //GEOMETRY_VEC


/*
 * Class:	Geometry
 * Purpose:	Loads geometries
 * Misc:
 */
class Geometry : public Node{

	public:

		enum DataType{
			VERTEX,
			TEXTURECOORDINATE,
			NORMAL,
			FACE
		};

		Geometry();
		Geometry(const aiMesh* mesh);
		virtual ~Geometry();

		void acceptVisitor(NodeVisitor& v);
		void draw();

		// Helperfnction that loads a file
		static geometry_vec loadFile(const char* filePath);
		// Create a geometry directly from arrays of floats and ints.
		void createGeom(int nVerts, int nFaces, float* verts, float* norm, int* face, float* texCoords);
	

	private:
		// Center of mass, not used atm.
		Vec3 _cm;

		bool loaded;

		int nrVertices;
		int nrFaces;
		
		GLuint VAO;
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
