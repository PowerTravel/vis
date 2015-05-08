#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "Node.hpp"
#include <assimp/scene.h>

#include <GL/glew.h>
#include <vector>
#include <map>


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
class Geometry : public PhysicsInterface{


	public:
		enum DataType{
			VERTEX,
			TEXTURECOORDINATE,
			NORMAL,
			FACE,
			STREAM_0,
			STREAM_1,
			STREAM_2,
			STREAM_3,
			STREAM_4
		};

		Geometry();
		Geometry(int nVerts, int nFaces, float* verts, float* norm, int* face, float* texCoords);
		Geometry(const char* filePath);
		Geometry(const aiMesh* mesh);
		virtual ~Geometry();

		void acceptVisitor(NodeVisitor& v);
		void draw();
		void draw(int N, int n, float* points);
		void upload_stream_data(int channel, int N, int n,  float* data);
		
		void acceptPhysicsVisitor(PhysicsVisitor& v);

		unsigned int getVAO();
		int getNrFaces();
		int getNrVertives();

		// Helperfunction that loads a file
		static geometry_vec loadFile(const char* filePath);
		// Create a geometry directly from arrays of floats and ints.
		void createGeom(int nVerts, int nFaces, float* verts, float* norm, int* face, float* texCoords);

		bool zombie();
	protected:

		
	private:
		
		struct element_data{
			// Must be size one through four
			int size;
			int stride;
		};

		// All stream data is assumed to be in float;	
		class StreamData{
			private: 
				GLuint instanceBuffer;
				int nr_of_elements;

			public:
				StreamData()
				{
					glGenBuffers(1, &instanceBuffer);
				}
				virtual ~StreamData(){};
				std::vector<element_data> e_data;
		//		int size(){
		//			return nr_of_elements * size_of_element * sizeof(GLfloat);
		//		}
		};


		int nrVertices;
		int nrFaces;

		bool loaded;

		GLuint VAO;
		GLuint vertexBuffer;
		GLuint textureBuffer;
		GLuint normalBuffer;
		GLuint faceBuffer;
		GLuint instanceBuffer;
		std::map<DataType, StreamData> stream_buffer_table;
	
		bool instancing;

		void createGeom( const aiMesh* mesh );
		void loadVertices(int nrVertices, float* vertices);
		void loadTextureCoordinates(int nrTexCoords, float* coords);
		// Assumes they are triangles
		void loadFaces(int nrFaces, int* faces); 
		void loadNormals(int nrNormals, float* normals);
		void create_instanceBuffer(int N, int n=3, int channel = 0);
		DataType getStreamChannel(int channel);
};

#endif // GEOMETRY_HPP
