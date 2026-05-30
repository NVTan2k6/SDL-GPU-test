#include "Math.hpp"

vec3 operator*(float a, const vec3& v) {
    return v * a;
}
vec4 operator*(float a, const vec4& v) {
    return v * a;
}

vec3 Normalize(vec3 v){
	float mag=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	v.x/=mag;
	v.y/=mag;
	v.z/=mag;
	return v;
}

//Rotate X
mat4 RotationX(float a){
    float c = cosf(a);
    float s = sinf(a);
    return {
        1, 0,  0, 0,
        0, c, -s, 0,
        0, s,  c, 0,
        0, 0,  0, 1
    };
}
//Rotate Y
mat4 RotationY(float a){
    float c = cosf(a);
    float s = sinf(a);
    return {
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1
    };
}
//Rotate Z
mat4 RotationZ(float a){
    float c = cosf(a);
    float s = sinf(a);
    return {
         c,-s,0,0,
         s, c,0,0,
         0, 0,1,0,
         0, 0,0,1
    };
}
mat4 Rotation(float ax, float ay, float az){
    return RotationY(ay)*RotationX(ax)*RotationZ(az);
}

mat4 Translate(float x, float y, float z){
    return {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        x,y,z,1
    };
}

mat4 View(vec3 campos, float yaw, float pitch){
    mat4 rot = RotationY(-yaw) * RotationX(-pitch);
	mat4 trans = Translate(-campos.x, -campos.y, -campos.z);
    return trans*rot;
}

mat4 OnePointPerspective(float fov, float aspect, float near, float far){
    float f = 1.0f / tanf(fov * 0.5f);

    mat4 m{};

    m.m11 = f / aspect;
    m.m22 = f;

    m.m33 = far / (near - far);
    m.m34 = -1.0f;

    m.m43 = (far * near) / (near - far);
    m.m44 = 0.0f;

    return m;
}
mat4 TwoPointsPerspective(float fov, float aspect, float near, float far){
    float f = 1.0f / tanf(fov * 0.5f);
    mat4 m{};
    m.m11 = 1.0f / (f*aspect);
    m.m22 = 1.0f / f;
    m.m33 = (near+far) / (near - far);	
    m.m34 = -1.0f;
    m.m43 = 2.0f * (far * near) / (near - far);	
    m.m44 = 0.0f;
    return m;
}
mat4 OrthographicPerspective(float w, float h, float near, float far){
    mat4 m{};
    m.m11 = 2.0f / w;
    m.m22 = 2.0f / h;
    m.m33 = 1.0f / (near - far);	
	m.m41 = m.m24 = -1;
	m.m43 = near/(near-far);
    m.m44 = 1;
    return m;
}