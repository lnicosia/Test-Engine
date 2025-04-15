#include "MatrixTest.hpp"
#include "../Matrix.hpp"

using namespace sml;
void TestMatrixInit()
{
	float cStyle1x1[1][1]{ {0.0f} };
	float cStyle1x2[1][2] = { {0.0f, 0.0f} };
	float cStyle2x1[2][1] = { {0.0f}, {0.0f} };
	float cStyle2x2[2][2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

	// Vectors

	vec3 v1;
	printf("vec3 v1;\n");
	v1.print();
	vec3 v2 = vec3();
	printf("\nvec3 v2 = vec3();\n");
	v2.print();
	vec3 v3{};
	printf("\nvec3 v3{};\n");
	v3.print();
	vec3 v4(1.0f);
	printf("\nvec3 v4(1.0f);\n");
	v4.print();
	vec3 v5{1.0f};
	printf("\nvec3 v5{1.0f};\n");
	v5.print();
	vec3 v6 = vec3{1.0f};
	printf("\nvec3 v6 = vec3{1.0f};\n");
	v6.print();
	vec3 v7(1.0f, 2.0f, 3.0f);
	printf("\nvec3 v7(1.0f, 2.0f, 3.0f);\n");
	v7.print();
	vec3 v8{1.0f, 2.0f, 3.0f};
	printf("\nvec3 v7{1.0f, 2.0f, 3.0f};\n");
	v8.print();
	printf("\n");

	printf("Utilities:\n");
	vec3 v9(2.0f, 2.0f, 2.0f);
	printf("vec3 v9(2.0f, 2.0f, 2.0f);\n");
	v9.print();
	printf("\nv9.length() = %f\n", v9.length());
	printf("\nv10 = v9.normalized() =\n");
	vec3 v10 = v9.normalized();
	v10.print();
	printf("\nv11 = v9.fnormalized() =\n");
	vec3 v11 = v9.fnormalized();
	v11.print();
	printf("\nv9 =\n");
	v9.print();
	printf("\nv9.normalize() =\n");
	v9.normalize();
	v9.print();
	v9 = { 2.0f, 2.0f, 2.0f};
	printf("\nReset v9: ");
	v9.print();
	printf("\nv9.fnormalize() =\n");
	v9.fnormalize();
	v9.print();
	printf("\nnormalized(vec3{2.0f, 2.0f, 2.0f}) = ");
	vec3::normalized(vec3{2.0f, 2.0f, 2.0f}).print();
	printf("\nfnormalized(vec3{2.0f, 2.0f, 2.0f}) = ");
	vec3::fnormalized(vec3{2.0f, 2.0f, 2.0f}).print();

	vec4 v12{-0.5f, -0.5f, 0.0f, 1.0f};
	printf("\nvec4 v12{-0.5f, -0.5f, 0.0f, 1.0f};\n");
	v12.print();
	printf("\n");

	
	// Matrices
	const Matrix<float, 1, 1> testMat1x1{ {0.0f} };
	const Matrix<float, 1, 2> testMat1x2{ {1.0f, 2.0f} };
	const Matrix<float, 2, 1> testMat2x1{ {1.0f}, {2.0f} };
	const Matrix<float, 2, 2> testMat2x2{ {1.0f, 2.0f}, {3.0f, 4.0f} };
	const Matrix<float, 2, 3> testMat2x3{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };
	const Matrix<float, 3, 2> testMat3x2{ {1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f} };
	const Matrix<float, 3, 3> testMat3x3{	{1.0f, 2.0f, 3.0f},
												{4.0f, 5.0f, 6.0f},
												{7.0f, 8.0f, 9.0f}};
	const mat3 testMat3{	{1.0f, 2.0f, 3.0f},
							{4.0f, 5.0f, 6.0f},
							{7.0f, 8.0f, 9.0f}};
	const vec3 vm({1.0f, 2.0f, 3.0f});
	const mat3 testMatFromVec{vm, vm, vm};
	printf("Construct matrix from vectors:\n");
	printf("const vec3 vm({1.0f, 2.0f, 3.0f});\nconst mat3 testMatFromVec{vm, vm, vm};\nRes:\n");
	testMatFromVec.print();
	const mat3 testMatFromVec2(vm, vm, vm);
	printf("const mat3 testMatFromVec(vm, vm, vm);\nRes:\n");
	testMatFromVec2.print();

	const mat3 id(1.0f);
	printf("\nIdendity matrix init:\n");
	id.print();
	printf("\n");
	const mat4 testMat4x4{	{1.0f,  2.0f,  3.0f,  4.0f},
							{5.0f,  6.0f,  7.0f,  8.0f},
							{9.0f,  10.0f, 11.0f, 12.0f},
							{13.0f, 14.0f, 15.0f, 16.0f}};
	const mat4 testMat4x4_2{vec4{1.0f,  2.0f,  3.0f,  4.0f},
							vec4{5.0f,  6.0f,  7.0f,  8.0f},
							vec4{9.0f,  10.0f, 11.0f, 12.0f},
							vec4{13.0f, 14.0f, 15.0f, 16.0f}};
	testMat4x4_2.print();
	printf("\n");
}

void TestMatrixMult()
{
	Matrix<float, 2, 2> defaultMat;
	printf("Default mat:\n");
	defaultMat.print();
	Matrix<float, 2, 3> m1 = { {1.0f, 2.0f, 0.0f}, {4.0f, 3.0f, -1.0f} };
	printf("m1 =\n");
	m1.print();
	Matrix<float, 3, 2> m2 = { {5.0f, 1.0f}, {2.0f, 3.0f}, {3.0f, 4.0f} };
	printf("m2 =\n");
	m2.print();
	auto res1 = m1 * m2;
	printf("m1 x m2 =\n");
	res1.print();
	auto res2 = m2 * m1;
	printf("m2 x m1 =\n");
	res2.print();

	mat2 m3{ {1.0f, 0.0f}, {2.0f, -1.0f}};
	printf("m3 =\n");
	m3.print();
	mat2 m4{ {3.0f, 4.0f}, {-2.0f, -3.0f}};
	printf("m1 =\n");
	m4.print();
	auto res3 = m3 * m4;
	printf("m3 x m4 =\n");
	res3.print();
}

void TestMatrixOperations()
{
	printf("Matrix operations:\n");
	mat4 m;
	m = identity<float, 4>();
	printf("Identity matrix:\n");
	m.print();
	m *= translate(vec3{1.0f, 0.0f, 0.0f});
	printf("Translated by {1, 0, 0}:\n");
	m.print();
	m = identity<float, 4>(0.5f);
	printf("New m =\n");
	m.print();
	m *= scale(vec3(2.0f));
	printf("Scaled by 2:\n");
	m.print();
}