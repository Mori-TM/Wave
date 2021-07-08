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
Proj = PerspectivMatrix(Aspect, 75.0, 0.01, 10000.0);

vec3 A = { 0.0, 0.0, 1.0 };
vec3 B = { 11.0, 2.0, 1.0 };

vec3 Res = Add3(A, B);

```
WaveLoader is a basic C && C++ OBJ, STL and Dae Loader it can Load Models with their Material or VertexColors.
```C
WaveModelData ModelData = WaveLoadOBJ("Vulkan.obj", WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_NORMALS | WAVE_LOAD_MATERIAL);
              ModelData = WaveLoadSTL("Vulkan.stl", WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_NORMALS | WAVE_LOAD_MATERIAL);
              //No Material Loading just Vertex Colors
              ModelData = WaveLoadDAE("Vulkan.dae", WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_NORMALS | WAVE_LOAD_MATERIAL);
              //Or use
              ModelData = WaveLoadModel("Vulkan.(dae/obj/stl)", WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_NORMALS | WAVE_LOAD_MATERIAL);

int MaterialIndex = 0;

for (int i = 0; i < ModelData->Mesh->VerticeCount; i++)
{
  Vertices.Pos.x = ModelData.Mesh[i].Vertices.x;
  Vertices.Pos.y = ModelData.Mesh[i].Vertices.y;
  Vertices.Pos.z = ModelData.Mesh[i].Vertices.z;

  Vertices.TexCoord.x = ModelData.Mesh[i].TexCoords.x;
  Vertices.TexCoord.y = ModelData.Mesh[i].TexCoords.y;

  Vertices.Normal.x = ModelData.Mesh[i].Normals.x;
  Vertices.Normal.y = ModelData.Mesh[i].Normals.y;
  Vertices.Normal.z = ModelData.Mesh[i].Normals.z;
  
  //check if Model Has Material
  if (ModelData->HasMaterial == 1)
		{
			if (ModelData->Material[MaterialIndex].VertexIndex == i)
				MaterialIndex++;

			Vertices.Vertex.AmbientColor.x = ModelData->Material[MaterialIndex-1].AmbientColor.x;
			Vertices.Vertex.AmbientColor.y = ModelData->Material[MaterialIndex-1].AmbientColor.y;
			Vertices.Vertex.AmbientColor.z = ModelData->Material[MaterialIndex-1].AmbientColor.z;
      
      Vertices.Vertex.DiffuseColor.x = ModelData->Material[MaterialIndex-1].DiffuseColor.x;
			Vertices.Vertex.DiffuseColor.y = ModelData->Material[MaterialIndex-1].DiffuseColor.y;
			Vertices.Vertex.DiffuseColor.z = ModelData->Material[MaterialIndex-1].DiffuseColor.z;
		}
		else
		{
			Vertices.Vertex.Color.x = ModelData->Mesh[i].VertexColor.x;
			Vertices.Vertex.Color.y = ModelData->Mesh[i].VertexColor.y;
			Vertices.Vertex.Color.z = ModelData->Mesh[i].VertexColor.z;
		}
}	

WaveFreeModel(&ModelData);
```
