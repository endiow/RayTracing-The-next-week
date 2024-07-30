# RayTracing-The-next-week

使用vs2022+opencv实现RayTracing The next week的内容，使用opencv将ppm图片实时渲染。

一个ppm文件查看网站：https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

## BVH树：

![image](./images/bvh.png)

使用bvh后速度大大提升

同一张动态模糊的图，没有bvh用了70分钟，用bvh只需13分钟！

![bvh.png](./RayTracing_1/output/moving_bvh.png)
