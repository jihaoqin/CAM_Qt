# CAM_Qt
这是一个基于Qt开发的用于CAD/CAM软件，用于三通管纤维缠绕的轨迹生成。
项目的演示视频，见  https://www.bilibili.com/video/BV1LE411c7n4/


## 项目亮点
- 项目架构为MVC, 将界面、控制和数据相关代码进行分离。
- 几何元素的继承关系, 本项目的几何元素如下图所示</br>
![几何元素间的继承关系](https://github.com/jihaoqin/CAM_Qt/blob/master/docs/%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB.png)
- 通过存储面片间的拓扑关系，实现跨面片的轨迹生成
- 通过图形界面对缠绕机形式进行配置</br>
![](https://github.com/jihaoqin/CAM_Qt/blob/master/docs/%E6%9C%BA%E5%BA%8A%E9%85%8D%E7%BD%AE.png)
- 避障功能，能够检测执行器和模具、纤维和模具之间的碰撞关系，并给出避障算法
- 根据缠绕机形式生成G代码，代码示例如下
```
;desktop
G90 F30000
#1 = X	;Z's offset
#2 = 92.943	;X's offset
#3 = -10.562	;A's offset
#4 = 0	;C's offset
G01
Z [#1+-44] X [#2+-50.0625] A [#3+180] C [#4+50.2019]
Z [#1+-44.4126] X [#2+-50.0542] A [#3+185.125] C [#4+67.7392]
```
- 针对西门子840Dsl控制的机器人缠绕系统生成运动代码，代码示例如下
```
;Comau4
G90
R1 = 350;offset between table and tee pipe 
$P_UIFR[1]=CTRANS(X,1838.936,Y,1007.022-R1,Z,893.659):CROT(X,0,Y,0,Z,0);workpiece coordinate system
ORIWKS
ORIVIRT1
TRAORI
G54
F2000
G1 X=-149.362 Y=-209 Z=0 A=0 B=0 C=0 MA1=-180 MB1=-78.8846
G1 X=-149.363 Y=-208.03 Z=0 A=0 B=0 C=0 MA1=-181.207 MB1=-76.2465
G1 X=-149.367 Y=-206.827 Z=0 A=0 B=0 C=0 MA1=-182.664 MB1=-73.0571
G1 X=-149.373 Y=-205.588 Z=0 A=0 B=0 C=0 MA1=-184.118 MB1=-69.8637
```

