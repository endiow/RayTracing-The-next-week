# RayTracing-The-next-week

使用vs2022+opencv实现RayTracing The next week的内容，使用opencv将ppm图片实时渲染。

一个ppm文件查看网站：https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

## BVH树：

![image](https://github.com/user-attachments/assets/b25d770e-5c44-4b00-9a00-c954f7081b16)

使用bvh后速度大大提升

同一张动态模糊的图，没有bvh用了70分钟，用bvh只需13分钟！

![bvh.png](./RayTracing_1/output/moving_bvh.png)
