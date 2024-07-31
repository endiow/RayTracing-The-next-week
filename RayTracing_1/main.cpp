#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "header/rtweekend.h"
#include "header/Sphere.h"
#include "header/HitableList.h"
#include "header/Camera.h"
#include "header/Material.h"
#include "header/scene.h"


using namespace cv;

static Vec3 Color(const Ray& r, const Vec3& background, const Hitable& world, int depth)
{
	HitRecord Rec;

	//�ݹ���ֱ�ӷ��غ�ɫ
	if (depth <= 0)
		return Vec3(0, 0, 0);

	//û�л��У����ر���ֵ
	//���Ե�0������һ���ַ�Χ, ��ֹ���巢���Ĺ����ٴ����Լ��ཻ
	if (!world.Hit(r, 0.001, infinity, Rec))
		return background;

	Ray scattered;	//ɢ��
	Vec3 attenuation;	//˥��
	Vec3 emitted = Rec.mat_ptr->emitted(Rec.u, Rec.v, Rec.P);	//��������

	//û�з���ɢ�䣬���ع�Դ��ֵ
	if (!Rec.mat_ptr->Scatter(r, Rec, attenuation, scattered))
		return emitted;

	//�������Ϊ��㣬����Ϊ���������ཻ��һ�� ��������
	//����������ײ�����������
	
	//���ǹ�Դ�����ض�η���,�����ֵ
	return emitted + attenuation * Color(scattered, background, world, depth - 1);	
}

int main()
{
	const int nx = 400;//ͼƬ��ȣ���λΪ���أ�
	const int ny = 225;//ͼƬ�߶ȣ���λΪ���أ�
	const int ns = 100;//ɨ�����
	const int max_depth = 50;
	const double aspect_ratio = double(nx) / ny;
	const Vec3 background(0, 0, 0);

	//�ļ�ͷд��
	std::cout << "P3" << std::endl << nx << " " << ny << std::endl << "255" << std::endl;

	//Ԥ���������
	int WindowWidth;//���ڿ��
	int WindowHeight;//���ڸ߶�
	if (nx > ny)  //�����Ⱦ��ͼƬΪ��棬���㴰�ڿ����߶�
	{
		WindowHeight = 720;
		WindowWidth = (int)((double)WindowHeight * (double)nx / (double)ny);
	}
	else //�����Ⱦ��ͼƬΪ���棬���㴰�ڿ����߶�
	{
		WindowWidth = 1200;
		WindowHeight = (int)((double)WindowWidth * (double)ny / (double)nx);
	}

	Mat RenderingImage(ny, nx, CV_8UC3, Scalar(50, 50, 50)); //����һ��ͼƬ
	namedWindow("ͼ��Ԥ������Ⱦ�У�", WINDOW_NORMAL);//���ñ���
	moveWindow("ͼ��Ԥ������Ⱦ�У�", (int)((1920.0 - WindowWidth) / 2), (int)((1080.0 - WindowHeight) / 2) - 50);//���ô���λ��
	resizeWindow("ͼ��Ԥ������Ⱦ�У�", WindowWidth, WindowHeight);//���ô��ڴ�С

	Vec3 lookfrom(278, 278, -800);
	Vec3 lookat(278, 278, 0);
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	auto vfov = 40.0;

	//Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 0.0);
	Camera camera(Vec3(13,2,3), Vec3(0, 2, 0), vup, 20.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	//�����ʼ��
	HitableList world;
	world = random_scene_0_final();
	//world = random_scene_1_moving();
	//world = two_perlin_spheres();
	//world = earth();
	//world = simple_light();
	//world = cornell_box();
	//world.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5, make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5))));
	//world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100, make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0))));
	//world.add(make_shared<Sphere>(Vec3(1, 0, -1), 0.5, make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0)));
	////world.add(make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 1.0)));
	//world.add(make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
	//world.add(make_shared<Sphere>(Vec3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5)));

	for (int j = ny - 1; j >= 0; j--)//����Ϣ
	{
		for (int i = 0; i < nx; i++)//����Ϣ
		{
			//��ʼ����ɫ
			Vec3 Col(0, 0, 0);

			for (int s = 0; s < ns; s++)
			{
				double U = double((i + random_double(0, 1)) / double(nx));
				double V = double((j + random_double(0, 1)) / double(ny));
				//�õ�ԭ������A�ͷ�������B
				Ray r = camera.GetRay(U, V);
				//��������Ƿ񾭹����壬������ɫ
				//�ۼӣ�ȡ��ֵ
				Col += Color(r, background, world, max_depth);
			}
			//ȡ��ֵ
			Col /= double(ns);
			//٤��У��
			Col = Vec3(sqrt(Col[0]), sqrt(Col[1]), sqrt(Col[2]));

			//�������������߼�⵽����ɫ���Ϊ�졢�̡�������ͨ��
			int ir = colCap(int(255.99 * Col.R()));
			int ig = colCap(int(255.99 * Col.G()));
			int ib = colCap(int(255.99 * Col.B()));

			//����ǰ���ص�����ͨ��ֵд���ļ�
			std::cout << ir << " " << ig << " " << ib << std::endl;
			//ʵʱ��ʾ��Ⱦ֡
			RenderingImage.at<cv::Vec3b>(ny - 1 - j, i)[0] = ib;
			RenderingImage.at<cv::Vec3b>(ny - 1 - j, i)[1] = ig;
			RenderingImage.at<cv::Vec3b>(ny - 1 - j, i)[2] = ir;
			//Sleep(0);

		}
		//ÿ�м������Ժ�ˢ��Ԥ������
		if (!(j % (ny / 100))) //ÿ��Ⱦny/100�к����Ԥ������ͼƬ
		{
			imshow("ͼ��Ԥ������Ⱦ�У�", RenderingImage);
			waitKey(1);//�ȴ�1�����¼��ô���ˢ�����
		}
	}
	imshow("ͼ��Ԥ������Ⱦ�У�", RenderingImage);
	waitKey(5000); //�ȴ�3000����
	destroyAllWindows();//�رմ���
	return 0;
}
