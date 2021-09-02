#pragma once

#include <Eigen/Dense>
#include <glm/matrix.hpp>
#include <assimp/matrix4x4.h>

namespace motion_animation {

namespace util {

template<typename T, int m, int n>
inline glm::mat<m, n, float, glm::precision::highp> MatrixEigen2GLM(const Eigen::Matrix<T, m, n>& em)
{
	glm::mat<m, n, float, glm::precision::highp> mat;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			mat[j][i] = em(i, j);
		}
	}
	return mat;
}

template<typename T, int m>
inline glm::vec<m, float, glm::precision::highp> VectorEigen2GLM(const Eigen::Matrix<T, m, 1>& em)
{
	glm::vec<m, float, glm::precision::highp> v;
	for (int i = 0; i < m; ++i)
	{
		v[i] = em(i);
	}
	return v;
}

template<typename T>
inline Eigen::Matrix<T, 4, 4> MatrixAssimp2Eigen(const aiMatrix4x4t<T>& assimp_matrix) {
	Eigen::Matrix<T, 4, 4> eigen_matrix;
	eigen_matrix << assimp_matrix.a1, assimp_matrix.a2, assimp_matrix.a3, assimp_matrix.a4,
		assimp_matrix.b1, assimp_matrix.b2, assimp_matrix.b3, assimp_matrix.b4,
		assimp_matrix.c1, assimp_matrix.c2, assimp_matrix.c3, assimp_matrix.c4,
		assimp_matrix.d1, assimp_matrix.d2, assimp_matrix.d3, assimp_matrix.d4;

	return eigen_matrix;
}

}  // namespace util

}  // namespace motion_animation