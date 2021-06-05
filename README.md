# Wave
This are Self written Single Header files written in C with the Name Wave.

WaveMath is a basic C Math Library.
```C
mat4 Model;
Model = LoadMat4Identity();
Model = RotateYMat4(VertexConst.Model, 90.0);
Model = TranslateMat4(VertexConst.Model, 1.0, 0.0, 0.0);

mat4 Proj;
float Aspect = (float)Width / Height;
Proj = PerspectivMatrix(Aspect, 75.0, 0.01, 10000.0);

vec3 A = { 0.0, 0.0, 1.0 };
vec3 B = { 11.0, 2.0, 1.0 };

vec3 Res = Add3(A, B);

```
WaveLoader is a basic C OBJ Loader it can Load Triangulated Models with their Material.
```C
WaveModelData ModelData = LoadOBJ("Vulkan.obj", WAVE_FLIP_UVS | WAVE_GEN_UVS | WAVE_GEN_NORMALS | WAVE_LOAD_MATERIAL);

for (int i = 0; i < ModelData.VerticeCount; i++)
{
  Vertices.Pos.x = ModelData.Vertices[i].x;
  Vertices.Pos.y = ModelData.Vertices[i].y;
  Vertices.Pos.z = ModelData.Vertices[i].z;

  Vertices.TexCoord.x = ModelData.TexCoords[i].x;
  Vertices.TexCoord.y = ModelData.TexCoords[i].y;

  Vertices.Normal.x = ModelData.Normals[i].x;
  Vertices.Normal.y = ModelData.Normals[i].y;
  Vertices.Normal.z = ModelData.Normals[i].z;

  Vertices.Color.x = ModelData.Material[i].DiffuseColor.x;
  Vertices.Color.y = ModelData.Material[i].DiffuseColor.y;
  Vertices.Color.z = ModelData.Material[i].DiffuseColor.z;
}	

WaveFreeModel(&ModelData);
```
