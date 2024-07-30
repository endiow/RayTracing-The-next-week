#pragma once
#include "Vec3.h"

//三线性插值  用正方体八个顶点的数据得出其内任意一点的数据
inline double trilinear_interp(double c[2][2][2], double u, double v, double w) 
{
    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * u + (1 - i) * (1 - u)) 
                       * (j * v + (1 - j) * (1 - v)) 
                       * (k * w + (1 - k) * (1 - w)) 
                       * c[i][j][k];

    return accum;
}

inline double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) 
{
    //hermite cube来平滑差值。
    //hermite曲线插值保证在线段端点的导数为0，也就是保证了一阶参数连续性
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);

    //最初平滑曲线是这样的 w(t)=3t² - 2t³ ，它保证了w(0)=0, w(1)=1.和w’(0) = 0和w’(1)=0，而新版的这个函数还保证了w’’(0) = 0和w’’(1)=0
    /*auto uu = u * u * u * (u * (u * 6 - 15) + 10);
    auto vv = v * v * v * (v * (v * 6 - 15) + 10);
    auto ww = w * w * w * (w * (w * 6 - 15) + 10);*/

    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) 
            {
                Vec3 weight_v(u - i, v - j, w - k); //该点到各个晶格顶点的距离向量
                accum += (i * uu + (1 - i) * (1 - uu)) 
                       * (j * vv + (1 - j) * (1 - vv)) 
                       * (k * ww + (1 - k) * (1 - ww)) 
                       * Dot(c[i][j][k], weight_v);     //距离向量点乘梯度向量得到权重
            }

    return accum;
}

class perlin 
{
public:
    perlin() 
    {
        ranvec = new Vec3[point_count];

        for (int i = 0; i < point_count; ++i) 
        {
            ranvec[i] = UnitVector(Vec3::random(-1, 1));
        }

        //生成三个打乱顺序的数组  [0,255]
        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~perlin() 
    {
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const Vec3& p) const 
    {
        //u,v,w相当于权重
        auto u = p.X() - floor(p.X());  //得到x,y,z的小数部分  floor向下取整
        auto v = p.Y() - floor(p.Y());
        auto w = p.Z() - floor(p.Z());

        

        //floor向下取整
        int i = static_cast<int>(floor(p.X()));
        int j = static_cast<int>(floor(p.Y()));
        int k = static_cast<int>(floor(p.Z()));
        Vec3 c[2][2][2]{};

        //i,j,k为正方体的最小角  找到八个梯度向量  插值
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^perm_y[(j + dj) & 255] ^perm_z[(k + dk) & 255]];

        return perlin_interp(c, u, v, w);
    }

    //扰动(turbulence), 是一种由多次噪声运算的结果相加得到的产物。
    double turb(const Vec3& p, int depth = 7) const 
    {
        auto accum = 0.0;
        Vec3 temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) 
        {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);     //返回绝对值
    }

private:
    static const int point_count = 256;
    Vec3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm() 
    {
        auto p = new int[point_count];

        for (int i = 0; i < perlin::point_count; i++)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static void permute(int* p, int n) 
    {
        for (int i = n - 1; i > 0; i--) 
        {
            int target = random_int(0, i);  //将第i个值与随机一个小于i的值交换
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
};