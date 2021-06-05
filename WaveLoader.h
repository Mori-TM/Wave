#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WAVEMAX(x, y) (((x) > (y)) ? (x) : (y))
#define WAVEMIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum
{
	WAVE_GEN_UVS = 1,
	WAVE_GEN_NORMALS = 2,
	WAVE_FLIP_UVS = 4,
	WAVE_LOAD_MATERIAL = 8
} WaveSettings;

typedef struct
{
	float x;
	float y;
	float z;
} WaveVec3;

float WaveFastInvertSqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;              // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);   // what the fuck? 
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed - it's for more accuracy

	return y;
}

WaveVec3 WaveNormalize(WaveVec3 v)
{
	float length = WaveFastInvertSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x *= length;
	v.y *= length;
	v.z *= length;

	return v;
}

WaveVec3 WaveCross(WaveVec3 a, WaveVec3 b)
{
	WaveVec3 r;
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

	return r;
}

float WaveDot(WaveVec3 a, WaveVec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

typedef struct
{
	char MaterialName[2048];

	WaveVec3 AmbientColor;
	WaveVec3 DiffuseColor;
	unsigned int SpecularExponent;
	WaveVec3 SpecularColor;
	char AmbientTexture[4096];
	char DiffuseTexture[4096];
	char SpecularTexture[4096];
	char AlphaTexture[4096];
	char NormalTexture[4096];
	char HeightTexture[4096];
} WaveModelMaterial;

typedef struct
{
	WaveModelMaterial* Material;

	unsigned int VerticeCount;
	WaveVec3* Vertices;

	unsigned int TexCoordCount;
	WaveVec3* TexCoords;

	unsigned int NormalCount;
	WaveVec3* Normals;
} WaveModelData;

typedef struct
{
	char Name[2048];
	unsigned int Pos;
} TempMaterialDescription;

WaveVec3* WaveGenUVs(int VerticesCount, WaveVec3* Vertices, WaveVec3* Normals)
{
	WaveVec3* TexCoord = malloc(VerticesCount * sizeof(WaveVec3));

	float MinX = 1000000000.0;
	float MaxX = -1000000000.0;

	float MinY = 1000000000.0;
	float MaxY = -1000000000.0;
	for (unsigned int i = 0; i < VerticesCount; i++)
	{
	//		float TempUVsqrt = sqrtf(Vertices[i].x * Vertices[i].x + Vertices[i].y * Vertices[i].y + Vertices[i].z * Vertices[i].z);
	//		TexCoord[i].x = sin(cos(Vertices[i].x / TempUVsqrt));
	//		TexCoord[i].y = sin(sin(Vertices[i].y / TempUVsqrt));

	
	//	MinX = WAVEMIN(MinX, Vertices[i].x);
	//	MinY = WAVEMIN(MinY, Vertices[i].y);
	//
	//	MaxX = WAVEMAX(MaxX, Vertices[i].x);
	//	MaxY = WAVEMAX(MaxY, Vertices[i].y);
	//
	//	float KX = 1 / (MaxX - MinX);
	//	float KY = 1 / (MaxY - MinY);
	//
	//	TexCoord[i].x = (Vertices[i].x - MinX) * KX;
	//	TexCoord[i].y = (Vertices[i].y - MinY) * KY;
	
		
		float DotProduct = 2 * WaveDot(Normals[i], Vertices[i]);
		float X = Vertices[i].x - Normals[i].x * DotProduct;
		float Y = Vertices[i].y - Normals[i].y * DotProduct;
		float Z = Vertices[i].z - Normals[i].z * DotProduct;
		WaveVec3 R = { X, Y, Z };

		if ((R.x >= R.y) && (R.x >= R.z))
		{
			float UC = -R.z;
			float VC = -R.y;
			float M = fabsf(R.x);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}

		if ((R.x <= R.y) && (R.x <= R.z))
		{
			float UC = +R.z;
			float VC = -R.y;
			float M = fabsf(R.x);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}

		if ((R.y >= R.z) && (R.y >= R.x))
		{
			float UC = +R.x;
			float VC = +R.z;
			float M = fabsf(R.y);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}

		if ((R.y <= R.z) && (R.z <= R.x))
		{
			float UC = +R.x;
			float VC = -R.z;
			float M = fabsf(R.y);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}

		if ((R.z >= R.y) && (R.z >= R.x))
		{
			float UC = +R.x;
			float VC = -R.y;
			float M = fabsf(R.z);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}

		if ((R.z <= R.y) && (R.z <= R.x))
		{
			float UC = -R.x;
			float VC = -R.y;
			float M = fabsf(R.z);

			TexCoord[i].x = ((UC / M) + 1) / 2;
			TexCoord[i].y = ((VC / M) + 1) / 2;
		}
		
	}

	return TexCoord;
}

WaveVec3* WaveGenNormals(int VerticesCount, WaveVec3* Vertices)
{
	WaveVec3* Normals = malloc(VerticesCount * sizeof(WaveVec3));

	for (unsigned int i = 0; i < VerticesCount; i += 3)
	{
		WaveVec3 U;
		U.x = Vertices[i + 1].x - Vertices[i + 0].x;
		U.y = Vertices[i + 1].y - Vertices[i + 0].y;
		U.z = Vertices[i + 1].z - Vertices[i + 0].z;

		WaveVec3 V;
		V.x = Vertices[i + 2].x - Vertices[i + 0].x;
		V.y = Vertices[i + 2].y - Vertices[i + 0].y;
		V.z = Vertices[i + 2].z - Vertices[i + 0].z;
		
		Normals[i+0] = WaveNormalize(WaveCross(U, V));
		Normals[i+1] = WaveNormalize(WaveCross(U, V));
		Normals[i+2] = WaveNormalize(WaveCross(U, V));
	}

	return Normals;
}

WaveModelData LoadOBJ(const char* Path, enum WaveSettings Settings)
{
	FILE* File = fopen(Path, "r");
	if (!File)
		return;

	unsigned int TempVerticeCount = 0;
	unsigned int TempUVCount = 0;
	unsigned int TempNormalCount = 0;
	WaveVec3* TempVertices = malloc(1 * sizeof(WaveVec3));
	WaveVec3* TempUVs = malloc(1 * sizeof(WaveVec3));
	WaveVec3* TempNormals = malloc(1 * sizeof(WaveVec3));

	unsigned int VertexIndicesCount = 0;
	unsigned int UvIndicesCount = 0;
	unsigned int NormalIndicesCount = 0;
	unsigned int* VertexIndices = malloc(1 * sizeof(unsigned int));
	unsigned int* UvIndices = malloc(1 * sizeof(unsigned int));
	unsigned int* NormalIndices = malloc(1 * sizeof(unsigned int));

	int MaterialCount = -1;
	WaveModelMaterial* Material = malloc(1 * sizeof(WaveModelMaterial));
	
	int TempMaterialCount = 0;
	TempMaterialDescription* TempMaterial = malloc(1 * sizeof(TempMaterialDescription));

	while (1)
	{
		char LineHeader[128];

		int Res = fscanf(File, "%s", LineHeader);
		if (Res == EOF)
			break;

		if (strcmp(LineHeader, "mtllib") == 0 && Settings & WAVE_LOAD_MATERIAL)
		{
			char Path[2048];
			fscanf(File, "%s\n", &Path);
			
			FILE* MatFile = fopen(Path, "r");
			if (!MatFile)
				goto NoMatterial;

			while (1)
			{
				char MatLineHeader[128];
	
				int MatRes = fscanf(MatFile, "%s", MatLineHeader);
				if (MatRes == EOF)
					break;
	
				
				if (strcmp(MatLineHeader, "Ka") == 0)
					fscanf(MatFile, "%f %f %f\n", &Material[MaterialCount].AmbientColor.x, &Material[MaterialCount].AmbientColor.y, &Material[MaterialCount].AmbientColor.z);
				
				if (strcmp(MatLineHeader, "Kd") == 0)
					fscanf(MatFile, "%f %f %f\n", &Material[MaterialCount].DiffuseColor.x, &Material[MaterialCount].DiffuseColor.y, &Material[MaterialCount].DiffuseColor.z);
	
				if (strcmp(MatLineHeader, "Ks") == 0)
					fscanf(MatFile, "%f %f %f\n", &Material[MaterialCount].SpecularColor.x, &Material[MaterialCount].SpecularColor.y, &Material[MaterialCount].SpecularColor.z);
	
				if (strcmp(MatLineHeader, "Ns") == 0)
					fscanf(MatFile, "%f\n", &Material[MaterialCount].SpecularExponent);
	
				if (strcmp(MatLineHeader, "map_Ka") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].AmbientTexture);
	
				if (strcmp(MatLineHeader, "map_Kd") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].DiffuseTexture);
	
				if (strcmp(MatLineHeader, "map_Ks") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].SpecularTexture);
	
				if (strcmp(MatLineHeader, "map_d") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].AlphaTexture);
	
				if (strcmp(MatLineHeader, "map_bump") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].NormalTexture);
	
				if (strcmp(MatLineHeader, "disp") == 0)
					fscanf(MatFile, "%s\n", &Material[MaterialCount].HeightTexture);

				if (strcmp(MatLineHeader, "newmtl") == 0)
				{
					MaterialCount++;
					fscanf(MatFile, "%s\n", &Material[MaterialCount].MaterialName);
					
					Material = realloc(Material, (MaterialCount + 2) * sizeof(WaveModelMaterial));

				}
			}
	
			fclose(MatFile);
		}
		NoMatterial:

		if (strcmp(LineHeader, "usemtl") == 0)
		{
			fscanf(File, "%s\n", &TempMaterial[TempMaterialCount].Name);
			TempMaterial[TempMaterialCount].Pos = VertexIndicesCount;
			TempMaterialCount++;
			TempMaterial = realloc(TempMaterial, (TempMaterialCount + 1) * sizeof(TempMaterialDescription));
		}

		if (strcmp(LineHeader, "v") == 0)
		{
			fscanf(File, "%f %f %f\n", &TempVertices[TempVerticeCount].x, &TempVertices[TempVerticeCount].y, &TempVertices[TempVerticeCount].z);
			TempVerticeCount++;
			TempVertices = realloc(TempVertices, (TempVerticeCount + 1) * sizeof(WaveVec3));
		}

		if (strcmp(LineHeader, "vt") == 0)
		{
			fscanf(File, "%f %f\n", &TempUVs[TempUVCount].x, &TempUVs[TempUVCount].y);
			TempUVCount++;
			TempUVs = realloc(TempUVs, (TempUVCount + 1) * sizeof(WaveVec3));
		}

		if (strcmp(LineHeader, "vn") == 0)
		{
			fscanf(File, "%f %f %f\n", &TempNormals[TempNormalCount].x, &TempNormals[TempNormalCount].y, &TempNormals[TempNormalCount].z);
			TempNormalCount++;
			TempNormals = realloc(TempNormals, (TempNormalCount + 1) * sizeof(WaveVec3));
		}

		if (strcmp(LineHeader, "f") == 0)
		{
			unsigned int VertexIndex[3], UvIndex[3], NormalIndex[3];
			
			int Matches;
			if (TempVerticeCount > 0 && TempUVCount > 0 && TempNormalCount > 0)
				Matches = fscanf(File, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &VertexIndex[0], &UvIndex[0], &NormalIndex[0], &VertexIndex[1], &UvIndex[1], &NormalIndex[1], &VertexIndex[2], &UvIndex[2], &NormalIndex[2]);

			else if (TempNormalCount == 0 && TempUVCount > 0)
				Matches = fscanf(File, "%d/%d %d/%d %d/%d\n", &VertexIndex[0], &UvIndex[0], &VertexIndex[1], &UvIndex[1], &VertexIndex[2], &UvIndex[2]);

			else if (TempUVCount == 0 && TempNormalCount > 0)
				Matches = fscanf(File, "%d//%d %d//%d %d//%d\n", &VertexIndex[0], &NormalIndex[0], &VertexIndex[1], &NormalIndex[1], &VertexIndex[2], &NormalIndex[2]);

			else
				Matches = fscanf(File, "%d %d %d\n", &VertexIndex[0], &VertexIndex[1], &VertexIndex[2]);

			VertexIndices = realloc(VertexIndices, (VertexIndicesCount + 3) * sizeof(unsigned int));
			UvIndices = realloc(UvIndices, (UvIndicesCount + 3) * sizeof(unsigned int));
			NormalIndices = realloc(NormalIndices, (NormalIndicesCount + 3) * sizeof(unsigned int));

			VertexIndices[VertexIndicesCount] = VertexIndex[0];
			VertexIndices[VertexIndicesCount + 1] = VertexIndex[1];
			VertexIndices[VertexIndicesCount + 2] = VertexIndex[2];
			VertexIndicesCount += 3;

			if (TempUVCount > 0)
			{
				UvIndices[UvIndicesCount] = UvIndex[0];
				UvIndices[UvIndicesCount + 1] = UvIndex[1];
				UvIndices[UvIndicesCount + 2] = UvIndex[2];
				UvIndicesCount += 3;
			}
			if (TempNormalCount > 0)
			{
				NormalIndices[NormalIndicesCount] = NormalIndex[0];
				NormalIndices[NormalIndicesCount + 1] = NormalIndex[1];
				NormalIndices[NormalIndicesCount + 2] = NormalIndex[2];
				NormalIndicesCount += 3;
			}			
		}
	}

	fclose(File);

	WaveModelData Data;
	Data.Material = malloc(VertexIndicesCount * sizeof(WaveModelMaterial));
	Data.VerticeCount = VertexIndicesCount;
	Data.TexCoordCount = UvIndicesCount;
	Data.NormalCount = NormalIndicesCount;

	Data.Vertices = malloc(VertexIndicesCount * sizeof(WaveVec3));

	if (UvIndicesCount == 0)
		Data.TexCoords = malloc(VertexIndicesCount * sizeof(WaveVec3));
	else
		Data.TexCoords = malloc(UvIndicesCount * sizeof(WaveVec3));

	if (NormalIndicesCount == 0)
		Data.Normals = malloc(VertexIndicesCount * sizeof(WaveVec3));
	else
		Data.Normals = malloc(NormalIndicesCount * sizeof(WaveVec3));

	
	for (unsigned int i = 0; i < TempMaterialCount; i++)
	{
		for (int j = 0; j < MaterialCount; j++)
		{
			if (strcmp(TempMaterial[i].Name, Material[j].MaterialName) == 0)
			{
				WaveModelMaterial TempMat = Material[j];
				Material[j] = Material[i];
				Material[i] = TempMat;
			}
		}
	}

	int Pos = 0;

	for (unsigned int i = 0; i < VertexIndicesCount; i++)
	{
		unsigned int VertexIndex = VertexIndices[i];
		Data.Vertices[i] = TempVertices[VertexIndex - 1];

		if (TempMaterial[Pos].Pos == i)
			Pos++;
		
		Data.Material[i] = Material[Pos - 1];
	}

	if ((Settings & WAVE_GEN_NORMALS) && NormalIndicesCount == 0)
		Data.Normals = WaveGenNormals(VertexIndicesCount, Data.Vertices);

	for (unsigned int i = 0; i < NormalIndicesCount; i++)
	{
		unsigned int NormalIndex = NormalIndices[i];
		Data.Normals[i] = TempNormals[NormalIndex - 1];
	}

	if ((Settings & WAVE_GEN_UVS) && UvIndicesCount == 0)
		Data.TexCoords = WaveGenUVs(VertexIndicesCount, Data.Vertices, Data.Normals);

	for (unsigned int i = 0; i < UvIndicesCount; i++)
	{
		unsigned int UvIndex = UvIndices[i];
		if (Settings & WAVE_FLIP_UVS)
		{
			Data.TexCoords[i].x = TempUVs[UvIndex - 1].x;
			Data.TexCoords[i].y = -TempUVs[UvIndex - 1].y;
		}
		else
			Data.TexCoords[i] = TempUVs[UvIndex - 1];
	}

	free(Material);
	free(TempMaterial);

	free(TempVertices);
	free(TempUVs);
	free(TempNormals);

	free(VertexIndices);
	free(UvIndices);
	free(NormalIndices);

	return Data;
}

void WaveFreeModel(WaveModelData* ModelData)
{
	free(ModelData->Material);
	free(ModelData->Vertices);
	free(ModelData->Normals);
	free(ModelData->TexCoords);
}