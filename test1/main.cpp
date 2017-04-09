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
int dist[nodeNum];     // 表示当前点到源点的最短路径长度
int prevN[nodeNum];     // 记录当前点的前一个结点

void Dijkstra(int nodeNum, int v, int *dist, int *prevN, int c[6][6])
{
	bool *sSet = new bool[nodeNum];    // 判断是否已存入该点到S集合中
	memset(sSet, false, sizeof(sSet));

	for (int i = 1; i <= nodeNum; ++i)
	{
		dist[i] = c[v][i];
		if (dist[i] == maxLen)
			prevN[i] = 0;
		else
			prevN[i] = v;
	}
	dist[v] = 0;
	sSet[v] = 1;

	// 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
	// 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
	// 注意是从第二个节点开始，第一个为源点
	for (int i = 2; i <= nodeNum; ++i)
	{
		int tmp = maxLen;
		int u = v;
		// 找出当前未使用的点j的dist[j]最小值
		for (int j = 1; j <= nodeNum; ++j)
			if ((!sSet[j]) && dist[j]<tmp)
			{
				u = j;              // u保存当前邻接点中距离最小的点的号码
				tmp = dist[j];
			}
		sSet[u] = 1;    // 表示u点已存入S集合中

						// 更新dist
		for (int j = 1; j <= nodeNum; ++j)
			if ((!sSet[j]) && c[u][j]<maxLen)
			{
				int newdist = dist[u] + c[u][j];
				if (newdist < dist[j])
				{
					dist[j] = newdist;
					prevN[j] = u;
				}
			}
	}
}

/*
function: 查找从源点vNode到终点uNode的路径，并输出
parameter:
prevN[n]--记录当前点的前一个节点
vNode--源节点
uNode--目的节点
return: void
*/
void searchPath(int *prevN, int vNode, int uNode)
{
	// 用数组que保存一条路径信息
	int que[nodeNum];
	memset(que, 0, sizeof(que));
	int tot = 1;
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
	for (int i = tot; i >= 1; --i)
		if (i != 1)
			cout << que[i] << " -> ";
		else
			cout << que[i] << endl;
}

void matrixFun(void)
{
	// 初始化subDist[][]为maxLen
	int subDist[nodeNum][nodeNum];// 记录图的两点间路径长度
	for (int i = 0; i < nodeNum; ++i)
	{
		for (int j = 0; j < nodeNum; ++j)
		{
			subDist[i][j] = maxLen;
		}
	}
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
		dist[i] = maxLen;
	for (int i = 0; i < nodeNum; ++i)
	{
		for (int j = 0; j < nodeNum; ++j)
			printf("%8d", subDist[i][j]);
		printf("\n");
	}

	Dijkstra(nodeNum, 0, dist, prevN, subDist);

	searchPath(prevN, 0, 1);
	cout << dist[1] << "\n";
	searchPath(prevN, 0, 2);
	cout << dist[2] << "\n";
	searchPath(prevN, 0, 3);
	cout << dist[3] << "\n";
	searchPath(prevN, 0, 4);
	cout << dist[4] << "\n";
	searchPath(prevN, 0, 4);
	cout << dist[5] << "\n";
	cout << endl;
}
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
