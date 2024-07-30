#pragma once

#include "Hitable.h"
#include "AABB.h"

class Sphere : public Hitable
{
	Vec3 Center = { 0,0,0 }; //����
	double Radius = 0;//�뾶
	shared_ptr<Material> mat_ptr;

public:
	Sphere() {}
	Sphere(Vec3 Cen, double R, shared_ptr<Material> m) :Center(Cen), Radius(R), mat_ptr(m) {};	//���캯������ʼ��������뾶

	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;	//�̳�Hitable��Hit

	bool bounding_box(double t0, double t1, aabb& output_box) const;	//�����Χ��
};


//�������R��Tmin��Tmax��Χ�����е�ǰ���壬�򷵻�true
bool Sphere::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec)const
{
	//��һԪ���η���
	Vec3 OC = R.Origin() - Center;//�����ĵ����߷���������
	double A = Dot(R.Direction(), R.Direction());
	double B = Dot(OC, R.Direction());
	double C = Dot(OC, OC) - Radius * Radius;
	double Discriminant = B * B - A * C;

	if (Discriminant > 0) //�����������
	{

		//Rec.MatPtr = MatPtr;
		//����Ƚ�С�Ľ�
		double Temp = (-B - sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin) //�����������T��TMin��TMax֮�䣬��������Ҫ��ֵ
		{
			//���������Ľ⡢�����λ�ú�����㷨�ߵ�Rec��
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
		//����ϱߵĵ㲻�����ǵļ�ⷶΧ�ڣ�����ȥ��ͬ���ķ��������һ����
		Temp = (-B + sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin)
		{
			//���������Ľ⡢�����λ�ú�����㷨�ߵ�Rec��
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	//�������û���������򷵻�false
	return false;
}

bool Sphere::bounding_box(double t0, double t1, aabb& output_box) const 
{
	output_box = aabb(Center - Vec3(Radius, Radius, Radius), Center + Vec3(Radius, Radius, Radius));
	return true;
}


class Moving_Sphere : public Hitable
{
	Vec3 Center0, Center1; //����
	double time0 = 0, time1 = 0;
	double Radius = 0;//�뾶
	shared_ptr<Material> mat_ptr;

public:
	Moving_Sphere() {}
	Moving_Sphere(Vec3 cen0, Vec3 cen1, double t0, double t1, double r, shared_ptr<Material> m)
		: Center0(cen0), Center1(cen1), time0(t0), time1(t1), Radius(r), mat_ptr(m) {};	//���캯������ʼ��������뾶

	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;	//�̳�Hitable��Hit

	Vec3 Center(double time) const;

	bool bounding_box(double t0, double t1, aabb& output_box) const;
};

//�������R��Tmin��Tmax��Χ�����е�ǰ���壬�򷵻�true
bool Moving_Sphere::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
{
	//��һԪ���η���
	Vec3 OC = R.Origin() - Center(R.time());//�����ĵ����߷���������
	double A = Dot(R.Direction(), R.Direction());
	double B = Dot(OC, R.Direction());
	double C = Dot(OC, OC) - Radius * Radius;
	double Discriminant = B * B - A * C;

	if (Discriminant > 0) //�����������
	{

		//Rec.MatPtr = MatPtr;
		//����Ƚ�С�Ľ�
		double Temp = (-B - sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin) //�����������T��TMin��TMax֮�䣬��������Ҫ��ֵ
		{
			//���������Ľ⡢�����λ�ú�����㷨�ߵ�Rec��
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center(R.time())) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
		//����ϱߵĵ㲻�����ǵļ�ⷶΧ�ڣ�����ȥ��ͬ���ķ��������һ����
		Temp = (-B + sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin)
		{
			//���������Ľ⡢�����λ�ú�����㷨�ߵ�Rec��
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center(R.time())) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	//�������û���������򷵻�false
	return false;
}

Vec3 Moving_Sphere::Center(double time) const 
{
	return Center0 + ((time - time0) / (time1 - time0)) * (Center1 - Center0);	//��ֵ�ó����ʱ������λ��
}

bool Moving_Sphere::bounding_box(double t0, double t1, aabb& output_box) const 
{
	aabb box0(
		Center(t0) - Vec3(Radius, Radius, Radius),
		Center(t0) + Vec3(Radius, Radius, Radius));

	aabb box1(
		Center(t1) - Vec3(Radius, Radius, Radius),
		Center(t1) + Vec3(Radius, Radius, Radius));

	output_box = aabb::surrounding_box(box0, box1);
	return true;
}