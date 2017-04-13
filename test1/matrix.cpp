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
// @purpose: 代码重构--按照赋最大值作为relax，并减少了for次数进一步优化
// @author: MuPei
// @version: 2.2
// @data: 20170410

// addition4
// @subtitle: 重构3
// @purpose: 完整实现linux和win下程序实现与测时功能
// @author: MuPei
// @version: 2.3
// @data: 20170411

#include <stdio.h>
#include <iostream>
#include <memory.h>

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

/*
function:		实现dijkstra算法，包括从最小队列抽取最小点和松弛操作
parameter:
	beginNode	--起始节点id
	dist[]		--距源节点距离
	prevN[]		--记录当前节点的前一个节点
	subDist[][]	--任意两节点间距
return:			void
*/
void Dijkstra(int beginNode, int *dist, int *prevN, int subDist[6][6])
{
	int tmp = 0; // 
	int nextNode = 0;
	int ss = 0;
	int tmpLen = 0;
	int sSet[6]; // 存放已经找到的节点
	memset(sSet, -1, sizeof(sSet));
	sSet[0] = beginNode;
	
	while (ss != nodeNum-1) // 循环nodeNum-1次即找到了nodeNum-1个节点的最短路径
	{
		tmp = maxLen;
		// 两重循环遍历已在集合S内的点到其相邻节点的最短距离
		for (int i = 0; i <= ss; ++i)
		{
			for (int j = 1; j < nodeNum; ++j)
			{
				tmpLen = dist[sSet[i]] + subDist[sSet[i]][j] < subDist[0][j] ? dist[sSet[i]] + subDist[sSet[i]][j] : subDist[0][j]; // 这个距离是取得全局最优的算法关键，证明见《算法导论》367页
				if (tmpLen < tmp)
				{
					tmp = tmpLen;
					nextNode = j;
					prevN[nextNode] = sSet[i];
				}
			}
		}
		
		// 松弛操作：通过赋最大值的方式将当前点去掉
		for (int k = 0; k < nodeNum; ++k)
		{
			subDist[k][nextNode] = maxLen;
		}
		ss++;
		sSet[ss] = nextNode; // 将nextNode包含进集合S
		// 取得源点到当前点的最小距离
		dist[nextNode] = tmp;
	}
}

/*
function:	查找从源点vNode到终点uNode的路径，并输出
parameter:
			prevN[n]--记录当前点的前一个节点
			vNode--源节点
			uNode--目的节点
return:		void
*/
void searchPath(int *prevN, int vNode, int uNode)
{
	// 用数组que保存一条路径信息
	int que[nodeNum];
	memset(que, 0, sizeof(que));
	int tot = 0;
	que[tot] = uNode;
	tot++;
	int tmp = prevN[uNode];
	// 逐次从vNode连接到nNode
	while (tmp != vNode)
	{
		que[tot] = tmp;
		tmp = prevN[tmp];
		tot++;
	}
	que[tot] = vNode;
	for (int i = tot; i > -1; --i)
		if (i != 0)
			cout << que[i]+1 << " -> ";
		else
			cout << que[i]+1 << endl;
}

/*
function:	算法主函数
parameter:	void
return:		void
*/
void matrixFun(void)
{
	int dist[nodeNum];     // 表示当前点到源点的最短路径长度
	int prevN[nodeNum];     // 记录当前点的前一个结点
	// 初始化subDist[][]为maxLen
	int subDist[nodeNum][nodeNum]; // 记录图的两点间路径长度
	for (int i = 0; i < nodeNum; ++i)
	{
		for (int j = 0; j < nodeNum; ++j)
		{
			subDist[i][j] = maxLen;
			dist[j] = maxLen; // 在这里把dist向量赋值		
		}
	}
	dist[0] = 0;

	subDist[0][1] = 8;
	subDist[0][2] = 3;
	subDist[0][3] = 2;
	subDist[1][3] = 1;
	subDist[1][4] = 3;
	subDist[2][3] = 5;
	subDist[2][5] = 6;
	subDist[3][4] = 2;
	subDist[4][5] = 1;

	subDist[1][0] = 8;
	subDist[2][0] = 3;
	subDist[3][0] = 2;
	subDist[3][1] = 1;
	subDist[4][1] = 3;
	subDist[3][2] = 5;
	subDist[5][2] = 6;
	subDist[4][3] = 2;
	subDist[5][4] = 1;

	for (int i = 0; i < nodeNum; ++i)
	{
		for (int j = 0; j < nodeNum; ++j)
		{
			// cout << subDist[i][j];
			printf("%8d", subDist[i][j]);
		}
		cout << endl;
	}

	Dijkstra(0, dist, prevN, subDist);

	searchPath(prevN, 0, 1);
	cout << dist[1] << endl;
	searchPath(prevN, 0, 2);
	cout << dist[2] << endl;
	searchPath(prevN, 0, 3);
	cout << dist[3] << endl;
	searchPath(prevN, 0, 4);
	cout << dist[4] << endl;
	searchPath(prevN, 0, 5);
	cout << dist[5] << endl;
}

/*
function:	测程序运行时间，支持win/Linux/Mac/openBSD等平台
parameter:	void
return:		void
*/
void test_time_start(void)
{
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
	matrixFun();
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
	matrixFun();
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
