#undef UNICODE
#undef _UNICODE
#include "Windows.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
using namespace std;

HWND hWnd;
HWND button;
HWND outputWindow;
HWND inputFrequency;
HWND stopAt;
HWND triggerButton;
HWND stopButton;
HWND resetButton;
HWND helpButton;
HWND groupBox;
HWND toggle;
HWND press;
HWND statusText;
HWND rmlGroupBox;
HWND rightM;
HWND middleM;
HWND leftM;
HWND numberClicks;
HWND clicksPerSecond;
HWND triggerKey;
HWND clicksStopAt;

WNDCLASS windClass;
HINSTANCE hInstance;

bool quit=false;
bool switchFlag = true;
int numClicks = 0;
int numClicksSinceStop = 0;
bool doToggle = false;
unsigned char toggleState = 0;
bool waitingForTrigger = false;
int status = 0;
int prevStatus = 0;
bool clickedOnceForTriggerFlag = false;
int mouseToClick = 0; //0=left 1=middle 2=right;
bool sameTriggerAndClick = false;
bool waitingForTriggerUp = false;

unsigned char keyDown[VK_OEM_CLEAR];
unsigned char keyPrev[VK_OEM_CLEAR];
unsigned char keyTrig[VK_OEM_CLEAR];
unsigned char keyUpTrig[VK_OEM_CLEAR];

#define TRIGGER_BTN 1000
#define STOP_BTN 2000
#define INPUT_TEXT 3000
#define OUTPUT_TEXT 4000
#define RESET_BTN 4500
#define HELP_BTN 5000

#define T_P_GROUP 6000
#define R_M_L_GROUP 7000

#define STOP_AT_TEXT 8000


LARGE_INTEGER countsOnLastFrame;
LARGE_INTEGER currentCounts;
double frameTime = 0.0;
double frequency = 100.0;
int stopAtNum = 0;
char triggerText[4]="13";
double countsToSeconds(__int64 a)
{
	LARGE_INTEGER temp;
	QueryPerformanceFrequency(&temp); // = counts/second
	return (double)(double(a)/double(temp.QuadPart));
}


LRESULT CALLBACK winCallBack(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void handleMessages()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		
	{
		TranslateMessage(&msg);
		DispatchMessage	(&msg);
	}
}

HFONT s_hFont = NULL;
HFONT s_hFontBold = NULL;

void my_mouse_event(_In_ DWORD     dwFlags,
					_In_ DWORD     dx,
					_In_ DWORD     dy,
					_In_ DWORD     dwData,
					_In_ ULONG_PTR dwExtraInfo,
					_In_ UINT nCnt = 1)
{
	INPUT input[4096];
	input[0].type = INPUT_MOUSE;
	input[0].mi.dx = dx;
	input[0].mi.dy = dy;
	input[0].mi.dwFlags = dwFlags;
	input[0].mi.mouseData = dwData;
	input[0].mi.time = 0;
	input[0].mi.dwExtraInfo = dwExtraInfo;

	if (nCnt > 1)
	{
		nCnt = 1 + (nCnt - 1) * 2;

		for (UINT i = 1; i < nCnt; ++i)
		{
			input[i].type = input[0].type;
			input[i].mi.dx = input[0].mi.dx;
			input[i].mi.dy = input[0].mi.dy;
			input[i].mi.dwFlags = input[0].mi.dwFlags;
			input[i].mi.mouseData = input[0].mi.mouseData;
			input[i].mi.time = input[0].mi.time;
			input[i].mi.dwExtraInfo = input[0].mi.dwExtraInfo;

			// Replace all down by up

			if (input[0].mi.dwFlags & MOUSEEVENTF_LEFTDOWN)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_LEFTUP;
			}

			if (input[0].mi.dwFlags & MOUSEEVENTF_MIDDLEDOWN)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_MIDDLEUP;
			}

			if (input[0].mi.dwFlags & MOUSEEVENTF_RIGHTDOWN)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_RIGHTUP;
			}

			// Replace all up by down

			if (input[0].mi.dwFlags & MOUSEEVENTF_LEFTUP)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_LEFTDOWN;
			}

			if (input[0].mi.dwFlags & MOUSEEVENTF_MIDDLEUP)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_MIDDLEDOWN;
			}

			if (input[0].mi.dwFlags & MOUSEEVENTF_RIGHTUP)
			{
				if ((i % 2) == 1)
					input[i].mi.dwFlags|= MOUSEEVENTF_RIGHTDOWN;
			}
		}
	}

	UINT ret = SendInput(nCnt, input, sizeof(INPUT));

	if (ret != nCnt)
	{
		SetDlgItemText(hWnd,GetDlgCtrlID(statusText),"status (read-only): error in SendInput()");
	}
}

enum Mode
{
	Mode_Press
	, Mode_Toggle
};

enum Button
{
	Button_Left
	, Button_Middle
	, Button_Right
};

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

std::string erase_multiple_spaces(const std::string& str_in)
{
	std::string str = str_in;

	std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
	str.erase(new_end, str.end());

	return str;
}

bool has_only_digits(const std::string& s)
{
	return s.find_first_not_of("0123456789") == std::string::npos;
}

bool has_only_digits_dot(const std::string& s)
{
	 std::string::size_type n = s.find_first_not_of("0123456789");

	 if (n != std::string::npos)
		 return (s[n] == '.') || (s[n] == ' ');
}

void parse_command_line(LPCSTR command_line, double& clicks_per_second, int& trigger_key, int& stop_at, Mode& mode, Button& button)
{
	std::vector<std::string> parameters = split(erase_multiple_spaces(command_line), ' ');

	for (size_t i = 0; i < parameters.size(); i+= 2)
	{
		if (parameters[i] == "-c")
		{
			if ((i + 1) < parameters.size())
			{
				if (has_only_digits_dot(parameters[i + 1]))
				{
					clicks_per_second = atof(parameters[i + 1].c_str());
				}
			}
		}

		if (parameters[i] == "-t")
		{
			if ((i + 1) < parameters.size())
			{
				if (has_only_digits(parameters[i + 1]))
				{
					trigger_key = atoi(parameters[i + 1].c_str());
				}
			}
		}

		if (parameters[i] == "-s")
		{
			if ((i + 1) < parameters.size())
			{
				if (has_only_digits(parameters[i + 1]))
				{
					stop_at = atoi(parameters[i + 1].c_str());
				}
			}
		}

		if (parameters[i] == "-m")
		{
			if ((i + 1) < parameters.size())
			{
				if (parameters[i + 1] == "p")
				{
					mode = Mode_Press;
				}

				if (parameters[i + 1] == "t")
				{
					mode = Mode_Toggle;
				}
			}
		}

		if (parameters[i] == "-b")
		{
			if ((i + 1) < parameters.size())
			{
				if (parameters[i + 1] == "l")
				{
					button = Button_Left;
				}

				if (parameters[i + 1] == "m")
				{
					button = Button_Middle;
				}

				if (parameters[i + 1] == "r")
				{
					button = Button_Right;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

static char g_appRoot[MAX_PATH];

bool fillAppDataPath(char szPath[MAX_PATH], const char* subFile)
{
	if (g_appRoot[0] != 0)
	{
		strcpy(szPath, g_appRoot);
		size_t ilen = strlen(szPath);
		strcpy(&szPath[ilen], subFile);
		return true;
	}

	DWORD nModPathSize = ::GetModuleFileNameA(NULL, szPath, MAX_PATH);

	if (nModPathSize == 0)
		return false;

	if (nModPathSize >= MAX_PATH)
		return false;

	if (g_appRoot[0] == 0)
	{
		size_t pLast = nModPathSize - 1;
		while (szPath[pLast] != '\\')
			--pLast;

		memcpy(g_appRoot, szPath, pLast);
		g_appRoot[pLast] = 0;
	}

	if (subFile == NULL)
		return true; // this running executable pathname was requested

	return fillAppDataPath(szPath, subFile);
}

void loadSubFileToMem(const char* subFile, char*& pBuffer, size_t& szBuffer)
{
	pBuffer = NULL; szBuffer = 0;

	char lszPath[MAX_PATH];

	if (!fillAppDataPath(lszPath, subFile))
		return;

	FILE* fp = fopen(lszPath, "rb");

	if (fp == NULL)
		return;

	int n = fseek(fp, 0, SEEK_END);

	if (n < 0)
		return;

	size_t nFileSize = (size_t)ftell(fp);

	n = fseek(fp, 0, SEEK_SET);

	if (n < 0)
		return;

	if (nFileSize > 1024)
		return;

	pBuffer = new char[1024];

	memset(pBuffer, 0, 1024);

	size_t nsiz = fread_s(pBuffer, 1024, nFileSize, 1, fp);

	if (nsiz != 1)
	{
		delete[] pBuffer;
		pBuffer = NULL;
		return;
	}

	szBuffer = nFileSize;
}

bool saveMemToSubFile(const char* subFile, char*& pBuffer, size_t szBuffer)
{
	char lszPath[MAX_PATH];

	if (!fillAppDataPath(lszPath, subFile))
		return false;

	FILE* fp = fopen(lszPath, "wb");

	if (fp == NULL)
		return false;

	size_t nwr = fwrite(pBuffer, szBuffer, 1, fp);

	if (nwr != 1)
		return false;

	delete[] pBuffer;
	pBuffer = 0;

	return true;
}

void _ftoa_s(double val, char* buf, size_t sz, int)
{
	int n = _sprintf_p(buf, sz, "%5.2f", val);
	int m = n;
}

//////////////////////////////////////////////////////////////////////////////////////////////

static bool s_bHasCommandLine = false;

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	memset(g_appRoot, 0, MAX_PATH);

	double my_clicks_per_second = 10;
	int my_trigger_key = 0x0D;
	int my_stop_at = 0;
	Mode my_mode = Mode_Press;
	Button my_button = Button_Left;

	char* my_pBuffer = NULL;
	size_t my_szBuffer = 0;

	loadSubFileToMem("\\settings.dat", my_pBuffer, my_szBuffer);

	if ((my_pBuffer != NULL) && (my_szBuffer == 1024))
		parse_command_line(my_pBuffer, my_clicks_per_second, my_trigger_key, my_stop_at, my_mode, my_button);

	if (strlen(command_line) >= 4)
	{
		parse_command_line(command_line, my_clicks_per_second, my_trigger_key, my_stop_at, my_mode, my_button);
		s_bHasCommandLine = true;
	}

	//copy-pasta windows stuff below
	hInstance=instanceH;
	// Initializing the window class
	windClass.style			= CS_HREDRAW | CS_VREDRAW;
	windClass.lpfnWndProc		= winCallBack;
	windClass.cbClsExtra		= 0;
	windClass.cbWndExtra		= 0;
	windClass.hInstance		= instanceH;
	windClass.hIcon			= LoadIcon(windClass.hInstance,MAKEINTRESOURCE(101));
	windClass.hCursor			= LoadCursor(NULL,IDC_ARROW);
	windClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	windClass.lpszClassName	= "The Fastest Mouse Clicker for Windows";

	//Registering the window class
	RegisterClass(&windClass);

	hWnd=CreateWindow("The Fastest Mouse Clicker for Windows","The Fastest Mouse Clicker for Windows", WS_OVERLAPPEDWINDOW, 100, 100,438,446, NULL, NULL, instanceH, NULL);

	statusText = CreateWindow("Static","status (read-only): idle",WS_VISIBLE|WS_CHILD,5,1,410,35,hWnd,0,0,0);
	numberClicks = CreateWindow("Static","# clicks (read-only)",WS_VISIBLE|WS_CHILD,5,40,410,20,hWnd,0,0,0);
	clicksPerSecond = CreateWindow("Static","clicks/s",WS_VISIBLE|WS_CHILD,5,60,410,20,hWnd,0,0,0);
	triggerKey = CreateWindow("Static","trigger key",WS_VISIBLE|WS_CHILD,5,80,410,20,hWnd,0,0,0);
	clicksStopAt = CreateWindow("Static","stop at",WS_VISIBLE|WS_CHILD,5,100,410,20,hWnd,0,0,0);

	char numStrInputFrequency[9];
	_ftoa_s(my_clicks_per_second, numStrInputFrequency, 9, 10);

	char numStrStopAt[6];
	_itoa_s(my_stop_at, numStrStopAt, 6, 10);

	char numStrTriggerButton[4];
	_itoa_s(my_trigger_key, numStrTriggerButton, 4, 10);
	_itoa_s(my_trigger_key, triggerText, 4, 10);

	outputWindow =		CreateWindow("Edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_NUMBER, 170, 40, 80, 20, hWnd, (HMENU)OUTPUT_TEXT, 0, 0);
	inputFrequency =	CreateWindow("Edit", numStrInputFrequency, WS_VISIBLE | WS_CHILD | WS_BORDER, 170, 60, 80, 20, hWnd, (HMENU)INPUT_TEXT, 0, 0);
	triggerButton =		CreateWindow("Button", numStrTriggerButton, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 190, 80, 40, 20, hWnd, (HMENU)TRIGGER_BTN, 0, 0);
	stopAt =			CreateWindow("Edit", numStrStopAt, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 170, 100, 80, 20, hWnd, (HMENU)STOP_AT_TEXT, 0, 0);
	stopButton =		CreateWindow("Button","STOP!",WS_VISIBLE | WS_CHILD,5,125,410,50,hWnd,(HMENU)STOP_BTN,0,0);
	resetButton =		CreateWindow("Button","Reset to defaults", WS_VISIBLE | WS_CHILD, 5, 180, 410/2, 50, hWnd, (HMENU)RESET_BTN, 0, 0);
	helpButton =		CreateWindow("Button","Help",WS_VISIBLE | WS_CHILD,5+410/2,180,410/2,50,hWnd,(HMENU)HELP_BTN,0,0);

	groupBox = CreateWindow("Button","trigger key mode",WS_VISIBLE | WS_CHILD | BS_GROUPBOX,5,240,410,65,hWnd,(HMENU)T_P_GROUP,0,0);
	press = CreateWindow("Button","press",WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,10,260,400,20,hWnd,(HMENU)T_P_GROUP,0,0);
	toggle = CreateWindow("Button","toggle",WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,10,280,400,20,hWnd,(HMENU)T_P_GROUP,0,0);

	rmlGroupBox = CreateWindow("Button","mouse button to click",WS_VISIBLE | WS_CHILD | BS_GROUPBOX,5,310,410,85,hWnd,(HMENU)R_M_L_GROUP,0,0);
	leftM = CreateWindow("Button","left",WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,10,330,400,20,hWnd,(HMENU)R_M_L_GROUP,0,0);	
	middleM = CreateWindow("Button","middle",WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,10,350,400,20,hWnd,(HMENU)R_M_L_GROUP,0,0);
	rightM = CreateWindow("Button","right",WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,10,370,400,20,hWnd,(HMENU)R_M_L_GROUP,0,0);

	SendMessage(hWnd, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));

	SendMessage(inputFrequency,EM_LIMITTEXT,8,0);

	if (my_mode == Mode_Toggle)
		doToggle = true;
	else
		doToggle = false;
	SendMessage(doToggle?toggle:press,BM_CLICK,0,0);

	if (my_button == Button_Left)
	{
		mouseToClick = 0;
		SendMessage(leftM, BM_CLICK, 0, 0);
	}
	if (my_button == Button_Middle)
	{
		mouseToClick = 1;
		SendMessage(middleM, BM_CLICK, 0, 0);
	}
	if (my_button == Button_Right)
	{
		mouseToClick = 2;
		SendMessage(rightM, BM_CLICK, 0, 0);
	}

	ShowWindow	(hWnd, show);
	UpdateWindow(hWnd);

	QueryPerformanceCounter(&countsOnLastFrame);

	memset(keyDown,0,VK_OEM_CLEAR);
	memset(keyPrev,0,VK_OEM_CLEAR);
	memset(keyTrig,0,VK_OEM_CLEAR);
	memset(keyUpTrig,0,VK_OEM_CLEAR);

	while(!quit)
	{			
		if(waitingForTrigger)
		{
			status = 1;
			if(status != prevStatus)
			{
				SetDlgItemText(hWnd,GetDlgCtrlID(statusText),"status (read-only): please hit trigger key");				
			}

			memset(keyDown,0,VK_OEM_CLEAR);
			memset(keyTrig,0,VK_OEM_CLEAR);
			memset(keyUpTrig,0,VK_OEM_CLEAR);
			for(int i=0;i<VK_OEM_CLEAR;i++)
			{
				if(GetAsyncKeyState(i))
				{
					keyDown[i]=1;
					if(!keyPrev[i])
						keyTrig[i]=1;
				}
				else if(keyPrev[i])
				{
					keyUpTrig[i]=1;				
				}

				keyPrev[i] = keyDown[i];				 
			}	

			if(keyUpTrig[1] && !clickedOnceForTriggerFlag)
			{
				clickedOnceForTriggerFlag = true;
				continue;
			}

			for(int i=0;i<VK_OEM_CLEAR;i++)
			{
				sameTriggerAndClick=false;
				if(keyUpTrig[i] && clickedOnceForTriggerFlag)
				{
					switch(mouseToClick)
					{
					case 0:
						if(i==VK_LBUTTON)
							sameTriggerAndClick=true;
						break;
					case 1:
						if(i==VK_MBUTTON)
							sameTriggerAndClick=true;
						break;
					case 2:
						if(i==VK_RBUTTON)
							sameTriggerAndClick=true;
						break;
					};
					_itoa_s(i,triggerText,4,10);
					SetFocus(outputWindow);
					SetDlgItemText(hWnd,GetDlgCtrlID(triggerButton),triggerText);
					waitingForTrigger = false;
					clickedOnceForTriggerFlag = false;
					break;
				}
			}
			if(waitingForTrigger == false)
				continue;
		}
		else
		{

			QueryPerformanceCounter(&currentCounts);
			frameTime=countsToSeconds(currentCounts.QuadPart-countsOnLastFrame.QuadPart);

			char numStr[12];

			//to ensure no problems with starting and stopping the toggle state:
			if(toggleState == 0 && GetAsyncKeyState(atoi(triggerText)))
				toggleState = 1;
			if(toggleState == 1 && !GetAsyncKeyState(atoi(triggerText)))
				toggleState = 2;
			if(toggleState == 2 && GetAsyncKeyState(atoi(triggerText)))
				toggleState = 3;
			if(toggleState == 3 && !GetAsyncKeyState(atoi(triggerText)))
				toggleState = 0;

			if(frameTime>1.0f/(frequency*2.0f))
			{
				UINT nBatchClicks = 1;

				if (frequency >= 10000)
					nBatchClicks = 100 + UINT(floor((frequency - 10000) / 100));
				else if (frequency >= 5000)
					nBatchClicks = 50 + UINT(floor((frequency - 5000) / 100));
				else if (frequency >= 1000)
					nBatchClicks = 10 + UINT(floor((frequency - 1000) / 100));
				else if (frequency >= 100)
					nBatchClicks = 1 + UINT(floor((frequency - 100) / 100));
				else
					nBatchClicks = 1;

				if(stopAtNum > 0 && numClicksSinceStop >= stopAtNum)
				{
					if(doToggle)
					{
						numClicksSinceStop = 0;
						toggleState = 0;
					}
					else
					{
						waitingForTriggerUp = true;
					}
				}

				if((doToggle && toggleState>=2) || !doToggle && GetAsyncKeyState(atoi(triggerText)) && !waitingForTriggerUp)
				{
					status = 2;
					if(status != prevStatus)
					{
						SetDlgItemText(hWnd,GetDlgCtrlID(statusText),"status (read-only): clicking");
					}
					if(switchFlag)
					{
						switch(mouseToClick)
						{
						case 0:
							my_mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0,nBatchClicks);
							break;
						case 1:
							my_mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0,nBatchClicks);
							break;
						case 2:
							my_mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0,nBatchClicks);
							break;
						};
						
						switchFlag=false;
					}
					else
					{
						switch(mouseToClick)
						{
						case 0:
							my_mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0,nBatchClicks);
							break;
						case 1:
							my_mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0,nBatchClicks);
							break;
						case 2:
							my_mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0,nBatchClicks);
							break;
						};
						
						switchFlag=true;
						numClicks+= nBatchClicks;
						numClicksSinceStop+= nBatchClicks;
						_itoa_s(numClicks,numStr,12,10);
						SetDlgItemText(hWnd,GetDlgCtrlID(outputWindow),numStr);

					}			
				}
				else if(!switchFlag)
				{
					switch(mouseToClick)
					{
					case 0:
						my_mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0,nBatchClicks);
						break;
					case 1:
						my_mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0,nBatchClicks);
						break;
					case 2:
						my_mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0,nBatchClicks);
						break;
					};
					switchFlag=true;

					numClicks+= nBatchClicks;
					numClicksSinceStop+= nBatchClicks;
					_itoa_s(numClicks,numStr,12,10);
					SetDlgItemText(hWnd,GetDlgCtrlID(outputWindow),numStr);
				}
				else
				{
					status = 0;
					numClicksSinceStop = 0;
					if(!GetAsyncKeyState(atoi(triggerText)))
					{
						waitingForTriggerUp = false;
					}
					if(status != prevStatus)
					{
						SetDlgItemText(hWnd,GetDlgCtrlID(statusText),"status (read-only): idle");
					}
				}
				countsOnLastFrame=currentCounts;
			}

			GetDlgItemText(hWnd,GetDlgCtrlID(inputFrequency),numStr,11);

			frequency = double(atof(numStr));
			bool bodd = has_only_digits_dot(std::string(numStr));

			if (!bodd)
			{
				if (frequency <= 0.0)
					frequency = 10.0;
				char abuf[9];
				_ftoa_s(frequency, abuf, 9, 10);
				SetDlgItemText(hWnd,GetDlgCtrlID(inputFrequency),abuf);
			}

			GetDlgItemText(hWnd, GetDlgCtrlID(stopAt), numStr, 11);
			stopAtNum = int(atof(numStr));
		}
		handleMessages();
		prevStatus = status;

		Sleep(1);
	}
	return 0;
}

LRESULT CALLBACK winCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) 
{
	HDC dc;   
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case RESET_BTN:
			{
				numClicks = 0;
				numClicksSinceStop = 0;

				double my_clicks_per_second = 10;
				int my_trigger_key = 0x0D;
				int my_stop_at = 0;
				Mode my_mode = Mode_Press;
				Button my_button = Button_Left;

				char numStrInputFrequency[9];
				_ftoa_s(my_clicks_per_second, numStrInputFrequency, 9, 10);

				char numStrStopAt[6];
				_itoa_s(my_stop_at, numStrStopAt, 6, 10);

				char numStrTriggerButton[4];
				_itoa_s(my_trigger_key, numStrTriggerButton, 4, 10);
				_itoa_s(my_trigger_key, triggerText, 4, 10);

				SetWindowTextA(outputWindow, "0");
				SetWindowTextA(inputFrequency, numStrInputFrequency);
				SetWindowTextA(stopAt, numStrStopAt);
				SetWindowTextA(triggerButton, numStrTriggerButton);

				if (my_mode == Mode_Toggle)
					doToggle = true;
				else
					doToggle = false;
				SendMessage(doToggle ? toggle : press, BM_CLICK, 0, 0);

				if (my_button == Button_Left)
				{
					mouseToClick = 0;
					SendMessage(leftM, BM_CLICK, 0, 0);
				}
				if (my_button == Button_Middle)
				{
					mouseToClick = 1;
					SendMessage(middleM, BM_CLICK, 0, 0);
				}
				if (my_button == Button_Right)
				{
					mouseToClick = 2;
					SendMessage(rightM, BM_CLICK, 0, 0);
				}

				//ShowWindow(hWnd, show);
				UpdateWindow(hWnd);
			}
			break;
		case HELP_BTN:
			MessageBox(hWnd, "The Fastest Mouse Clicker for Windows version 1.9.7.0.\n\n"
				"YOU CAN START THE AUTO-CLICKING AT ANY MOMENT BY PRESSING THE 'trigger key' (13 = Enter). Reading the entire Help is optional.\n\n"
				"THE FIELDS YOU CAN NOT MODIFY.\n\n"
				"'status (read-only)', the topmost text field, is either 'idle' or 'clicking'.\n"
				"'# clicks (read-only)', the top text field, indicates number of clicks.\n\n"
				"THE FIELDS YOU CAN MODIFY (CALLED THE CLICKING PARAMETERS: THEY COULD BE SET FROM THE COMMAND LINE TOO, SEE BELOW).\n\n"
				"'clicks/s', the middle text field, is the frequency of the clicks in clicks per second. "
				"Frequency can be as high as 99 thousands (99999) clicks per second.\n"
				"*NEW* YOU MAY ENTER FRACTIONAL FREQUENCIES. For example, 0.5 is one click every two seconds.\n"
				"'trigger key', below that, is the trigger key. Click on it and then press a key (or hit a mouse button). "
				"That key will then trigger the mouse clicks when it remains pressed. If you just press and release the key, only few clicks should be made. "
				"Default number in the button, 13, is the 'Enter' key (for example, 32 is the 'Space' key, 112 is the 'F1' key, "
				"for more Virtual-Key Codes see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731.aspx).\n"
				"'stop at', the lower text field, is the number of clicks before the clicking will automatically stop. "
				"0 is default and means infinity, i.e. clicking will never stop.\n"
				"'trigger key mode' is a radio-button group, you can select either 'press' or 'toggle' mode of clicking.\n"
				"'mouse button to click' is a radio-button group too, you can select either 'left', 'middle' or 'right' button.\n"
				"Note 1: You can't have the same mouse button be the trigger and clicker.\n"
				"Note 2: You can't change the trigger key if you chose the left mouse button; you must press the Reset to defaults button.\n"
				"Note 3: The trigger key still works when this program is minimized. You must close the program to stop a trigger key from clicking.\n\n"
				"ADDITIONAL BUTTONS AND FEATURES.\n\n"
				"'STOP!' button stops toggled clicking mandatory.\n"
				"'Help' button displays this help window.\n"
				"*NEW* 'Reset to defaults' button sets all the clicking parameters back to default values.\n"
				"*NEW* COMMAND LINE HAS BEEN SUPPORTED TO SET ALL THE CLICKING PARAMETERS DESCRIBED ABOVE.\n"
				"TheFastestMouseClicker.exe -c <clicks/s> -t <trigger key> -s <stop at> -m {p|t} -b {l|m|r},\n"
				"where '-m {p|t}' is the 'trigger key mode' {'press'|'toggle'} and '-b {l|m|r}' is the 'mouse button to click' {'left'|'middle'|'right'}. "
				"Unspecified or unrecognized values will be treated as defaults (see them by pressing the 'Reset to defaults' button).\n"
				"*NEW* All the clicking parameters are auto-saved between run-times.\n\n"
				"Copyright (c) 2017-2018 Open Source Developer Masha Novedad.\n"
				"https://sourceforge.net/projects/fast-mouse-clicker-pro/",
				"Help - The Fastest Mouse Clicker for Windows", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
			break;
		case R_M_L_GROUP:
			mouseToClick=0;
			if((HWND)lp == leftM)
			{
				mouseToClick = 0;
			}
			else if((HWND)lp == middleM)
			{
				mouseToClick = 1;
			}
			else if((HWND)lp == rightM)
			{
				mouseToClick = 2;
			}
			break;
		case T_P_GROUP:
			toggleState=0;
			if((HWND)lp == toggle)
			{
				doToggle = true;
			}
			else if((HWND)lp == press)
			{
				doToggle = false;
			}			
			break;
		case STOP_BTN:
			toggleState = 0;
			break;
		case TRIGGER_BTN:
			if(status!=2 && !waitingForTrigger)
			{
				toggleState = 0;
				waitingForTrigger = true;
			}
			break;
		}
		break;

	case WM_CREATE:
		{
			InvalidateRect(hWnd,NULL,TRUE);

			const CHAR* fontName = "Helvetica";
			const long nFontSize = 16;

			HDC hdc = GetDC(hWnd);

			LOGFONTA logFont = {0};
			logFont.lfHeight = -nFontSize;
			logFont.lfWeight = FW_NORMAL;
			strcpy(logFont.lfFaceName, fontName);

			s_hFont = CreateFontIndirect(&logFont);

			LOGFONTA logFontBold = {0};
			logFontBold.lfHeight = -nFontSize;
			logFontBold.lfWeight = FW_BOLD;
			strcpy(logFontBold.lfFaceName, fontName);

			s_hFontBold = CreateFontIndirect(&logFontBold);

			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_SETFONT:
		{
			SendMessage(button, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(outputWindow, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(inputFrequency, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(stopAt, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(triggerButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(stopButton, WM_SETFONT, (WPARAM)s_hFontBold, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(resetButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(helpButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(groupBox, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(toggle, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(press, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(statusText, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(rmlGroupBox, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(rightM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(middleM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(leftM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(numberClicks, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(clicksPerSecond, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(triggerKey, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(clicksStopAt, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
		}
		break;
	case WM_SIZE:
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case WM_PAINT:

		dc = BeginPaint(hWin, &ps);
		EndPaint(hWin, &ps);
		break;
		// When it's time for the window to go away
	case WM_DESTROY:
		{
			if (!s_bHasCommandLine)
			{
				// DONE: how to get these parameters from Windows GUI controls?
				char winTxt[1024];

				memset(winTxt, 0, 1024);
				GetWindowText(inputFrequency, winTxt, 1024);
				double my_clicks_per_second = atof(winTxt);

				memset(winTxt, 0, 1024);
				GetWindowText(triggerButton, winTxt, 1024);
				int my_trigger_key = atoi(winTxt);

				memset(winTxt, 0, 1024);
				GetWindowText(stopAt, winTxt, 1024);
				int my_stop_at = atoi(winTxt);

				Mode my_mode = doToggle ? Mode_Toggle : Mode_Press;

				Button my_button = (mouseToClick == 0) ? Button_Left : ((mouseToClick == 1) ? Button_Middle : Button_Right);

				char* outBuffer = new char[1024];
				memset(outBuffer, 0, 1024);

				int npr = _snprintf(outBuffer, 1024, "-c %5.2f -t %d -s %d -m %s -b %s", my_clicks_per_second, my_trigger_key, my_stop_at, (my_mode == Mode_Press) ? "p" : "t", (my_button == Button_Left) ? "l" : ((my_button == Button_Middle) ? "m" : "r"));

				saveMemToSubFile("\\settings.dat", outBuffer, 1024);
			}

			DeleteObject(s_hFont);
			s_hFont = NULL;
			quit=true;
			PostQuitMessage(0);
		}
		break;
		// called any time the window is moved
	case WM_MOVE:
		// Invalidate the rect to force a redraw
		InvalidateRect(hWin, NULL, TRUE);
		break;
	default:
		return DefWindowProc(hWin, msg, wp, lp);

	}
	return 0;
}