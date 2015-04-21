#ifndef _MATLIB
#define _MATLIB

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Eigen/Dense"

namespace matlib = glm;
typedef matlib::vec3 vec3;
typedef matlib::vec4 vec4;
typedef matlib::mat4 mat4;



inline Eigen::Matrix4d glm_to_eig(glm::mat4 glm_m)
{
	Eigen::Matrix4d	m;
	// transposing since glm is COLUMN MAJOR, while eigen is ROW MAJOR
	glm::mat4 m_transpose = glm::transpose(glm_m);
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			m(i,j) = m_transpose[i][j];
		}
	}
	return m;
}

inline mat4 eig_to_glm(Eigen::Matrix4d eig_m)
{
	mat4 m;
	// transposing since glm is COLUMN MAJOR, while eigen is ROW MAJOR
	Eigen::Matrix4d m_transpose = eig_m.transpose();
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			m[i][j] = m_transpose(i,j);
		}
	}
	return m;
}

inline vec3 eig_to_glm( Eigen::Vector3d v)
{
	vec3 v3 = vec3(0.0);
	v3[0] = v(0);
	v3[1] = v(1);
	v3[2] = v(2);
	return v3;
}

#endif // _MATLIB
