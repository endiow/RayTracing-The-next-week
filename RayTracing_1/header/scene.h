#pragma once
#include "HitableList.h"
#include "Material.h"
#include "Sphere.h"
#include "BVH.h"

static HitableList random_scene_0_final()
{
	HitableList world;

	world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double(0, 1);
			Vec3 center(a + 0.9 * random_double(0, 1), 0.2, b + 0.9 * random_double(0, 1));
			if ((center - Vec3(4, 0.2, 0)).Vec3Length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					// diffuse
					auto albedo = Vec3(random_double(0, 1), random_double(0, 1), random_double(0, 1)) * Vec3(random_double(0, 1), random_double(0, 1), random_double(0, 1));
					world.add(make_shared<Sphere>(center, 0.2, make_shared<Lambertian>(albedo)));
				}
				else if (choose_mat < 0.95)
				{
					// Metal
					auto albedo = Vec3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
					auto fuzz = random_double(0, .5);
					world.add(make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
				}
				else
				{
					// glass
					world.add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));

	world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));

	world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<HitableList>(make_shared<bvh_node>(world, 0, 1));
}


HitableList random_scene_1_moving() 
{
	HitableList world;

	world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -10; a < 10; a++) 
	{
		for (int b = -10; b < 10; b++) 
		{
			auto choose_mat = random_double();
			Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - Vec3(4, .2, 0)).Vec3Length() > 0.9) 
			{
				if (choose_mat < 0.8) 
				{
					// diffuse
					auto albedo = Vec3::random() * Vec3::random();
					world.add(make_shared<Moving_Sphere>(center, center + Vec3(0, random_double(0, .5), 0), 0.0, 1.0, 0.2,make_shared<Lambertian>(albedo)));
				}
				else if (choose_mat < 0.95) 
				{
					// Metal
					auto albedo = Vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));
	world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));
	world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<HitableList>(make_shared<bvh_node>(world, 0, 1));
}