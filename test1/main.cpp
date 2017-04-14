// @title: wsn课程作业1--dijkstra算法求无向图最短路径
// @author: MuPei
// @version: 1.0
// @data: 20170406
// @copyright: MuPei

// addition1
// @subtitle: 计时
// @purpose: linux与win测时，并预留斐波那契堆优化接口
// @author: MuPei
// @version: 2.0
// @data: 20170407

// addition2
// @subtitle: 重构1
// @purpose: 代码重构
// @author: MuPei
// @version: 2.1
// @data: 20170409

// addition3
// @subtitle: 重构2
// @purpose: 代码重构--按照赋最大值作为判断是否在集合S内，并减少了for次数进一步优化
// @author: MuPei
// @version: 2.2
// @data: 20170410

// addition4
// @subtitle: 重构3
// @purpose: 完整实现linux和win下程序实现与测时功能
// @author: MuPei
// @version: 2.3
// @data: 20170411

// addition5
// @subtitle: 重构4
// @purpose: 以C++的STL最小优先队列实现优化（虽然觉得好像看不出短甚至更长了）
// @author: MuPei
// @version: 2.4
// @data: 20170413

// addition5
// @subtitle: 重构5
// @purpose: 优化两种不同算法的函数格式和调用形式
// @author: MuPei
// @version: 2.5
// @data: 20170414

#include <stdio.h>
#include <iostream>
#include <memory.h>
#include<algorithm>
#include<queue>


#if defined(_WIN32) || defined(WIN32) // win
#define WINDOWS_IMPL
#include <Windows.h>
// #include <time.h> // time(), clock()
// #include <Mmsystem.h> // timeGetTime()
// #pragma comment(lib, "Winmm.lib")
#include <Winbase.h> // QueryPerformanceCounter()
#pragma comment(lib, "Kernel32.lib")
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(BSD) // linux
#define LINUX_IMPL
#include <sys/time.h> // gettimeofday()
#endif

using namespace std;

const int nodeNum = 6;
const int maxLen = 1000;

struct Node
{
	int num, val;   //存放结点编号和到初始点的距离 
};

//typedef int(*R)[6];
struct myGraph
{
    int subDist[nodeNum][nodeNum]; // 记录图的两点间路径长度
    int dist[nodeNum]; // 表示当前点到源点的最短路径长度
    int prevN[nodeNum];     // 记录当前点的前一个结点
};

/*
function:		实现dijkstra算法，包括从最小队列抽取最小点和松弛操作
parameter:
	beginNode	--起始节点id
	dist[]		--距源节点距离
	prevN[]		--记录当前节点的前一个节点
	subDist[][]	--任意两节点间距
return:			void
*/
myGraph Dijkstra(int beginNode, myGraph myG)
{
	int tmp = 0; // 
	int nextNode = 0;
	int ss = 0;
	int tmpLen = 0;
	int sSet[6]; // 存放已经找到的节点
	memset (sSet, -1, sizeof(sSet));
	sSet[0] = beginNode;
	
	while (ss != nodeNum-1) // 循环nodeNum-1次即找到了nodeNum-1个节点的最短路径
	{
		tmp = maxLen;
		// 两重循环遍历已在集合S内的点到其相邻节点的最短距离
		for (int i = 0; i <= ss; ++i)
		{
			for (int j = 1; j < nodeNum; ++j)
			{
				tmpLen = myG.dist[sSet[i]] + myG.subDist[sSet[i]][j] < myG.subDist[0][j] ? myG.dist[sSet[i]] + myG.subDist[sSet[i]][j] : myG.subDist[0][j]; // 这个距离是取得全局最优的算法关键，证明见《算法导论》367页
				if (tmpLen < tmp)
				{
					tmp = tmpLen;
					nextNode = j;
					myG.prevN[nextNode] = sSet[i];
				}
			}
		}
		
		// 将已在集合S内的点距源点距离改为最大值
		for (int k = 0; k < nodeNum; ++k)
		{
			myG.subDist[k][nextNode] = maxLen;
		}
		ss++;
		sSet[ss] = nextNode; // 将nextNode包含进集合S
		// 取得源点到当前点的最小距离
		myG.dist[nextNode] = tmp;
	}
    return myG;
}

/*
function:	查找从源点vNode到终点uNode的路径，并输出
parameter:
			prevN[n]--记录当前点的前一个节点
			vNode--源节点
			uNode--目的节点
return:		void
*/
void searchPath(myGraph myG, int vNode, int uNode)
{
	int que[nodeNum - 1]; // prevN是反向路径，这里倒一下是正向路径
	int id = uNode;
	int i = 0;
	while (id != vNode)
	{
		que[i] = id + 1;
		i++;
		id = myG.prevN[id];
	}
	cout << id + 1;

	for (i; i > 1; --i)
	{
		cout << " -> " << que[i - 1];
	}
	cout << " -> " << que[i - 1] << endl;
	
}

/*
function:	邻接算法主函数
parameter:	void
return:		void
*/
void matrixFun(myGraph myG)
{
	myG = Dijkstra(0, myG);

    for (int i = 1; i < nodeNum; ++i)
    {
        searchPath(myG, 0, i);
        cout << myG.dist[i] << endl;
    }
}

bool operator < (Node a, Node b)
{
	if (a.val == b.val)
	{
		return a.num > b.num;
	}
	return a.val>b.val;              //先出小 
}

void minQ(myGraph myG)
{
	priority_queue<Node> qq;;   //优先从小到大
	
	Node nod[nodeNum - 1];
	
	while (!qq.empty())
	{
		qq.pop(); //清空
	}

	nod[0].num = 0;
	nod[0].val = 0;

	qq.push (nod[0]);   //将起点放入队列 
	
	while (!qq.empty())  //不为空时 
	{
		int j = 0;
		for (int i = 1; i < nodeNum; ++i)
		{
			if (myG.subDist[qq.top().num][i] != -1 && myG.dist[i]>myG.dist[qq.top().num] + myG.subDist[qq.top().num][i])
			{
                myG.dist[i] = myG.dist[qq.top().num] + myG.subDist[qq.top().num][i]; // 更新距源点距离
				nod[j].num = i;
				nod[j].val = myG.dist[i]; // 将信息保存在nod中
				qq.push (nod[j]); // 放入优先队列
				myG.prevN[i] = qq.top().num;
			}
			
			j++;
		}

		qq.pop();
	}

/*	for(int i = 1; i < nodeNum; ++i)
	{
	cout << "初始点到" << i + 1 << "点的距离为：" << myG.dist[i] << endl;
	}
	

	for (int i = 2; i <= V; i++)
	{
		cout << nod[i].num << "->";
	}
	cout << endl;*/
    for (int i = 1; i < nodeNum; ++i)
    {
        searchPath(myG, 0, i);
        cout << myG.dist[i] << endl;
    }
}

myGraph designGraph(myGraph myG)
{
    for (int i = 0; i < nodeNum; ++i)
    {
        for (int j = 0; j < nodeNum; ++j)
        {
            myG.subDist[i][j] = maxLen;
            myG.dist[j] = maxLen; // 在这里把dist向量赋值
        }
    }

	myG.subDist[0][1] = 8;
    myG.subDist[0][2] = 3;
    myG.subDist[0][3] = 2;
    myG.subDist[1][3] = 1;
    myG.subDist[1][4] = 3;
    myG.subDist[2][3] = 5;
    myG.subDist[2][5] = 6;
    myG.subDist[3][4] = 2;
    myG.subDist[4][5] = 1;

    myG.subDist[1][0] = 8;
    myG.subDist[2][0] = 3;
    myG.subDist[3][0] = 2;
    myG.subDist[3][1] = 1;
    myG.subDist[4][1] = 3;
    myG.subDist[3][2] = 5;
    myG.subDist[5][2] = 6;
    myG.subDist[4][3] = 2;
    myG.subDist[5][4] = 1;

    myG.dist[0] = 0;

	return myG;
}

/*
function:	测程序运行时间，支持win/Linux/Mac/openBSD等平台
parameter:	void
return:		void
*/
void test_time_start(void)
{
	myGraph myG = {0, 0, 0};
	myG = designGraph(myG); // 记录图的信息
	myGraph myG1 = {0, 0, 0 };
	myG1 = designGraph(myG1); // 记录图的信息

	for (int i = 0; i < nodeNum; ++i)
	{
		for (int j = 0; j < nodeNum; ++j)
		{
			printf("%8d", myG.subDist[i][j]);
		}
		cout << endl;
	}

#ifdef WINDOWS_IMPL
	// QueryPerformanceCounter()
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus = 0;
	double dfFreq = 0;
	double dfTim = 0;
	QueryPerformanceFrequency(&litmp);
	dfFreq = litmp.QuadPart;
	
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	matrixFun(myG);
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq * 1000000;// 获得对应的时间值，单位为秒
	cout << "用时" << dfTim << "us" << endl;

	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	
	minQ(myG1);
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq * 1000000;// 获得对应的时间值，单位为秒
	cout << "用时" << dfTim << "us" << endl;
#elif defined(LINUX_IMPL)
	// gettimeofday()
	struct timeval tpstart, tpend;
	double timeuse;
	
	gettimeofday(&tpstart, NULL);
    matrixFun(myG);
	gettimeofday(&tpend, NULL);
	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;//注意，秒的读数和微秒的读数都应计算在内
	cout << "用时" << timeuse << "us" << endl;

	gettimeofday(&tpstart, NULL);
    minQ(myG1);
    gettimeofday(&tpend, NULL);
    timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;//注意，秒的读数和微秒的读数都应计算在内
    cout << "用时" << timeuse << "us" << endl;
#endif
}

int main(void)
{
	test_time_start();
	return 0;
}
