# CAM_Qt
这是一个基于Qt开发的用于CAD/CAM软件，用于三通管纤维缠绕的轨迹生成。
项目的演示视频，见  https://www.bilibili.com/video/BV1LE411c7n4/


## 项目亮点
- 项目架构为MVC, 将界面、控制和数据相关代码进行分离。
- 几何元素的继承关系, 本项目的几何元素如下图所示
![几何元素间的继承关系](https://github.com/jihaoqin/CAM_Qt/blob/master/docs/%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB.png)
- 通过存储面片间的拓扑关系，实现跨面片的轨迹生成
- 通过图形界面对缠绕机形式进行配置
![](https://github.com/jihaoqin/CAM_Qt/blob/master/docs/%E6%9C%BA%E5%BA%8A%E9%85%8D%E7%BD%AE.png)
- 避障功能，能够检测执行器和模具、纤维和模具之间的碰撞关系，并给出避障算法
- 根据缠绕机形式生成G代码
- 针对西门子840Dsl控制的机器人缠绕系统生成运动代码

