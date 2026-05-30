#pragma once
#include <cmath>

inline float pi=3.14159265359;

// Matrix Math
struct mat4{
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
	mat4 operator*(const mat4& b) const {
		const mat4& a = *this;
		mat4 r{};

		r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
		r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
		r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
		r.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

		r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
		r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
		r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
		r.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

		r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
		r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
		r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
		r.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

		r.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
		r.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
		r.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
		r.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;

		return r;
	}
};

struct vec3{
	float x, y, z;
	vec3 operator+(const vec3& b) const {
		return {x+b.x, y+b.y, z+b.z};	
	}
	vec3 operator-(const vec3& b) const {
		return {x-b.x, y-b.y, z-b.z};	
	}
	vec3 operator*(const float a) const {
		return {x*a, y*a, z*a};	
	}
};

struct vec4{
	float x, y, z, w = 0;
	vec4(float _x=0, float _y=0, float _z=0, float _w=0){
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	}
	vec4(vec3 v, float _w=0){
		x=v.x;
		y=v.y;
		z=v.z;
		w=_w;
	}
	vec4 operator+(const vec4& b) const {
		return {x+b.x, y+b.y, z+b.z, w+b.w};	
	}
	vec4 operator-(const vec4& b) const {
		return {x-b.x, y-b.y, z-b.z, w+b.w};	
	}
	vec4 operator*(const float a) const {
		return {x*a, y*a, z*a, w*a};	
	}
};

extern vec3 Normalize(vec3 v);

extern mat4 RotationX(float a);
extern mat4 RotationY(float a);
extern mat4 RotationZ(float a);
extern mat4 Rotation(float ax, float ay, float az);

extern mat4 Translate(float x, float y, float z);//Shift position

extern mat4 View(vec3 campos, float yaw, float pitch);//View, shift objects by player relative position and direction

extern mat4 OnePointPerspective(float fov, float aspect, float znear, float zfar);
extern mat4 TwoPointsPerspective(float fov, float aspect, float znear, float zfar);
extern mat4 OrthographicPerspective(float w, float h, float znear, float zfar);
