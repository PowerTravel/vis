#include "Geometry.hpp"
#include "NodeVisitor.hpp"
//#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


Geometry::Geometry() 
{
	_bb = BoundingBox();
	nrVertices = 0;
	nrFaces = 0;
	loaded = false;
	glGenVertexArrays(1, &VAO);
}

Geometry::Geometry(const aiMesh* mesh)
{
	nrVertices = 0;
	nrFaces = 0;
	glGenVertexArrays(1, &VAO);
	
	createGeom(mesh);
	loaded = true;	
}

Geometry::~Geometry()
{
	if(vertexBuffer)
	{
		glDeleteBuffers(0, &vertexBuffer);	
	}
	if(textureBuffer)
	{
		glDeleteBuffers(0, &textureBuffer);	
	}
	if(normalBuffer)
	{
		glDeleteBuffers(0, &normalBuffer);	
	}
	if(faceBuffer)
	{
		glDeleteBuffers(0, &faceBuffer);	
	}
	
	glDeleteVertexArrays(1, &VAO);
}

/*
 * Name:	loadFile
 * Purpose: loads an .obj file into the gpu
 * Input:	const char* filePath: The filepath of the .obj
 * Output:	a vector of geometries
 * Misc:	The image of the texture is assumed to be in the same
 *			folder as the .obj file
 */
geometry_vec Geometry::loadFile(const char* filePath){

	// Let assimp read the file, Triangluate and do other
	// optimization stuff
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
							aiProcess_OptimizeMeshes |
							aiProcess_Triangulate | 
							aiProcess_JoinIdenticalVertices|
							aiProcess_GenSmoothNormals);
	if(!scene){
		fprintf(stderr, "Failed to load model '%s' \n '%s'\n ", filePath, importer.GetErrorString());
		return geometry_vec();
	}

	geometry_vec geomVec = geometry_vec(scene->mNumMeshes);
	// Start to load meshes
	for(int i=0; i<scene->mNumMeshes; i++){

		aiMesh* mesh = scene-> mMeshes[i];
		if( !mesh )
		{
			fprintf(stderr, "failed to load mesh. \n");
		}else{
			// If the mesh succseded to load we create a new geometry
			// from it
			geomVec[i] = geometry_ptr(new Geometry(mesh));

/*
			// this state will carry materials and textures
			State materialState = State();
			aiMaterial* mat =scene->mMaterials[mesh->mMaterialIndex];
			// If the file had materials we add them to the state
			if(mesh->mMaterialIndex != 0){
				materialState.setMaterial(Material(mat));
			}
	
			// Then we push all the diffuse textures
			for(int j=0; j<mat->GetTextureCount(aiTextureType_DIFFUSE); j++){
				aiString path;
				if(mat->GetTexture(aiTextureType_DIFFUSE, j, &path) == AI_SUCCESS){
					std::string fullPath = filePath;
					std::string fileName = path.C_Str();
					fullPath = fullPath.replace(fullPath.rfind("/")+1, std::string::npos, path.C_Str() );
					
					Texture t = Texture(fullPath);
					if(t.loaded())
					{
						materialState.addTexture(State::DIFFUSE, t);
					}
				}
			}
			// And merge the state with the geometry
			geomVec[i] -> setState( &materialState);
*/
		}
	}
	return geomVec;
}

/*
 * Name:	createGeom
 * Purpose:	Loads a geometry from arrays of floats and ints.
 * Input:	-int nrVerts: The number of vertices 
 *			-int nrFaces: The number of faces
 *			-float* verts: The array containing the verices. Assumed
 *			to be 3*nrVertices long.
 *			-float* norm: The array containing the normals. Assumed
 *			to be 3*nrVertices long.
 *			-int* face: The array of faces.Assumed to be 3*nrFaces
 *			long. That is, the faces are assumed to be triangles.
 *			-float* texCoords: The array of texture coordinates.
 *			Assumed to be 2*nrVertices long.
 * Output: 	-	
 * Misc:	Requires vertices, normals and faces, but not 
 *			texturecoordinates.
 */
void Geometry::createGeom(int nVerts, int nFaces, float* verts, float* norm, int* face, float* texCoords)
{
	_bb = BoundingBox(nVerts, verts);

	nrVertices = nVerts;
	nrFaces = nFaces;	


	if(verts != NULL)
	{
		loadVertices(nrVertices, verts);
	}

	if(texCoords!=NULL)
	{
		loadTextureCoordinates(nrVertices, texCoords);
	}

	if( norm != NULL)
	{
		loadNormals(nrVertices, norm);
	}

	if( face != NULL){
		loadFaces(nrFaces, face);
	}

	loaded = true;
}

/*
 * Name:	createGeom	
 * Purpose: Loads a geometry to  the GPU from an aiMesh*
 * Input:	const aiMesh* mesh: A mesh aquired from assimp.
 * Output:	-
 * Misc:	The mesh needs to be triangulated
 */
void Geometry::createGeom( const aiMesh* mesh )
{
	nrVertices = mesh->mNumVertices;
	nrFaces = mesh->mNumFaces;	

	// Copy over Vertices
	if(mesh->HasPositions()){
		float* vertices = new float[3*nrVertices];

		for(int i = 0; i<nrVertices; i++)
		{
			float v[3] = {	mesh->mVertices[i].x,
							mesh->mVertices[i].y,
							mesh->mVertices[i].z};
			vertices[3*i+0] = v[0];
			vertices[3*i+1] = v[1];
			vertices[3*i+2] = v[2];
		}

		// load the data to the gpu
		loadVertices(nrVertices, vertices);

		_bb = BoundingBox(nrVertices, vertices);

		delete vertices;
	}

	// copy over the texture coordinates
	if(mesh->HasTextureCoords(0)){
		float* texCoords = new float[nrVertices * 2];
		for(int i=0; i<nrVertices; i++)
		{
			texCoords[i*2+0] = mesh->mTextureCoords[0][i].x;
			texCoords[i*2+1] = mesh->mTextureCoords[0][i].y;
		}
	
		// and load them onto the gpu
		loadTextureCoordinates(nrVertices, texCoords);

		delete texCoords;
	}
	
	// copy over the normals
	if( mesh->HasNormals() ){
		float* normals = new float[3*nrVertices];

		for(int i = 0; i<nrVertices; i++){
			normals[3*i+0]=mesh->mNormals[i].x;
			normals[3*i+1]=mesh->mNormals[i].y;
			normals[3*i+2]=mesh->mNormals[i].z;
		}
		
		// and load them over to the gpu
		loadNormals(nrVertices, normals);
		
		delete normals;
	}


	// Copy over Faces
	if( mesh->HasFaces()){
		int* faces = new int[3*nrFaces];

		for(int i = 0; i<nrFaces; i++)
		{
			faces[3*i+0] = mesh->mFaces[i].mIndices[0];
			faces[3*i+1] = mesh->mFaces[i].mIndices[1];
			faces[3*i+2] = mesh->mFaces[i].mIndices[2];
		}
	
		// And load them over to the gpu
		loadFaces(nrFaces, faces);

		delete faces;
	}
}


// The following four functions are almost identical.
// They do the actual loading of data over to the gpu
// to predefined locations.
// Vertecies are loaded to space 0;
void Geometry::loadVertices(int nrVertices, float* vertices)
{

	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*nrVertices*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)NULL);
	glEnableVertexAttribArray(VERTEX);
	
	glBindVertexArray(0);
}
// Texture Coordinates are loaded to space 1;
void Geometry::loadTextureCoordinates(int nrTexCoords, float* coords)
{

	glBindVertexArray(VAO);
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer );
	glBufferData(GL_ARRAY_BUFFER, 2*nrTexCoords*sizeof(GLfloat), coords, GL_STATIC_DRAW);

	glVertexAttribPointer(TEXTURECOORDINATE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)NULL);
	glEnableVertexAttribArray(TEXTURECOORDINATE);

	glBindVertexArray(0);
}
// Normals are loaded to space 2;
void Geometry::loadNormals(int nrNormals, float* normals)
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*nrNormals*sizeof(GLfloat), normals, GL_STATIC_DRAW);

	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0,(GLvoid*) NULL);
	glEnableVertexAttribArray(NORMAL);

	glBindVertexArray(0);
}
// Faces are loaded to space 3;
void Geometry::loadFaces(int nrFaces, int* faces)
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &faceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*nrFaces*sizeof(int), faces, GL_STATIC_DRAW);
	glVertexAttribPointer(FACE, 3, GL_FLOAT, GL_FALSE, 0,(GLvoid*) NULL);
	glEnableVertexAttribArray(FACE);
	glBindVertexArray(0);
}

/*
 * Name:	draw
 * Purpose: Draws the geometry
 * Input:	-
 * Output:	-
 * Misc:	-
 */
#include <iostream>
void Geometry::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3*nrFaces, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Geometry::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
}

unsigned int Geometry::getVAO()
{
	return VAO;
}
int Geometry::getNrFaces()
{
	return nrFaces;
}
int Geometry::getNrVertives()
{
	return nrVertices;
}
