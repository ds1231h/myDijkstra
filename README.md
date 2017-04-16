#dijkstra最短路径算法—WSN作业1


**dijkstra算法**在路由选择中颇为常用，现将之以**C语言**实现。

算法依据《算法导论》，以**邻接矩阵**实现。

具体算法分析和可行性证明见我的博客：

[1]半路算法之Dijkstra——以路由算法的角度进行理解:
http://blog.csdn.net/ds1231h/article/details/70144370

[2]dijkstra算法再整理:
http://blog.csdn.net/ds1231h/article/details/70182462

[3]半路算法之二项树与二项堆:
http://blog.csdn.net/ds1231h/article/details/70187963

程序中含有测时功能

算法效率：
linux(AMD A4-4300M APU, g++ 5.4.0)下以邻接矩阵实现时间为196us，STL的优先队列实现时间为221us。
windows7(i7-4710MQ, g++ 5.3.0)下以邻接矩阵实现时间为1525us，STL的优先队列实现时间为2765us。



Copyright(C) 2017 MuPei
