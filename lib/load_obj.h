#pragma once

#include "lib/OBJ_Loader.h"
#include "DxLib.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace DxLib;
using namespace std;

int btnFlg = 0;
double px, py;
double t = 0.0; double t0 = 0.0;
double s = 0.0; double s0 = 0.0;
double r = 2.5;
string filename = "kuriyama_v4";
int inc = 1;

int load_obj(float pos[21000][3], float normal[21000][3], int tri_id[11000][3], int* meshNum, int* vertexNum, int* triNum) {
	objl::Loader Loader;
	ostringstream s_input;
	ostringstream s_output;
	s_input << "obj_test/" << filename << ".obj";
	bool loadout = Loader.LoadFile(s_input.str());
	int k = 0; int l = 0;
	*meshNum = 0; *vertexNum = 0; *triNum = 0;

	if (loadout) {
		// Create/Open e1Out.txt
		s_output << "obj_test/e1Out.txt";
		ofstream file(s_output.str());

		// Go through each loaded mesh and out its contents
		/* */*meshNum = Loader.LoadedMeshes.size();
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";
			file << "Vertices:\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			/* */*vertexNum += curMesh.Vertices.size();
			for (int j = 0; j < curMesh.Vertices.size(); j++) {
				file << "V" << j << ": " <<
					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
				if (l < 21000) {
					pos[l][0] = -curMesh.Vertices[j].Position.Z * 10.0;
					pos[l][1] = -curMesh.Vertices[j].Position.Y * 10.0;
					pos[l][2] = curMesh.Vertices[j].Position.X * 10.0;
					normal[l][0] = -curMesh.Vertices[j].Normal.Z;
					normal[l][1] = -curMesh.Vertices[j].Normal.Y;
					normal[l][2] = curMesh.Vertices[j].Normal.X;
				}
				l++;
			}

			// Print Indices
			file << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3) {
				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
				if (k < 21000) {
					tri_id[k][0] = curMesh.Indices[j];
					tri_id[k][1] = curMesh.Indices[j + 1];
					tri_id[k][2] = curMesh.Indices[j + 2];
				}
				++k;
			}
			*triNum = k;

			// Print Material
			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";
			file << "\n";
		}
		file.close();
	}
	else {
		s_output << "obj_test/e1Out.txt";
		ofstream file(s_output.str());
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
		file.close();
	}

	return 0;
}

/*--Display func-------------------------------------------------------------------------*/
void show_obj(float pos[21000][3], float normal[21000][3], int tri_id[11000][3], int vertexNum, int triNum) {

	VERTEX3D *Vertex;
	Vertex = (VERTEX3D *)calloc(vertexNum, sizeof(VERTEX3D));
	unsigned short *index;
	index = (unsigned short *)calloc(3 * triNum, sizeof(unsigned short));
	VECTOR CenterPos = VGet(320.0, 240.0, 0.0);
	int red = 200; int green = 200; int blue = 50;

	for (int i = 0, n = min(vertexNum, 21000); i < n; i++) {
		Vertex[i].pos = VAdd(CenterPos, VGet(pos[i][0], pos[i][1], pos[i][2]));
		Vertex[i].norm = VGet(normal[i][0], normal[i][1], normal[i][2]);
		Vertex[i].dif = GetColorU8(red, green, blue, 255); 
		Vertex[i].spc = GetColorU8(0, 0, 0, 0);
		Vertex[i].u = 0.0; Vertex[i].v = 0.0; Vertex[i].su = 0.0; Vertex[i].sv = 0.0;
	}
	for (int i = 0,n = min(triNum, 11000); i < n; i++) {
		index[3 * i] = tri_id[i][0];
		index[3 * i + 1] = tri_id[i][1];
		index[3 * i + 2] = tri_id[i][2];
	}

	DrawPolygonIndexed3D(Vertex, vertexNum, index, triNum, DX_NONE_GRAPH, FALSE);
	if (Vertex != NULL)	free(Vertex);
	if (index != NULL)	free(index);
}

void show_obj(float pos[21000][3], float normal[21000][3], int tri_id[11000][3], int vertexNum, int triNum, 
	VECTOR center, double rad, double theta) {

	VERTEX3D *Vertex;
	Vertex = (VERTEX3D *)calloc(vertexNum, sizeof(VERTEX3D));
	unsigned short *index;
	index = (unsigned short *)calloc(3 * triNum, sizeof(unsigned short));
	int red = 200; int green = 200; int blue = 50;

	for (int i = 0, n = min(vertexNum, 21000); i < n; i++) {
		Vertex[i].pos = VAdd(center, VGet(
			rad * (pos[i][0] * cos(theta) - pos[i][2] * sin(theta)),
			rad * pos[i][1], 
			rad * (pos[i][0] * sin(theta) + pos[i][2] * cos(theta))));
		Vertex[i].norm = 
			VGet(normal[i][0] * cos(theta) - normal[i][2] * sin(theta),
				normal[i][1], 
				normal[i][0] * sin(theta) + normal[i][2] * cos(theta));
		Vertex[i].dif = GetColorU8(red, green, blue, 255);
		Vertex[i].spc = GetColorU8(0, 0, 0, 0);
		Vertex[i].u = 0.0; Vertex[i].v = 0.0; Vertex[i].su = 0.0; Vertex[i].sv = 0.0;
	}
	for (int i = 0, n = min(triNum, 11000); i < n; i++) {
		index[3 * i] = tri_id[i][0];
		index[3 * i + 1] = tri_id[i][1];
		index[3 * i + 2] = tri_id[i][2];
	}

	DrawPolygonIndexed3D(Vertex, vertexNum, index, triNum, DX_NONE_GRAPH, FALSE);
	if (Vertex != NULL)	free(Vertex);
	if (index != NULL)	free(index);
}
