#pragma once

#include "Hitable.h"
#include "Texture.h"


class Material
{
public:
	/*
		*���ϵ�ɢ����Ϊ
		*��������:������ߡ�������Ϣ��˥����ɢ�䡣
	*/
	virtual Vec3 emitted(double u, double v, const Vec3& p) const	//������ߣ�ֻ�й�Դʵ��
	{
		return Vec3(0.0, 0.0, 0.0);
	}

	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	Lambertian(shared_ptr<texture> A) :Albedo(A) {}

	//ɢ����Ϊ
	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		//����㵥λ�����������
		Vec3 Target = Rec.P + Rec.Normal + RandomInUnitSphere();	//������㷨�߶˵�ΪԲ�ĵ�Բ�ڵ� ���������
		//Target = UnitVector(Target);

		Scattered = Ray(Rec.P, Target - Rec.P, RIn.time());		//����Ray��ɢ�����
		Attenuation = Albedo->value(Rec.u, Rec.v, Rec.P);	//����˥������
		return true;
	}

public:
	shared_ptr<texture> Albedo;  //����ָ��
};

/*
*	��������
*/
class Metal : public Material
{
	Vec3 Albedo; //������
	double Fuzz;//�ֲڶ�

public:

	Metal(const Vec3& A) :Albedo(A), Fuzz(0.5) {}

	Metal(const Vec3& A, double F) :Albedo(A) 
	{
		Fuzz = F > 1 ? 1 : F < 0 ? 0 : F;	//���ƴֲڶ���0��1֮��
	}

	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		Vec3 Reflected = Reflect(UnitVector(RIn.Direction()), Rec.Normal);//��÷������
		//Scattered = Ray(Rec.P, Reflected);//�õ�������㷴��Ĺ���

		//���ݴֲڶȻ�ȡ���������ߣ��ֲڶ�Խ�󣬷��������ɢ�Ը���
		Scattered = Ray(Rec.P, Reflected + Fuzz * RandomInUnitSphere(), RIn.time());
		Attenuation = Albedo;//���� ˥��
		return (Dot(Scattered.Direction(), Rec.Normal) > 0); //�����������뷨�ߵļн�Ϊ�����return true ���淴��
	}
};

//���ڷ��ص�ǰ�Ƕ��·�����
inline double Schlick(double Cosine, double RefIdx_)
{
	double R0 = (1 - RefIdx_) / (1 + RefIdx_);
	R0 = R0 * R0;
	return R0 + (1 - R0) * pow((1 - Cosine), 5);
}

//͸����
class Dielectric : public Material
{
	double RefIdx;	//����du

public:
	Dielectric(double Ri) :RefIdx(Ri) {}

	virtual bool Scatter(const Ray& RIn, const HitRecord& Rec, Vec3& Attenuation, Ray& Scattered) const
	{
		Attenuation = Vec3(1.0, 1.0, 1.0);
		//ȷ������
		double etai_over_etat = (Rec.front_face) ? (1.0 / RefIdx) : (RefIdx);

		Vec3 unit_direction = UnitVector(RIn.Direction());
		double cos_theta = ffmin(Dot(-unit_direction, Rec.Normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		//���еĹ��߶�����������, ת�������˷���
		if (etai_over_etat * sin_theta > 1.0) 
		{
			Vec3 reflected = Reflect(unit_direction, Rec.Normal);
			Scattered = Ray(Rec.P, reflected, RIn.time());
			return true;
		}

		//��������ĸ��ʻ���������Ƕ��ı�
		double reflect_prob = Schlick(cos_theta, etai_over_etat);
		if (random_double(0, 1) < reflect_prob)
		{
			//��������
			Vec3 reflected = Reflect(unit_direction, Rec.Normal);
			Scattered = Ray(Rec.P, reflected, RIn.time());
			return true;
		}

		//�����������
		Vec3 refracted = refract(unit_direction, Rec.Normal, etai_over_etat);
		Scattered = Ray(Rec.P, refracted, RIn.time());

		return true;
	}

	friend inline double Schlick(double Cosine, double RefIdx_);
};


//��Դ
class diffuse_light : public Material 
{
public:
	diffuse_light(shared_ptr<texture> a) : emit(a) {}

	virtual Vec3 emitted(double u, double v, const Vec3& p) const 
	{
		return emit->value(u, v, p);
	}

public:
	shared_ptr<texture> emit;
};