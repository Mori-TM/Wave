typedef struct
{
	float x;
	float y;
} vec2;

typedef struct
{
	float x;
	float y;
	float z;
} vec3;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct
{
	float m[4][4];
} mat4;

mat4 LoadMat4Identity()
{
	mat4 Matrix;

	Matrix.m[0][0] = 1.0;
	Matrix.m[1][0] = 0.0;
	Matrix.m[2][0] = 0.0;
	Matrix.m[3][0] = 0.0;

	Matrix.m[0][1] = 0.0;
	Matrix.m[1][1] = 1.0;
	Matrix.m[2][1] = 0.0;
	Matrix.m[3][1] = 0.0;

	Matrix.m[0][2] = 0.0;
	Matrix.m[1][2] = 0.0;
	Matrix.m[2][2] = 1.0;
	Matrix.m[3][2] = 0.0;

	Matrix.m[0][3] = 0.0;
	Matrix.m[1][3] = 0.0;
	Matrix.m[2][3] = 0.0;
	Matrix.m[3][3] = 1.0;

	return Matrix;
}

mat4 MultiplyMat4(mat4 a, mat4 b)
{
	mat4 Matrix;

	Matrix.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2] + a.m[3][0] * b.m[0][3];
	Matrix.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2] + a.m[3][1] * b.m[0][3];
	Matrix.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2] + a.m[3][2] * b.m[0][3];
	Matrix.m[0][3] = a.m[0][3] * b.m[0][0] + a.m[1][3] * b.m[0][1] + a.m[2][3] * b.m[0][2] + a.m[3][3] * b.m[0][3];
																	  			  			  			  
	Matrix.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2] + a.m[3][0] * b.m[1][3];
	Matrix.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2] + a.m[3][1] * b.m[1][3];
	Matrix.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2] + a.m[3][2] * b.m[1][3];
	Matrix.m[1][3] = a.m[0][3] * b.m[1][0] + a.m[1][3] * b.m[1][1] + a.m[2][3] * b.m[1][2] + a.m[3][3] * b.m[1][3];
																	  			  			  			  
	Matrix.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2] + a.m[3][0] * b.m[2][3];
	Matrix.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2] + a.m[3][1] * b.m[2][3];
	Matrix.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2] + a.m[3][2] * b.m[2][3];
	Matrix.m[2][3] = a.m[0][3] * b.m[2][0] + a.m[1][3] * b.m[2][1] + a.m[2][3] * b.m[2][2] + a.m[3][3] * b.m[2][3];
																	  			  			  			  
	Matrix.m[3][0] = a.m[0][0] * b.m[3][0] + a.m[1][0] * b.m[3][1] + a.m[2][0] * b.m[3][2] + a.m[3][0] * b.m[3][3];
	Matrix.m[3][1] = a.m[0][1] * b.m[3][0] + a.m[1][1] * b.m[3][1] + a.m[2][1] * b.m[3][2] + a.m[3][1] * b.m[3][3];
	Matrix.m[3][2] = a.m[0][2] * b.m[3][0] + a.m[1][2] * b.m[3][1] + a.m[2][2] * b.m[3][2] + a.m[3][2] * b.m[3][3];
	Matrix.m[3][3] = a.m[0][3] * b.m[3][0] + a.m[1][3] * b.m[3][1] + a.m[2][3] * b.m[3][2] + a.m[3][3] * b.m[3][3];


	return Matrix;
}

mat4 TranslateMat4(mat4 M, float x, float y, float z)
{
	mat4 Matrix = M;

	Matrix.m[3][0] = x;
	Matrix.m[3][1] = y;
	Matrix.m[3][2] = z;
	Matrix.m[3][3] = 1.0;

	return Matrix;
}

mat4 ScaleMat4(mat4 M, float x, float y, float z)
{
	mat4 Matrix = M;

	Matrix.m[0][0] = x;
	Matrix.m[1][1] *= y;
	Matrix.m[2][2] *= z;
	Matrix.m[3][3] = 1.0;

	return Matrix;
}

mat4 RotateXMat4(mat4 M, float angle)
{
	mat4 Matrix;

	Matrix.m[0][0] = 1.0;
	Matrix.m[1][0] = 0.0;
	Matrix.m[2][0] = 0.0;
	Matrix.m[3][0] = 0.0;

	Matrix.m[0][1] = 0.0;
	Matrix.m[1][1] = cos(angle);
	Matrix.m[2][1] = -sin(angle);
	Matrix.m[3][1] = 0.0;

	Matrix.m[0][2] = 0.0;
	Matrix.m[1][2] = sin(angle);
	Matrix.m[2][2] = cos(angle);
	Matrix.m[3][2] = 0.0;

	Matrix.m[0][3] = 0.0;
	Matrix.m[1][3] = 0.0;
	Matrix.m[2][3] = 0.0;
	Matrix.m[3][3] = 1.0;

	return MultiplyMat4(Matrix, M);
}

mat4 RotateYMat4(mat4 M, float angle)
{

	mat4 Matrix;

	Matrix.m[0][0] = cos(angle);
	Matrix.m[1][0] = 0.0;
	Matrix.m[2][0] = sin(angle);
	Matrix.m[3][0] = 0.0;

	Matrix.m[0][1] = 0.0;
	Matrix.m[1][1] = 1.0;
	Matrix.m[2][1] = 0.0;
	Matrix.m[3][1] = 0.0;

	Matrix.m[0][2] = -sin(angle);
	Matrix.m[1][2] = 0.0;
	Matrix.m[2][2] = cos(angle);
	Matrix.m[3][2] = 0.0;

	Matrix.m[0][3] = 0.0;
	Matrix.m[1][3] = 0.0;
	Matrix.m[2][3] = 0.0;
	Matrix.m[3][3] = 1.0;

	return MultiplyMat4(Matrix, M);
}

mat4 RotateZMat4(mat4 M, float angle)
{
	mat4 Matrix;

	Matrix.m[0][0] = cos(angle);
	Matrix.m[1][0] = -sin(angle);
	Matrix.m[2][0] = 0.0;
	Matrix.m[3][0] = 0.0;

	Matrix.m[0][1] = sin(angle);
	Matrix.m[1][1] = cos(angle);
	Matrix.m[2][1] = 0.0;
	Matrix.m[3][1] = 0.0;

	Matrix.m[0][2] = 0.0;
	Matrix.m[1][2] = 0.0;
	Matrix.m[2][2] = 1.0;
	Matrix.m[3][2] = 0.0;

	Matrix.m[0][3] = 0.0;
	Matrix.m[1][3] = 0.0;
	Matrix.m[2][3] = 0.0;
	Matrix.m[3][3] = 1.0;

	return MultiplyMat4(Matrix, M);
}

float FastInvertSqrt(float number)
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

vec2 Normalize2(vec2 v)
{
	float length = FastInvertSqrt(v.x * v.x + v.y * v.y);
	v.x *= length;
	v.y *= length;

	return v;
}

vec3 Normalize3(vec3 v)
{
	float length = FastInvertSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x *= length;
	v.y *= length;
	v.z *= length;

	return v;
}

vec4 Normalize4(vec4 v)
{
	float length = FastInvertSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	v.x *= length;
	v.y *= length;
	v.z *= length;
	v.w *= length;

	return v;
}

vec3 Cross(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

	return r;
}

float Dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec2 Add2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;

	return r;
}

vec2 Sub2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;

	return r;
}

vec2 Mul2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;

	return r;
}

vec2 Div2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;

	return r;
}


vec3 Add3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

vec3 Sub3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

vec3 Mul3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;

	return r;
}

vec3 Div3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;

	return r;
}

vec4 Add4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	r.w = a.w + b.w;

	return r;
}

vec4 Sub4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	r.w = a.w - b.w;

	return r;
}

vec4 Mul4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	r.w = a.w * b.w;

	return r;
}

vec4 Div4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;
	r.w = a.w / b.w;

	return r;
}

mat4 LookAt(vec3 Pos, vec3 LookAt, vec3 Up)
{
	mat4 Matrix;
	
	vec3 Z = Normalize3(Sub3(LookAt, Pos));
	vec3 X = Normalize3(Cross(Z, Up));
	vec3 Y = Cross(X, Z);

	Z.x *= -1;
	Z.y *= -1;
	Z.z *= -1;

	Matrix.m[0][0] = X.x;
	Matrix.m[1][0] = X.y;
	Matrix.m[2][0] = X.z;
	Matrix.m[3][0] = -Dot(X, Pos);
	Matrix.m[0][1] = Y.x;
	Matrix.m[1][1] = Y.y;
	Matrix.m[2][1] = Y.z;
	Matrix.m[3][1] = -Dot(Y, Pos);
	Matrix.m[0][2] = Z.x;
	Matrix.m[1][2] = Z.y;
	Matrix.m[2][2] = Z.z;
	Matrix.m[3][2] = -Dot(Z, Pos);
	Matrix.m[0][3] = 0;
	Matrix.m[1][3] = 0;
	Matrix.m[2][3] = 0;
	Matrix.m[3][3] = 1.0;

	return Matrix;
}

float Radians(float Angle)
{
	return Angle * 3.14159265359 / 180;
}

mat4 PerspectivMatrix(float Aspect, float FOV, float NearPlane, float FarPlane)
{
	float F = 1.0f / tan(Radians(0.5 * FOV));

	mat4 Matrix =
	{
	  F / Aspect,
	  0.0,
	  0.0,
	  0.0,

	  0.0,
	  -F,
	  0.0,
	  0.0,

	  0.0,
	  0.0,
	  FarPlane / (NearPlane - FarPlane),
	  -1.0,

	  0.0,
	  0.0,
	  (NearPlane * FarPlane) / (NearPlane - FarPlane),
	  0.0
	};

	return Matrix;
}

mat4 OrthoMatrix(float Left, float Right, float Bottom, float Top, float Znear, float Zfar)
{
	mat4 Matrix;

	Matrix.m[0][0] = 2.0 / (Right - Left);
	Matrix.m[1][0] = 0.0;
	Matrix.m[2][0] = 0.0;
	Matrix.m[3][0] = 0.0;

	Matrix.m[0][1] = 0.0;
	Matrix.m[1][1] = 2.0 / (Top - Bottom);
	Matrix.m[2][1] = 0.0;
	Matrix.m[3][1] = 0.0;

	Matrix.m[0][2] = 0.0;
	Matrix.m[1][2] = 0.0;
	Matrix.m[2][2] = 2.0 / (Zfar - Znear);
	Matrix.m[3][2] = 0.0;

	Matrix.m[0][3] = -(Right + Left) / (Right - Left);
	Matrix.m[1][3] = -(Top + Bottom) / (Top - Bottom);
	Matrix.m[2][3] = -(Zfar + Znear) / (Zfar - Znear);
	Matrix.m[3][3] = 1.0;

	return Matrix;
}