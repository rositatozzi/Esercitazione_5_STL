#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace MeshLibrary {
bool importCell0Ds(const string& fileName, PolygonalMesh& mesh);//AGGIUSTA

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh, double tol1D);

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh, double tol2D);

bool importMesh(const string& path, PolygonalMesh& mesh, double tol1D, double tol2D);

void testLengthEdges(PolygonalMesh& mesh,  double tol1D);
void testAreaPolygonal(PolygonalMesh& mesh, double tol2D);
}

double crossProduct(Vector2d& v1, Vector2d& v2 );
double setTol1D();
double setTol2D(const double tol1D);
