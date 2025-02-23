# RayTracing-The-next-week

使用vs2022+opencv实现RayTracing The next week的内容，使用opencv将ppm图片实时渲染。

一个ppm文件查看网站：https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

## BVH树：

![image](./images/bvh.png)

使用bvh后速度大大提升

同一张动态模糊的图，没有bvh用了70分钟，用bvh只需13分钟！

![bvh.png](./RayTracing_1/output/moving_bvh.png)

## 柏林噪声

跟着跑代码很容易，但是要理解比较难

### 通过哈希映射得到随机值

![images](./RayTracing_1/output/perlin1.png)

### 三线性插值

![images](./RayTracing_1/output/perlin2.png)

### 使用Hermit Cubic进行插值

![images](./RayTracing_1/output/perlin3.png)

五次曲线 w(t) = t * t * t * (t * (t * 6 - 15) + 10)

最初平滑曲线是这样的 w(t)=3t² - 2t³ ，它保证了w(0)=0, w(1)=1.和w’(0) = 0和w’(1)=0，而新版的这个函数还保证了w’’(0) = 0和w’’(1)=0

![image](./images/chazhi.png)

### 梯度向量和距离向量点乘

![images](./RayTracing_1/output/perlin5.png)

我的理解是：三线性插值得到的结果还在正方体中，用梯度向量后得到的结果却可以跳出正方体，也就是说，可以取得的颜色更广，自然也就没有方块感

### 扰动(turbulence)

使用多个频率相加得到复合噪声

![images](./RayTracing_1/output/perlin6.png)

对于不同参数得到其他效果，可以深入学习分形布朗运动（‌FBM）

## cornell box

RayTracing的版本太多了，每个版本都有不同，导致我的cornell box出现了非常奇葩的bug

![images](./RayTracing_1/images/issue.png)

光线似乎并不朝所有方向发射

最烦的是，我修好了这个bug，平面又不显示了

最后还是去官网啃英语，重写了version 4 （其实当做复习也挺好的）

![image](./RayTracing_1_v4/output/cornell_box.png)

## 最终场景

![image](./RayTracing_1_v4/output/final.png)

参考文章：http://t.csdnimg.cn/Whhex

https://zhuanlan.zhihu.com/p/129372723

http://t.csdnimg.cn/0X1Ga
