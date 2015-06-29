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
		//void draw(int N, int n, float* points);
		void draw( int n, float* points);
		void upload_stream_data(int channel, int N, int n,  float* data);
		void push_stream_element(int channel, int N, int size, int stride, int start);
		
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
		// All stream data is assumed to be in float;	
		class StreamBuffer{
			private: 

				struct element_data{
					int channel; 	// The channel to use in the shader to access the data. Must be a positive integer;
					int N;			// Nr of elements
					int size;		// The size of the data, can be 1,2,3 or 4;
					int stride;		// The stride between each element in # of float points
					int start;		// The start of the first element
				};

				// ID of the buffer
				GLuint instanceBuffer;
				// Data in the buffer
				std::vector<element_data> e_data;

				bool initiated;

			public:
				// Information about how to read and store Elements	in a buffer

				StreamBuffer()
				{
					instanceBuffer = 0;
					initiated = false;
				};
				virtual ~StreamBuffer()
				{
					if(instanceBuffer)
					{
						glDeleteBuffers(0, &instanceBuffer);
					}	
				};

				bool isInitiated()
				{
					return initiated;
				};

				// See element_data for description of parameters
				void push_element(int channel, int N, int size, int stride, int start)
				{
					element_data element = {channel, N, size, stride, start};
					if(initiated)
					{
						std::cerr << "Geometry::push_element. Buffer already initiated. You cant modify the buffer after it has been initiated." << std::endl;
						return;
					}
					if( (element.size < 1) || (element.size > 4) )
					{
						std::cerr << "Geometry::Bad_Size_of element data. Allowed sizes are 1 through 4. Size given was " << size << std::endl;
						return;
					}
					e_data.push_back(element);
				};

				void initBuffer(GLuint VAO, GLuint stream_channel)
				{
					if(!initiated)
					{	
						glBindVertexArray(VAO);

						glGenBuffers(1, &instanceBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
						glBufferData(GL_ARRAY_BUFFER, size(), NULL, GL_STREAM_DRAW);

						for(auto it = e_data.begin(); it != e_data.end(); it++)
						{
							int channel = it->channel;
							int size = it->size;
							int stride = it->stride * sizeof(GLfloat);
							int start = it->start * sizeof(GLfloat);
							glVertexAttribPointer(channel, size, GL_FLOAT, GL_FALSE, stride,(void*) &start);
							glEnableVertexAttribArray(channel);
						}	
						
						glBindVertexArray(0);

						initiated = true;
					}
				};

				void fillBuffer(int N, float* data)
				{
					if(initiated)
					{
						glBufferSubData(GL_ARRAY_BUFFER, 0, N * sizeof(GLfloat), data);
						std::cout << "lel" << std::endl;
					}
				};

				int size()
				{
					int size = 0;
					for(auto it = e_data.begin(); it != e_data.end(); it++)
					{
						int size = it->size;
						int N = it->N;
						size += N*size*sizeof(GLfloat);
					}
					return size;
				};
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
		StreamBuffer stream_buffer;
	
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
