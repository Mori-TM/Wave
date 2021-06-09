#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

WaveVec3 WaveAdd(WaveVec3 a, WaveVec3 b)
{
	WaveVec3 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

WaveVec3 WaveSub(WaveVec3 a, WaveVec3 b)
{
	WaveVec3 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

WaveVec3 WaveMul(WaveVec3 a, WaveVec3 b)
{
	WaveVec3 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;

	return r;
}

WaveVec3 WaveDiv(WaveVec3 a, WaveVec3 b)
{
	WaveVec3 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;

	return r;
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

WaveVec3* WaveGenUVs(unsigned int VerticesCount, WaveVec3* Vertices, WaveVec3* Normals)
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

WaveVec3* WaveGenNormals(unsigned int VerticesCount, WaveVec3* Vertices)
{
	WaveVec3* Normals = malloc(VerticesCount * sizeof(WaveVec3));

	for (unsigned int i = 0; i < VerticesCount; i += 3)
	{
		WaveVec3 N = WaveCross(WaveSub(Vertices[i + 1], Vertices[i]), WaveSub(Vertices[i + 2], Vertices[i]));

		Normals[i] = WaveAdd(Normals[i], N);
		Normals[i + 1] = WaveAdd(Normals[i + 1], N);
		Normals[i + 2] = WaveAdd(Normals[i + 2], N);

		Normals[i] = WaveNormalize(Normals[i]);
		Normals[i + 1] = WaveNormalize(Normals[i + 1]);
		Normals[i + 2] = WaveNormalize(Normals[i + 2]);
	}

	return Normals;
}

WaveModelData WaveLoadOBJ(const char* Path, enum WaveSettings Settings)
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

	int WaveHasMaterial = 1;
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

		if (strcmp(LineHeader, "mtllib") == 0 && (Settings & WAVE_LOAD_MATERIAL))
		{
			char Path[2048];
			fscanf(File, "%s\n", &Path);
			
			FILE* MatFile = fopen(Path, "r");
			if (!MatFile)
			{
				WaveHasMaterial = 0;
				goto NoMatterial;
			}

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

		if (strcmp(LineHeader, "usemtl") == 0 && Settings & WAVE_LOAD_MATERIAL)
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
	Data.TexCoordCount = VertexIndicesCount;
	Data.NormalCount = VertexIndicesCount;

	Data.Vertices = malloc(VertexIndicesCount * sizeof(WaveVec3));
	Data.TexCoords = malloc(VertexIndicesCount * sizeof(WaveVec3));
	Data.Normals = malloc(VertexIndicesCount * sizeof(WaveVec3));
	
	for (unsigned int i = 0; i < TempMaterialCount; i++)
	{
		for (int j = 0; j < MaterialCount+1; j++)
		{
			if (strcmp(TempMaterial[i].Name, Material[j].MaterialName) == 0)
			{
				WaveModelMaterial TempMat = Material[j];
				Material[j] = Material[i];
				Material[i] = TempMat;
			}
		}
	}

//	for (unsigned int i = 0; i < TempMaterialCount; i++)
//		printf("%s\n", TempMaterial[i].Name);
//
//	printf("\n\n");
//
//	for (unsigned int i = 0; i < MaterialCount+1; i++)
//		printf("%s\n", Material[i].MaterialName);

	WaveModelMaterial EmptyMaterial = 
	{ 
		"Default", 
		{ 1.0, 1.0, 1.0 }, 
		{ 1.0, 1.0, 1.0 }, 
		1.0, 
		{ 1.0, 1.0, 1.0 }, 
		"NoTexture", 
		"NoTexture", 
		"NoTexture", 
		"NoTexture", 
		"NoTexture", 
		"NoTexture" 
	};

	int Pos = 0;

	for (unsigned int i = 0; i < VertexIndicesCount; i++)
	{
		unsigned int VertexIndex = VertexIndices[i];
		Data.Vertices[i] = TempVertices[VertexIndex - 1];

		if (NormalIndicesCount != 0)
		{
			unsigned int NormalIndex = NormalIndices[i];
			Data.Normals[i] = TempNormals[NormalIndex - 1];
		}
		if (UvIndicesCount != 0)
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

		if (WaveHasMaterial && (Settings & WAVE_LOAD_MATERIAL))
		{
			if (TempMaterial[Pos].Pos == i)
				Pos++;

			Data.Material[i] = Material[Pos - 1];
		}
		else
			Data.Material[i] = EmptyMaterial;
	}

	if ((Settings & WAVE_GEN_NORMALS) && NormalIndicesCount == 0)
		Data.Normals = WaveGenNormals(VertexIndicesCount, Data.Vertices);

	if ((Settings & WAVE_GEN_UVS) && UvIndicesCount == 0)
		Data.TexCoords = WaveGenUVs(VertexIndicesCount, Data.Vertices, Data.Normals);

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

typedef struct
{
	unsigned char Header[80];
	unsigned int Triangles;
} WaveSTLDescription;

typedef struct WaveSTLDescription
{
	WaveVec3 Normal;
	WaveVec3 Vertex1;
	WaveVec3 Vertex2;
	WaveVec3 Vertex3;
	short int ByteCount;
} WaveSTLVertex;

WaveModelData WaveLoadSTL(const char* Path, enum WaveSettings Settings)
{
	FILE* File = fopen(Path, "rb");
	if (!File)
		return;

	WaveSTLVertex* VertexArray;
	WaveSTLDescription Description;

	fread(&Description, sizeof(Description), 1, File);
	VertexArray = malloc(Description.Triangles * sizeof(WaveSTLVertex));

	unsigned int VerticeCount = Description.Triangles * 3;

	WaveModelMaterial EmptyMaterial =
	{
		"Default",
		{ 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0 },
		1.0,
		{ 1.0, 1.0, 1.0 },
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture"
	};

	WaveModelData Data;
	Data.Material = malloc(VerticeCount * sizeof(WaveModelMaterial));
	Data.VerticeCount = VerticeCount;
	Data.TexCoordCount = VerticeCount;
	Data.NormalCount = VerticeCount;

	Data.Vertices = malloc(VerticeCount * sizeof(WaveVec3));
	Data.TexCoords = malloc(VerticeCount * sizeof(WaveVec3));
	Data.Normals = malloc(VerticeCount * sizeof(WaveVec3));

	unsigned int j = 0;
	for (unsigned int i = 0; i < Description.Triangles; i++)
	{
		fread(&VertexArray[i], 50, 1, File);

		Data.Vertices[j] = VertexArray[i].Vertex1;
		Data.Vertices[j + 1] = VertexArray[i].Vertex2;
		Data.Vertices[j + 2] = VertexArray[i].Vertex3;

		Data.Normals[j] = VertexArray[i].Normal;
		Data.Normals[j + 1] = VertexArray[i].Normal;
		Data.Normals[j + 2] = VertexArray[i].Normal;

		Data.Material[j] = EmptyMaterial;
		Data.Material[j + 1] = EmptyMaterial;
		Data.Material[j + 2] = EmptyMaterial;

		j += 3;
	}

	if (Settings & WAVE_GEN_UVS)
		Data.TexCoords = WaveGenUVs(VerticeCount, Data.Vertices, Data.Normals);

	fclose(File);
	free(VertexArray);

	return Data;
}

typedef struct
{
	char* Line;
	unsigned int Length;

	unsigned int LineCount;
} WaveLine;

WaveLine* WaveGetLines(char* Buffer, long BufferLength)
{
	WaveLine* FinalLines = malloc(1 * sizeof(WaveLine));
	
	unsigned int LineCount = 0;

	char* p = strtok(Buffer, "\n");
	while (p)
	{
		FinalLines[LineCount].Line = p;
		FinalLines[LineCount].Length = strlen(p);

		LineCount++;
		FinalLines = realloc(FinalLines, (LineCount + 1) * sizeof(WaveLine));

		p = strtok(NULL, "\n");
	}

	FinalLines->LineCount = LineCount;

	free(p);

	return FinalLines;
}

typedef struct
{
	float* FloatArray;
	unsigned int FloatArrayCount;

	unsigned int ArrayCount;
} WaveFloatArray;

typedef struct
{
	char* CharArray;
	unsigned int CharArrayCount;

	unsigned int ArrayCount;
} WaveCharArray;

WaveFloatArray GetFloatsFromString(char* Line, long Length)
{
	WaveFloatArray FinalArray;

	float* FloatArray = malloc(1 * sizeof(float));
	unsigned int FloatArrayCount = 0;

	char* p = strtok(Line, " ");
	while (p)
	{
		FloatArray[FloatArrayCount] = atof(p);
		FloatArrayCount++;
		FloatArray = realloc(FloatArray, (FloatArrayCount + 1) * sizeof(float));

		p = strtok(NULL, " ");
	}

	FinalArray.FloatArray = FloatArray;
	FinalArray.FloatArrayCount = FloatArrayCount;
	free(p);
	return FinalArray;
}

WaveFloatArray WaveGetFloatsFromLine(char* Line, long Length)
{
	char* FloatLine = malloc(1 * sizeof(char));
	unsigned int FloatLineCount = 0;

	WaveFloatArray FloatArray;

	for (unsigned int i = 0; i < Length; i++)
	{
		if (Line[i] == '>')
		{
			FloatLine = malloc(1 * sizeof(char));
			FloatLineCount = 0;

			while (1)
			{
				i++;

				if (Line[i] == '<')
				{
					FloatLine[FloatLineCount] = '\0';
					FloatArray = GetFloatsFromString(FloatLine, FloatLineCount);
					break;
				}

				FloatLine[FloatLineCount] = Line[i];
				FloatLineCount++;
				FloatLine = realloc(FloatLine, (FloatLineCount + 1) * sizeof(char));
			}

			free(FloatLine);
			return FloatArray;
		}
	}
}

typedef struct
{
	WaveFloatArray* FloatArray;
	unsigned int VertexPos;
	unsigned int NormalPos;
	unsigned int TexCoordPos;
	unsigned int ColorPos;
	unsigned int IndexPos;
} WaveMeshData;

WaveMeshData WaveGetMeshData(WaveLine* Lines)
{
	WaveMeshData MeshData;

	WaveFloatArray* FloatArray = malloc(1 * sizeof(WaveFloatArray));
	unsigned int FloatArrayCount = 0;

	WaveCharArray* VertexOrder = malloc(1 * sizeof(WaveCharArray));
	unsigned int VertexCount = 0;

	for (unsigned int i = 0; i < Lines->LineCount; i++)
	{
		char* FoundMeshNode = strstr(Lines[i].Line, "<mesh>");
		if (FoundMeshNode)
		{
			while (1)
			{
				char* FoundMeshNodeEnd = strstr(Lines[i].Line, "</mesh>");
				if (FoundMeshNodeEnd)
					break;

				char* FoundVertex = strstr(Lines[i].Line, "float_array");

				if (FoundVertex)
				{
					FloatArray[FloatArrayCount] = WaveGetFloatsFromLine(Lines[i].Line, Lines[i].Length);
					FloatArrayCount++;
					FloatArray = realloc(FloatArray, (FloatArrayCount + 1) * sizeof(WaveFloatArray));
				}

				char* FoundIndex = strstr(Lines[i].Line, "<p>");
			
				if (FoundIndex)
				{
					FloatArray[FloatArrayCount] = WaveGetFloatsFromLine(Lines[i].Line, Lines[i].Length);
					FloatArrayCount++;
					FloatArray = realloc(FloatArray, (FloatArrayCount + 1) * sizeof(WaveFloatArray));
				}

				char* FoundOffset = strstr(Lines[i].Line, "offset");

				if (FoundOffset)
				{
					VertexOrder[VertexCount].CharArray = Lines[i].Line;
					VertexOrder[VertexCount].CharArrayCount = strlen(Lines[i].Line);
					VertexCount++;
					VertexOrder = realloc(VertexOrder, (VertexCount + 1) * sizeof(WaveCharArray));
				}

				i++;
			}
		}
	}

	VertexOrder->ArrayCount = VertexCount;
	FloatArray->ArrayCount = FloatArrayCount;

	MeshData.FloatArray = FloatArray;

	MeshData.VertexPos = 256;
	MeshData.NormalPos = 256;
	MeshData.TexCoordPos = 256;
	MeshData.ColorPos = 256;

	for (unsigned int i = 0; i < VertexOrder->ArrayCount; i++)
	{
		char* FoundVertex = strstr(VertexOrder[i].CharArray, "VERTEX");

		if (FoundVertex)
			MeshData.VertexPos = i;

		char* FoundNormal = strstr(VertexOrder[i].CharArray, "NORMAL");

		if (FoundNormal)
			MeshData.NormalPos = i;

		char* FoundTexCoord = strstr(VertexOrder[i].CharArray, "TEXCOORD");

		if (FoundTexCoord)
			MeshData.TexCoordPos = i;

		char* FoundColor = strstr(VertexOrder[i].CharArray, "COLOR");

		if (FoundColor)
			MeshData.ColorPos = i;
	}

	return MeshData;
}

WaveModelData WaveLoadDAE(const char* Path, enum WaveSettings Settings)
{
	char* Buffer = 0;
	long Length = 0;

	FILE* File = fopen(Path, "r");
	if (!File)
		return;

	fseek(File, 0, SEEK_END);
	Length = ftell(File);
	fseek(File, 0, SEEK_SET);
	Buffer = (char*)malloc(Length + 1);
	fread(Buffer, sizeof(char), Length, File);

	WaveLine* Lines = WaveGetLines(Buffer, Length);
	WaveMeshData MeshData = WaveGetMeshData(Lines);

	WaveVec3* FinalVertices = malloc(1 * sizeof(WaveVec3));
	WaveVec3* FinalNormlas = malloc(1 * sizeof(WaveVec3));
	WaveVec3* FinalTexCoords = malloc(1 * sizeof(WaveVec3));
	WaveVec3* FinalColors = malloc(1 * sizeof(WaveVec3));

	unsigned int VerticesCount = 0;
	unsigned int NormalCount = 0;
	unsigned int TexCoordCount = 0;
	unsigned int ColorCount = 0;
	//Indices
	unsigned int* FinalVerticeIndices = malloc(1 * sizeof(unsigned int));
	unsigned int* FinalNormlaIndices = malloc(1 * sizeof(unsigned int));
	unsigned int* FinalTexCoordIndices = malloc(1 * sizeof(unsigned int));
	unsigned int* FinalColorIndices = malloc(1 * sizeof(unsigned int));

	unsigned int VerticesIndicesCount = 0;
	unsigned int NormalIndicesCount = 0;
	unsigned int TexCoordIndicesCount = 0;
	unsigned int ColorIndicesCount = 0;

	for (unsigned int i = 0; i < MeshData.FloatArray[MeshData.VertexPos].FloatArrayCount; i += 3)
	{
		FinalVertices[VerticesCount].x = MeshData.FloatArray[MeshData.VertexPos].FloatArray[i];
		FinalVertices[VerticesCount].y = MeshData.FloatArray[MeshData.VertexPos].FloatArray[i + 1];
		FinalVertices[VerticesCount].z = MeshData.FloatArray[MeshData.VertexPos].FloatArray[i + 2];
		VerticesCount++;
		FinalVertices = realloc(FinalVertices, (VerticesCount + 1) * sizeof(WaveVec3));
	}

	if (MeshData.NormalPos != 256)
		for (unsigned int i = 0; i < MeshData.FloatArray[MeshData.NormalPos].FloatArrayCount; i += 3)
		{
			FinalNormlas[NormalCount].x = MeshData.FloatArray[MeshData.NormalPos].FloatArray[i];
			FinalNormlas[NormalCount].y = MeshData.FloatArray[MeshData.NormalPos].FloatArray[i + 1];
			FinalNormlas[NormalCount].z = MeshData.FloatArray[MeshData.NormalPos].FloatArray[i + 2];
			NormalCount++;
			FinalNormlas = realloc(FinalNormlas, (NormalCount + 1) * sizeof(WaveVec3));
		}

	if (MeshData.TexCoordPos != 256)
		for (unsigned int i = 0; i < MeshData.FloatArray[MeshData.TexCoordPos].FloatArrayCount; i += 2)
		{
			FinalTexCoords[TexCoordCount].x = MeshData.FloatArray[MeshData.TexCoordPos].FloatArray[i];
			FinalTexCoords[TexCoordCount].y = MeshData.FloatArray[MeshData.TexCoordPos].FloatArray[i + 1];
			FinalTexCoords[TexCoordCount].z = 0.0;
			TexCoordCount++;
			FinalTexCoords = realloc(FinalTexCoords, (TexCoordCount + 1) * sizeof(WaveVec3));
		}

	if (MeshData.ColorPos != 256)
		for (unsigned int i = 0; i < MeshData.FloatArray[MeshData.ColorPos].FloatArrayCount; i += 3)
		{
			FinalColors[ColorCount].x = MeshData.FloatArray[MeshData.ColorPos].FloatArray[i];
			FinalColors[ColorCount].y = MeshData.FloatArray[MeshData.ColorPos].FloatArray[i + 1];
			FinalColors[ColorCount].z = MeshData.FloatArray[MeshData.ColorPos].FloatArray[i + 2];
			ColorCount++;
			FinalColors = realloc(FinalColors, (ColorCount + 1) * sizeof(WaveVec3));
		}

	for (unsigned int i = 0; i < MeshData.FloatArray[MeshData.FloatArray->ArrayCount-1].FloatArrayCount; i += MeshData.FloatArray->ArrayCount-1)
	{
		FinalVerticeIndices[VerticesIndicesCount] = MeshData.FloatArray[MeshData.FloatArray->ArrayCount - 1].FloatArray[i + MeshData.VertexPos];
		VerticesIndicesCount++;
		FinalVerticeIndices = realloc(FinalVerticeIndices, (VerticesIndicesCount + 1) * sizeof(unsigned int));

		if (NormalCount != 0)
		{
			FinalNormlaIndices[NormalIndicesCount] = MeshData.FloatArray[MeshData.FloatArray->ArrayCount - 1].FloatArray[i + MeshData.NormalPos];
			NormalIndicesCount++;
			FinalNormlaIndices = realloc(FinalNormlaIndices, (NormalIndicesCount + 1) * sizeof(unsigned int));
		}
			
		if (TexCoordCount != 0)
		{
			FinalTexCoordIndices[TexCoordIndicesCount] = MeshData.FloatArray[MeshData.FloatArray->ArrayCount - 1].FloatArray[i + MeshData.TexCoordPos];
			TexCoordIndicesCount++;
			FinalTexCoordIndices = realloc(FinalTexCoordIndices, (TexCoordIndicesCount + 1) * sizeof(unsigned int));
		}
			
		if (ColorCount != 0)
		{
			FinalColorIndices[ColorIndicesCount] = MeshData.FloatArray[MeshData.FloatArray->ArrayCount - 1].FloatArray[i + MeshData.ColorPos];
			ColorIndicesCount++;
			FinalColorIndices = realloc(FinalColorIndices, (ColorIndicesCount + 1) * sizeof(unsigned int));
		}
	}

	WaveModelMaterial EmptyMaterial =
	{
		"Default",
		{ 1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0 },
		1.0,
		{ 1.0, 1.0, 1.0 },
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture",
		"NoTexture"
	};

	WaveModelData Data;
	Data.Material = malloc(VerticesIndicesCount * sizeof(WaveModelMaterial));
	Data.VerticeCount = VerticesIndicesCount;
	Data.TexCoordCount = VerticesIndicesCount;
	Data.NormalCount = VerticesIndicesCount;

	Data.Vertices = malloc(VerticesIndicesCount * sizeof(WaveVec3));
	Data.TexCoords = malloc(VerticesIndicesCount * sizeof(WaveVec3));
	Data.Normals = malloc(VerticesIndicesCount * sizeof(WaveVec3));

	for (unsigned int i = 0; i < VerticesIndicesCount; i++)
	{
		Data.Vertices[i] = FinalVertices[FinalVerticeIndices[i]];
		
		if (NormalIndicesCount != 0)
			Data.Normals[i] = FinalNormlas[FinalNormlaIndices[i]];
			
		if (TexCoordIndicesCount != 0)
		{
			if (Settings & WAVE_FLIP_UVS)
			{
				Data.TexCoords[i].x = FinalTexCoords[FinalTexCoordIndices[i]].x;
				Data.TexCoords[i].y = -FinalTexCoords[FinalTexCoordIndices[i]].y;
			}
			else
				Data.TexCoords[i] = FinalTexCoords[FinalTexCoordIndices[i]];
		}
			

		if (ColorIndicesCount != 0)
		{
			Data.Material[i] = EmptyMaterial;
			Data.Material[i].DiffuseColor = FinalColors[FinalColorIndices[i]];
		}
		else
			Data.Material[i] = EmptyMaterial;
	}
//	Data.Normals = WaveGenSmoothNormals(VerticesIndicesCount, Data.Vertices);
	if ((Settings & WAVE_GEN_NORMALS) && NormalIndicesCount == 0)
		Data.Normals = WaveGenNormals(VerticesIndicesCount, Data.Vertices);

	if ((Settings & WAVE_GEN_UVS) && TexCoordIndicesCount == 0)
		Data.TexCoords = WaveGenUVs(VerticesIndicesCount, Data.Vertices, Data.Normals);

	free(Buffer);
	free(Lines);
	free(FinalVertices);
	free(FinalNormlas);
	free(FinalTexCoords);
	free(FinalColors);

	free(FinalVerticeIndices);
	free(FinalNormlaIndices);
	free(FinalTexCoordIndices);
	free(FinalColorIndices);
	fclose(File);

	return Data;
}

void WaveFreeModel(WaveModelData* ModelData)
{
	free(ModelData->Material);
	free(ModelData->Vertices);
	free(ModelData->Normals);
	free(ModelData->TexCoords);
}
