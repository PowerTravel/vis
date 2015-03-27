#include "Shader.hpp"
#include <iostream>

// Initiates the gl program. Reads, compiles and links the shaders.
Shader::Shader(const char* vShaderFile, const char* fShaderFile)
{
	this->program = 0;
	// Creates a new openGL program and returns a reference.
	GLuint program = glCreateProgram();

	// Initiates two shader structs with data.
	ShaderFile shaders[2] = {
		{vShaderFile, GL_VERTEX_SHADER, NULL, 0},
		{fShaderFile, GL_FRAGMENT_SHADER, NULL, 0}
		};

	// Read and compile the two shaders.
	for(int i=0; i<2; i++ )
	{
		ShaderFile* s = &shaders[i];

		s->source = readShaderSource(s->filename);
		if( s->source == NULL){
			fprintf(stderr, "Failed to read %s\n", s->filename);
			exit( EXIT_FAILURE );
		}
		
		// Create a new shader
		GLuint shader = glCreateShader( s->type );
		// Copy the source code to the newly created shader object.
		glShaderSource(shader, 1, (const GLchar**) &s->source, NULL  );
		delete s->source;
		// Comile the shader.
		glCompileShader( shader );

		// See that the compiling whent okay
		GLint compiled;
		// Get compilation status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if( !compiled )
		{
			fprintf(stderr, "failed to compile %s\n", s->filename);

			char* logmsg;
			GLint logsize;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize );
			logmsg = new char[logsize];
			if(logmsg == NULL){
				perror("initprogram in gltools.cpp failed:");
			}

			glGetShaderInfoLog(shader, logsize, NULL, logmsg);
			fprintf(stderr, "%s\n", logmsg);
			delete logmsg;

			exit( EXIT_FAILURE  );
		}

		// Attatch the shader to the program
		glAttachShader(program, shader);
		shaders[i].id = shader;
	}


	// Link the program object to the program
	glLinkProgram( program );
	// After the program is linked the shaders can be deleted without affecting
	// the program.
	glDeleteShader(shaders[0].id);
	glDeleteShader(shaders[0].id);


	// Check that the program object linked okay.
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if( !linked )
	{
		fprintf(stderr, "Shader program failed to link.\n");		

		char* logmsg;
		GLint logsize;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize );
		logmsg = new char[logsize];
		if(logmsg == NULL){
			perror("initprogram in gltools.cpp failed:");
		}

		glGetProgramInfoLog(program, logsize, NULL, logmsg);
		fprintf(stderr, "%s\n", logmsg);
		delete logmsg;

		exit( EXIT_FAILURE  );
	}
	
	this->program = program;

	// M, V, P are default variables that all shaders are assumed to
	// use, That is Model, View and Projection matrices
	createUniform("M");
	createUniform("V");
	createUniform("P");
}

Shader::~Shader()
{

}

// Reads the content of a .glsl shader file and stores it in an allocated char array.
char* Shader::readShaderSource(const char* shaderFile)
{
	// open the file for reading
	FILE* fp = NULL; 
	char* buf = NULL;
	long size = 0;

	fp = fopen(shaderFile, "r");
	if( fp == NULL ){
		perror("readshadersource in gltools.cpp failed");
		return NULL;
	}

	// set the filepointer to eof
	fseek(fp, 0l, SEEK_END);
// get the size in bytes of the file
	size = ftell(fp);

	// returns the fp to the beginning of the file
	fseek(fp, 0l, SEEK_SET);
	// allocate memory for the file
	buf = new char[size+1];
	if(buf == NULL){
		perror("readshadersource in gltools.cpp failed");
		return NULL;
	}

	// reads to buf from fp and nullterminate the string.
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


void Shader::use()
{
	glUseProgram(this->program);
}


#include <iostream>
void Shader::createAttribute(std::string name)
{
	GLuint location = glGetAttribLocation(program, name.c_str());
	std::pair< std::string, GLuint > pair(name,location);
	ut.insert(pair);

}
void Shader::createUniform(std::string name)
{
	GLuint location = glGetUniformLocation(program, name.c_str() );
	std::pair< std::string, GLuint > pair(name,location);
	ut.insert(pair);
}
void Shader::setUniformMatrix(std::string name, int count,  float* data)
{	
	glUniformMatrix4fv(ut[name.c_str()], count, GL_FALSE, data);
}
void Shader::setUniform1f(std::string name, int count, float* data)
{
	glUniform1fv(ut[name.c_str()], count, data);
}
void Shader::setUniform2f(std::string name, int count, float* data)
{
	glUniform2fv(ut[name.c_str()], count, data);
}
void Shader::setUniform3f(std::string name,int count, float* data)
{
	glUniform3fv(ut[name.c_str()], count, data);
}
void Shader::setUniform4f(std::string name,int count, float* data)
{
	glUniform4fv(ut[name.c_str()], count, data);
}
void Shader::setUniform1i(std::string name,int count, int* data)
{
	glUniform1iv(ut[name.c_str()], count, data);
}
void Shader::setUniform2i(std::string name,int count, int* data)
{
	glUniform2iv(ut[name.c_str()], count,data);
}
void Shader::setUniform3i(std::string name,int count, int* data)
{
	glUniform3iv(ut[name.c_str()], count,data);
}
void Shader::setUniform4i(std::string name,int count, int* data)
{
	glUniform4iv(ut[name.c_str()], count,data);
}


GLuint Shader::getUniform(std::string name)
{
	return ut[name.c_str()];
}
