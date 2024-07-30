#pragma once

#include "Hitable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HitableList : public Hitable
{
public:
	std::vector<shared_ptr<Hitable>> objects;	//���������б�

public:
	HitableList() {}
	HitableList(shared_ptr<Hitable> object) { add(object); }	//��ʼ��/��� ���� 

	void clear() { objects.clear(); }
	void add(shared_ptr<Hitable> object) { objects.push_back(object); }
	std::vector<shared_ptr<Hitable>> Obj() const { return objects; }

	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec)const;

	bool bounding_box(double t0, double t1, aabb& output_box) const;
};

//���ٵ��ҳ��������еľ�������Ķ�����Ϊ��Ⱦֻ��Ⱦ��ǰ��ģ�����ı�ǰ��ĵ�ס�ˣ�
bool HitableList::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec)const
{
	HitRecord TempRec; //��¼���н��
	bool HitAnything = false;//��ʾ�Ƿ���������
	double ClosestSoFar = TMax;//��̬���������ߵ�β��

	for (const auto& object : objects)//�����б�
	{
		if (object->Hit(R, TMin, ClosestSoFar, TempRec))//�����ǰ���б�Ԫ�ر�����R����
		{
			HitAnything = true;	//��HitAnything����Ϊtrue����ʾ����R����������
			ClosestSoFar = TempRec.T;
			Rec = TempRec;
		}
	}
	return HitAnything;
}

inline bool HitableList::bounding_box(double t0, double t1, aabb& output_box) const
{
	if (objects.empty()) return false;

	aabb temp_box;
	bool first_box = true;

	for (const auto& object : objects) 
	{
		if (!object->bounding_box(t0, t1, temp_box)) return false;
		output_box = first_box ? temp_box : aabb::surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}
