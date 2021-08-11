#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include<io.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
                                                                        //vs2019自定义控制台代码模板 by mao
                                                                        //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0<< "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%"<<endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;
	
}

/*   ****************************************
void FpsTime(const time_t* t0, time_t* t1, const time_t* t2)// 图形界面计算显示FPS和运行时间
{
	if (!t0 || !t1 || !t2)
		return;
	static int FPS = 0;
	static int fps = 0;
	static wstring fpsTime;
	if (*t2 - *t1 == 1)
	{
		FPS = ++fps;
	}
	fpsTime = L"FPS: ";
	fpsTime += to_wstring(FPS);
	fpsTime += L" 运行时间: ";
	fpsTime += to_wstring(*t2 - *t0);
	fpsTime += L"s";
	settextcolor(LIGHTMAGENTA);
	outtextxy(0, 0, fpsTime.c_str());

	if (*t2 - *t1 == 1)
	{
		fps = 0;
		*t1 = time(nullptr);
	}
	else
	{
		fps++;
	}
}
*/    //*************************************
/*bool cmp(const int a, const int b)
{
	return a > b;
}*/
/*    ***************************************
bool ChangeDisSize(int cx, int cy)	                                  // 分辨率设置为 cx × cy，退出程序时自动还原
{
	LPDIRECTDRAW DXW;
	HRESULT ddrval;
	ddrval = DirectDrawCreate(NULL, &DXW, NULL);
	if (DD_OK != DD_OK) return FALSE;	                         // 创建DirectDraw对象失败返回
	ddrval = DXW->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (DD_OK != DD_OK) return FALSE;	                         // 取得窗口模式失败返回
	ddrval = DXW->SetDisplayMode(cx, cy, 16);
	if (DD_OK != DD_OK) return FALSE;	                          // 设置显示模式失败返回
}
*/
//**************************************************************//************************非主函数


#define MAKEUS(a, b)    ((unsigned short) ( ((unsigned short)(a))<<8 | ((unsigned short)(b)) ))
#define MAKEUI(a,b,c,d) ((unsigned int) ( ((unsigned int)(a)) << 24 | ((unsigned int)(b)) << 16 | ((unsigned int)(c)) << 8 | ((unsigned int)(d)) ))

#define M_DATA  0x00
#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_SOI   0xd8
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DNL   0xdc
#define M_DRI   0xdd
#define M_APP0  0xe0
#define M_APPF  0xef
#define M_COM   0xfe
int GetPNGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight)
{
	int Finished = 0;
	unsigned char uc[4];
	FILE* pfRead;

	*punWidth = 0;
	*punHeight = 0;

	if (fopen_s(&pfRead, path, "rb") != 0)
	{
		printf("[GetPNGWidthHeight]:can't open file:%s\n", path);
		return -1;
	}

	for (int i = 0; i < 4; i++)
		fread(&uc[i], sizeof(unsigned char), 1, pfRead);
	if (MAKEUI(uc[0], uc[1], uc[2], uc[3]) != 0x89504e47)
		printf("[GetPNGWidthHeight]:png format error\n", path);
	for (int i = 0; i < 4; i++)
		fread(&uc[i], sizeof(unsigned char), 1, pfRead);
	if (MAKEUI(uc[0], uc[1], uc[2], uc[3]) != 0x0d0a1a0a)
		printf("[GetPNGWidthHeight]:png format error\n", path);

	fseek(pfRead, 16, SEEK_SET);
	for (int i = 0; i < 4; i++)
		fread(&uc[i], sizeof(unsigned char), 1, pfRead);
	*punWidth = MAKEUI(uc[0], uc[1], uc[2], uc[3]);
	for (int i = 0; i < 4; i++)
		fread(&uc[i], sizeof(unsigned char), 1, pfRead);
	*punHeight = MAKEUI(uc[0], uc[1], uc[2], uc[3]);

}

int GetJPEGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight)
{
	int Finished = 0;
	unsigned char id, ucHigh, ucLow;
	FILE* pfRead;

	*punWidth = 0;
	*punHeight = 0;

	if (fopen_s(&pfRead, path, "rb") != 0)
	{
		printf("[GetJPEGWidthHeight]:can't open file:%s\n", path);
		return -1;
	}

	while (!Finished)
	{
		if (!fread(&id, sizeof(char), 1, pfRead) || id != 0xff || !fread(&id, sizeof(char), 1, pfRead))
		{
			Finished = -2;
			break;
		}

		if (id >= M_APP0 && id <= M_APPF)
		{
			fread(&ucHigh, sizeof(char), 1, pfRead);
			fread(&ucLow, sizeof(char), 1, pfRead);
			fseek(pfRead, (long)(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR);
			continue;
		}

		switch (id)
		{
		case M_SOI:
			break;

		case M_COM:
		case M_DQT:
		case M_DHT:
		case M_DNL:
		case M_DRI:
			fread(&ucHigh, sizeof(char), 1, pfRead);
			fread(&ucLow, sizeof(char), 1, pfRead);
			fseek(pfRead, (long)(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR);
			break;

		case M_SOF0:
			fseek(pfRead, 3L, SEEK_CUR);
			fread(&ucHigh, sizeof(char), 1, pfRead);
			fread(&ucLow, sizeof(char), 1, pfRead);
			*punHeight = (unsigned int)MAKEUS(ucHigh, ucLow);
			fread(&ucHigh, sizeof(char), 1, pfRead);
			fread(&ucLow, sizeof(char), 1, pfRead);
			*punWidth = (unsigned int)MAKEUS(ucHigh, ucLow);
			return 0;

		case M_SOS:
		case M_EOI:
		case M_DATA:
			Finished = -1;
			break;

		default:
			fread(&ucHigh, sizeof(char), 1, pfRead);
			fread(&ucLow, sizeof(char), 1, pfRead);
			printf("[GetJPEGWidthHeight]:unknown id: 0x%x ;  length=%hd\n", id, MAKEUS(ucHigh, ucLow));
			if (fseek(pfRead, (long)(MAKEUS(ucHigh, ucLow) - 2), SEEK_CUR) != 0)
				Finished = -2;
			break;
		}
	}

	if (Finished == -1)
		printf("[GetJPEGWidthHeight]:can't find SOF0!\n");
	else if (Finished == -2)
		printf("[GetJPEGWidthHeight]:jpeg format error!\n");
	return -1;
}

void GetPicWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight)
{
	int len = strlen(path);
	if (len <= 4)
		printf("[GetPicWidthHeight]:picture name is too short\n");
	if (!strncmp(path + len - 3, "jpg", 3))
		GetJPEGWidthHeight(path, punWidth, punHeight);
	else if (!strncmp(path + len - 3, "png", 3))
		GetPNGWidthHeight(path, punWidth, punHeight);
	else
		printf("[GetPicWidthHeight]:only support jpg and png\n");
}

// 显存指针
DWORD* g_pBuf;

// 快速画点函数
  void fast_putpixel(int x, int y,unsigned int WIDTH, COLORREF c)
{
	g_pBuf[y * WIDTH + x] = BGR(c);
}

// 快速读点函数
COLORREF fast_getpixel(int x, int y, unsigned int WIDTH)
{
	COLORREF c = g_pBuf[y * WIDTH + x];
	return BGR(c);
}

//**************************************************************//************************

int main()
{
	/*   *************************************
	constexpr int MIN = 1;                            //随机数
	constexpr int MAX = 100;
	constexpr int numm = 10;
	 random_device rd;
	default_random_engine eng(rd());
	uniform_int_distribution<int> distr(MIN, MAX);

	for (int n = 0; n < numm; ++n)
	{
		cout << distr(eng) << ",";
	}
	*/   //***********************************
	/*  //************************************
	SYSTEMTIME sys;                                    //获取系统时间
	GetLocalTime(&sys);
	cout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      ";
	cout<< setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << endl;
	*/   //***********************************
	/*   *************************************                                             //全屏图形窗口模板
	int rw, rh;
	rw = GetSystemMetrics(SM_CXSCREEN);                                          // 屏幕宽度 像素
	rh = GetSystemMetrics(SM_CYSCREEN);                                          // 屏幕高度 像素
	HWND hwnd = initgraph(rw, rh);                                                    // 初始化绘图窗口并获取窗口句柄
	setbkcolor(GREEN);
	cleardevice();
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);               // 获取窗口信息
																										 // 设置窗口信息 最大化 取消标题栏及边框
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);           // 修改窗口样式
	SetWindowPos(hwnd, HWND_TOP, 0, 0, rw, rh, 0);
	*/   //***********************************
	/*      ***********************************
	initgraph(1600, 900);
	setbkcolor(WHITE);
	cleardevice();
	*/  //***********************************
	/*    ***********************************                                             //后台方式运行，Unicode字符集改成多字节字符集
	HWND hwnd; if (hwnd = ::FindWindow("ConsoleWindowClass", NULL)) //找到控制台句柄
	{
		::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
	}
                                                                                                    //unicode字符集下：
	const char* sss = "ConsoleWindowClass";
	WCHAR wszName[200];
	memset(wszName, 0, sizeof(wszName));
	MultiByteToWideChar(CP_ACP, 0, sss, strlen(sss) + 1, wszName,
		sizeof(wszName) / sizeof(wszName[0]));
	HWND hwnd; if (hwnd = ::FindWindow(wszName, NULL)) //找到控制台句柄
	{
		::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
	}
	*/  //**********************************

	//*************************************************************************

	int n;
	cout << "1.图片转txt          2.txt转图片          3.txt转图片（调用显存指针）" << endl;
	cout << "请选择:";
	cin >> n;
	if (n == 1)
	{
		char s[100];
		TCHAR s1[200];
		cout << "请输入文件名:" << endl;
		cin >> s;

		//*************************************************************************
		runstart();
		//*************************计时开始***************************************

		unsigned int unWidth = 0, unHeight = 0;

		cout <<"路径：" << s << endl;
		GetPicWidthHeight(s, &unWidth, &unHeight);
		printf("宽度： %u\n", unWidth);
		printf("高度： %u\n", unHeight);
		if (unWidth == 0 && unHeight == 0)
		{
			//cout << "没找到" << s << endl;
			system("pause");
			exit(0);
		}
		if (unWidth >= 20000 && unHeight >= 20000)
		{
			cout << "异常" << s << endl;
			system("pause");
			exit(0);
		}
		MultiByteToWideChar(CP_ACP, 0, s, -1, s1, 200);
		//wcout << s1 << endl;
		//_getch();
		initgraph(unWidth, unHeight, 1);
		loadimage(NULL, s1);
		int a = 0;
		ofstream out("photo.txt", ios::out);
		if (!out)
		{
			cout << "未找到photo.txt" << endl;
			exit(0);
		}
		out << unHeight << "      " << unWidth << endl;
		cout << endl;
		for (int i = 0; i < unHeight; i++)
		{
			cout <<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b进度：" << i << "/" << unHeight << "     "<<i*100/unHeight<<"%";
			for (int j = 0; j < unWidth; j++)
			{
				a = getpixel(j, i);
				//cout <<hex<< a << " ";
				out << hex << a << " ";
			}
		}
		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b进度：" << unHeight << "/" << unHeight << "     "<<100<<"%";
		cout << endl << "完成，已写入photo.txt文件" << endl;
		out.close();
		closegraph();


		//*************************计时结束***************************************
		runend();
	}
	else if (n == 2)
	{
		runstart();
		ifstream in("photo.txt", ios::in);
		if (!in)
		{
			cout << "没有在相对路径下找到photo.txt文件" << endl;
			system("pause");
			exit(0);
		}
		int w=0, h=0;
		int a = 0;
		in >> h >> w;
		initgraph(w, h, 1);
		IMAGE img(w, h);
		//SetWorkingImage(&img);
		for (int i = 0; i < h; i++)
		{
			for (int j = 0;j < w;j++)
			{
				in >>hex>> a;
				putpixel(j, i, a);
			}
		}
		in.close();
		
		getimage( &img,0,0,w,h);
		saveimage(_T("testout.jpg"), &img);
		cout << "testout.jpg图片已保存在相对路径下" << endl;
		closegraph();
		runend();
	}

	else if (n == 3)
	{
		runstart();
		ifstream in("photo.txt", ios::in);
		if (!in)
		{
			cout << "没有在相对路径下找到photo.txt文件" << endl;
			system("pause");
			exit(0);
		}
		int w = 0, h = 0;
		int a = 0;
		in >> h >> w;
		initgraph(w, h, 1);
		IMAGE img(w, h);
		//SetWorkingImage(&img);
		g_pBuf = GetImageBuffer();
		for (int i = 0; i < h; i++)
		{
			for (int j = 0;j < w;j++)
			{
				in >> hex >> a;
				fast_putpixel(j, i,w, a);
			}
		}
		in.close();

		getimage(&img, 0, 0, w, h);
		saveimage(_T("testout.jpg"), &img);
		cout << "testout.jpg图片已保存在相对路径下" << endl;
		closegraph();
		runend();
	}


	/*   ************************************                            //微软TTS语音模板
	ISpVoice* pVoice = NULL;                                            //创建一个ISpVoice的空指针
	if (FAILED(::CoInitialize(NULL)))                                     //初始化COM编程环境
	{
		cout << "初始化COM环境失败！" << endl;
		return 0;
	}
	                                                                                   //获取ISpVoice接口对象：
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (SUCCEEDED(hr))                                                      //如果获取接口对象成功
	{
		char* szStr = (char*)malloc(200);
		sprintf(szStr, "你好，世界。");
		WCHAR wszClassName[200];
		memset(wszClassName, 0, sizeof(wszClassName));
		MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
			sizeof(wszClassName) / sizeof(wszClassName[0]));
		hr = pVoice->Speak(wszClassName, 0, NULL);         //朗读
		
		pVoice->Release();                                                 //释放分配资源
		pVoice = NULL;                                                     //防止野指针
	}
	::CoUninitialize();                                                       //退出COM编程环境
	*/  //*********************************
	/*   ************************************                                  //自定义控制台输出位置
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };                                           //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	COORD     cursorPos;                                                       //标准输出句柄
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorPos.X = 0;
	cursorPos.Y = csbi.dwCursorPosition.Y + 1;
	SetConsoleCursorPosition(hStdout, cursorPos);
	cout << "第1行" << endl;
	*/  //*********************************
	/*   **********************************                          //多线程模板
	void task()
    {
	//多线程代码
    }
	thread t(task);
	t.detach();
	*/   //*******************************
	rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
