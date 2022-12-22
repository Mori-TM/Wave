# Wave
This are Self written Single Header files written in C with the Name Wave.

WaveMath is a basic C Math Library.
```C
mat4 Model;
Model = LoadMat4Identity();
Model = ScaleMat4(Model, 1.0, 0.0, 0.0);
Model = RotateYMat4(Model, Radians(90.0));
Model = TranslateMat4(Model, 1.0, 0.0, 0.0);

mat4 Proj;
float Aspect = (float)Width / Height;
Proj = PerspectiveMat4(Aspect, 75.0, 0.01, 10000.0);

vec3 A = { 0.0, 0.0, 1.0 };
vec3 B = { 11.0, 2.0, 1.0 };

vec3 Res = Add3(A, B);

```
WaveLoader is a basic C && C++ OBJ, STL, (gltf in future) Loader it can Load Models with their Material or VertexColors.
```C
WaveModelData ModelData = WaveLoadModel("Vulkan.obj/stl", WAVE_LOAD_MATERIAL | WAVE_GEN_NORMALS | WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_INDICES | WAVE_REMOVE_REDUNDANT_MATERIALS);

for (uint32_t i = 0; i < ModelData->MeshCount; i++)
{
	WaveMeshData* WaveMesh = &ModelData->Meshes[i];
	char* DiffuseTexturePath = ModelData->Materials[i].DiffuseTexture;
	
	for (uint32_t j = 0; j < WaveMesh->VertexCount; j++)
	{
		NewVertices[j].Pos.x = WaveMesh->Vertices[j].Vertices.x;
		NewVertices[j].Pos.y = WaveMesh->Vertices[j].Vertices.y;
		NewVertices[j].Pos.z = WaveMesh->Vertices[j].Vertices.z;
		NewVertices[j].Normal.x = WaveMesh->Vertices[j].Normals.x;
		NewVertices[j].Normal.y = WaveMesh->Vertices[j].Normals.y;
		NewVertices[j].Normal.z = WaveMesh->Vertices[j].Normals.z;
		NewVertices[j].TexCoord.x = WaveMesh->Vertices[j].TexCoords.x;
		NewVertices[j].TexCoord.y = WaveMesh->Vertices[j].TexCoords.y;
	}
	
	for (uint32_t j = 0; j < WaveMesh->IndexCount; j++)
	{
		NewIndices[i] = WaveMesh->Indices[i];
	}		
}

WaveFreeModel(&ModelData);
```
