#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}

//This will make the triang A->B->C
void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}

void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
	{
		a_nSubdivisions = 3;
	}
	if (a_nSubdivisions > 360)
	{
		a_nSubdivisions = 360;
	}
		

	Release();
	Init();

	float fValue = 0.5f;
	//Your code starts here
	//3--2
	//|  |
	//0--1
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float interval = 2 * PI / a_nSubdivisions;
		float theta = interval * i;
		//circle
		AddTri(
			fValue*vector3(0.0f, 0.0f, 0.0f), 
			fValue*a_fRadius*vector3(cos(theta), sin(theta), 0.0f),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), 0.0f)
		);
		//cone
		AddTri(
			fValue*a_fRadius*vector3(cos(theta), sin(theta), 0.0f),
			fValue*vector3(0.0f, 0.0f, -a_fHeight),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), 0.0f)
		);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
	{
		a_nSubdivisions = 3;
	}
	if (a_nSubdivisions > 360)
	{
		a_nSubdivisions = 360;
	}
		

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float interval = 2 * PI / a_nSubdivisions;
		float theta = interval * i;
		//top circle
		AddTri(
			fValue*vector3(0.0f, 0.0f, a_fHeight),
			fValue*a_fRadius*vector3(cos(theta), sin(theta), a_fHeight),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), a_fHeight)
		);
		//bottom circle
		AddTri(
			fValue*vector3(0.0f, 0.0f, -a_fHeight),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), -a_fHeight),
			fValue*a_fRadius*vector3(cos(theta), sin(theta), -a_fHeight)
		);
		//middle
		AddQuad(
			fValue*a_fRadius*vector3(cos(theta), sin(theta), -a_fHeight),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), -a_fHeight),
			fValue*a_fRadius*vector3(cos(theta), sin(theta), a_fHeight),
			fValue*a_fRadius*vector3(cos(theta + interval), sin(theta + interval), a_fHeight)
		);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
	{
		a_nSubdivisions = 3;
	}
	if (a_nSubdivisions > 360)
	{
		a_nSubdivisions = 360;
	}
		

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float interval = 2 * PI / a_nSubdivisions;
		float theta = interval * i;
		//top
		AddQuad(
			fValue*vector3(a_fInnerRadius*cos(theta), a_fInnerRadius*sin(theta), -a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta + interval), a_fInnerRadius*sin(theta + interval), -a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta), a_fOuterRadius*sin(theta), -a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta + interval), a_fOuterRadius*sin(theta + interval), -a_fHeight)
		);
		//bottom
		AddQuad(
			fValue*vector3(a_fOuterRadius*cos(theta), a_fOuterRadius*sin(theta), a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta + interval), a_fOuterRadius*sin(theta + interval), a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta), a_fInnerRadius*sin(theta), a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta + interval), a_fInnerRadius*sin(theta + interval), a_fHeight)
		);
		//outside
		AddQuad(
			fValue*vector3(a_fOuterRadius*cos(theta), a_fOuterRadius*sin(theta), -a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta + interval), a_fOuterRadius*sin(theta + interval), -a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta), a_fOuterRadius*sin(theta), a_fHeight),
			fValue*vector3(a_fOuterRadius*cos(theta + interval), a_fOuterRadius*sin(theta + interval), a_fHeight)
		);
		//inside
		AddQuad(
			fValue*vector3(a_fInnerRadius*cos(theta + interval), a_fInnerRadius*sin(theta + interval), -a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta), a_fInnerRadius*sin(theta), -a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta + interval), a_fInnerRadius*sin(theta + interval), a_fHeight),
			fValue*vector3(a_fInnerRadius*cos(theta), a_fInnerRadius*sin(theta), a_fHeight)
		);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 30)
	{
		a_nSubdivisions = 30;
	}
		

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	
	//middle
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			float interval = 2 * PI / a_nSubdivisions;
			float theta = interval * i;
			float phi = interval * j;
			
			//follow the same pattern as a cylinder, but for two angles and using the cartesian system for spherical coordinates
			AddQuad(
				fValue*a_fRadius*vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)),
				fValue*a_fRadius*vector3(sin(theta + interval)*cos(phi), sin(theta + interval)*sin(phi), cos(theta + interval)),
				fValue*a_fRadius*vector3(sin(theta)*cos(phi + interval), sin(theta)*sin(phi + interval), cos(theta)),
				fValue*a_fRadius*vector3(sin(theta + interval)*cos(phi + interval), sin(theta + interval)*sin(phi + interval), cos(theta + interval))
			);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);
}