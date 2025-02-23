#pragma once
/*
Vec3类为空间三维坐标或RGB颜色的抽象类
E[3]数组为向量的具体数值
*/
#include <iostream>
#include <math.h>

#include "rtweekend.h"

class Vec3
{
public:	
	double E[3] = {0};

public:
	Vec3() {};
	Vec3(double E0, double E1, double E2)
	{
		E[0] = E0;
		E[1] = E1;
		E[2] = E2;
	}

	//返回单个元素的值
	//作为位置
	inline double X() const  { return E[0]; }
	inline double Y() const  { return E[1]; }
	inline double Z() const  { return E[2]; }
	//作为RGB颜色
	inline double R() const  { return E[0]; }
	inline double G() const  { return E[1]; }
	inline double B() const  { return E[2]; }

	//以下为运算符重载
	inline const Vec3& operator+() const  { return *this; }
	inline Vec3 operator-() const  { return Vec3(-E[0], -E[1], -E[2]); }
	inline double operator[](int i) const  { return E[i]; } //根据i获取元素
	inline double& operator[](int i) { return E[i]; } //根据下标i获取元素引用

	inline Vec3& operator+=(const Vec3& V2);
	inline Vec3& operator-=(const Vec3& V2);
	inline Vec3& operator*=(const Vec3& V2);
	inline Vec3& operator/=(const Vec3& V2);
	inline Vec3& operator*=(const double T);
	inline Vec3& operator/=(const double T);

	//返回向量长度，即模长
	inline double Vec3Length() const 
	{
		return sqrt(E[0] * E[0] + E[1] * E[1] + E[2] * E[2]);
	}

	//返回向量的平方
	inline double SquaredLength() const 
	{
		return E[0] * E[0] + E[1] * E[1] + E[2] * E[2];
	}

	//获得单位向量
	inline void  MakeUnitVector();

	//将颜色写入ppm
	void write_color(std::ostream& out, int samples_per_pixel) const;

	//生成随机向量
	inline static Vec3 random()
	{
		return Vec3(random_double(), random_double(), random_double());
	}

	inline static Vec3 random(double min, double max)
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	//所有光线都从内部的一个虚拟透镜发出, 经过lookfrom点, 这个透镜的半径越大, 图像就越模糊
	inline static Vec3 random_in_unit_disk() 
	{
		while (true) 
		{
			auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
			if (p.SquaredLength() >= 1) continue;
			return p;
		}
	}


	//以下为Vec3类的友元函数，在类内声明，类外定义，友元函数不属于任何类
	friend inline Vec3 UnitVector(Vec3 V);
	friend inline std::istream& operator>>(std::istream& IS, Vec3& T);
	friend inline std::ostream& operator<<(std::ostream& OS, const Vec3& T);
	friend inline Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator*(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator/(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator*(double T, const Vec3& V);
	friend inline Vec3 operator/(const Vec3& V, double T);
	friend inline Vec3 operator*(const Vec3& V, double T);
	friend inline double Dot(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 Cross(const Vec3& V1, const Vec3& V2);
};

inline Vec3& Vec3::operator+=(const Vec3& V)
{
	E[0] += V.E[0];
	E[1] += V.E[1];
	E[2] += V.E[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& V)
{
	E[0] *= V.E[0];
	E[1] *= V.E[1];
	E[2] *= V.E[2];
	return *this;
}
inline Vec3& Vec3::operator-=(const Vec3& V)
{
	E[0] -= V.E[0];
	E[1] -= V.E[1];
	E[2] -= V.E[2];
	return *this;
}
inline Vec3& Vec3::operator/=(const Vec3& V)
{
	E[0] /= V.E[0];
	E[1] /= V.E[1];
	E[2] /= V.E[2];
	return *this;
}
inline Vec3& Vec3::operator*=(const double T)
{
	E[0] *= T;
	E[1] *= T;
	E[2] *= T;
	return *this;
}
inline Vec3& Vec3::operator/=(const double T)
{
	double K = 1.0 / T;
	E[0] *= K;
	E[1] *= K;
	E[2] *= K;
	return *this;
}

//获取向量的单位向量
inline void Vec3::MakeUnitVector()
{
	double k = 1.0 / sqrt(E[0] * E[0] + E[1] * E[1] + E[2] * E[2]);
	E[0] *= k;
	E[1] *= k;
	E[2] *= k;
}

inline void Vec3::write_color(std::ostream& out, int samples_per_pixel) const
{
	// Divide the color total by the number of samples.
	auto scale = 1.0 / samples_per_pixel;
	auto r = scale * E[0];
	auto g = scale * E[1];
	auto b = scale * E[2];

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}


//以下为类的友元函数

//向量/模长 = 单位方向向量
//这里可能有问题
inline Vec3 UnitVector(Vec3 V)
{
	return V / V.Vec3Length();
}

//输入运算符重载 ，用于输出Vec3元素数据
inline std::istream& operator>>(std::istream& IS, Vec3& T)
{
	IS >> T.E[0] >> T.E[1] >> T.E[2];
	return IS;
}

//输出运算符重载，用于输入Vec3元素数据
inline std::ostream& operator<<(std::ostream& OS, const Vec3& T)
{
	OS << T.E[0] << " " << T.E[1] << " " << T.E[2];
	return OS;
}

inline Vec3 operator+(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] + V2.E[0], V1.E[1] + V2.E[1], V1.E[2] + V2.E[2]);
}

inline Vec3 operator-(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] - V2.E[0], V1.E[1] - V2.E[1], V1.E[2] - V2.E[2]);
}

inline Vec3 operator*(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] * V2.E[0], V1.E[1] * V2.E[1], V1.E[2] * V2.E[2]);
}

inline Vec3 operator/(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] / V2.E[0], V1.E[1] / V2.E[1], V1.E[2] / V2.E[2]);
}

inline Vec3 operator*(double T, const Vec3& V)
{
	return Vec3(T * V.E[0], T * V.E[1], T * V.E[2]);
}

inline Vec3 operator*(const Vec3& V, double T)
{
	return Vec3(T * V.E[0], T * V.E[1], T * V.E[2]);
}

inline Vec3 operator/(const Vec3& V, double T)
{
	return Vec3(V.E[0] / T, V.E[1] / T, V.E[2] / T);
}


//向量的dian乘，结果是数字 C = |a|·|b|·cos(a^b);	C = a1 * b1 + a2 * b2 + a3 * b3;
inline double Dot(const Vec3& V1, const Vec3& V2)
{
	return V1.E[0] * V2.E[0] + V1.E[1] * V2.E[1] + V1.E[2] * V2.E[2];
}


//向量的叉乘，结果是一个垂直于V1和V2所在平面的向量，其模长为 |a×b|=|a|·|b|·sin<a,b
inline Vec3 Cross(const Vec3& V1, const Vec3& V2)
{
	return Vec3(
		(V1.E[1] * V2.E[2] - V1.E[2] * V2.E[1]),
		(-(V1.E[0] * V2.E[2] - V1.E[2] * V2.E[0])),
		(V1.E[0] * V2.E[1] - V1.E[1] * V2.E[0])
	);
}