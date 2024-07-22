/**************************************************************************
* The Fastest Mouse Clicker for Windows version 2.6.1.1
* Copyright (c) 2016-2020 by Open Source Developer Masha Novedad
* Released under GNU Public License GPLv3
* 7th ANNIVERSARY tag is v2.6.1.1
**************************************************************************/

#undef UNICODE
#undef _UNICODE
#include "Windows.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <mutex>
using namespace std;

std::mutex mtx;

HWND hWnd;
HWND button;
HWND outputWindow;
HWND mouseCoords;
HWND genFixPosBatch;
HWND outputMouseX;
HWND outputMouseY;
HWND inputFrequency;
HWND stopAt;
HWND triggerButton;
HWND triggerButton2;
HWND stopButton;
HWND runGroupAppButton;
HWND resetButton;
HWND helpButton;
HWND folderButton;
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
HWND topMostChkBox;

WNDCLASS windClass;
WNDCLASS windClass2;
WNDCLASS windClass3;
HINSTANCE hInstance;

bool quit=false;
bool switchFlag = true;
int numClicks = 0;
int numClicksSinceStop = 0;
bool doToggle = false;
unsigned char toggleState = 0;
bool waitingForTrigger = false;
bool waitingForTrigger2 = false;
int status = 0;
int prevStatus = 0;
bool clickedOnceForTriggerFlag = false;
int mouseToClick = 0; //0=left 1=middle 2=right;
bool sameTriggerAndClick = false;
bool waitingForTriggerUp = false;
bool topMostWindow = false;

unsigned char keyDown[VK_OEM_CLEAR];
unsigned char keyPrev[VK_OEM_CLEAR];
unsigned char keyTrig[VK_OEM_CLEAR];
unsigned char keyUpTrig[VK_OEM_CLEAR];

#define TRIGGER_BTN 1000
#define TRIGGER_BTN2 1001
#define STOP_BTN 2000
#define RUNGROUPAPP_BTN 2500
#define INPUT_TEXT 3000
#define OUTPUT_TEXT 4000
#define OUTPUT_MOUSE_X 4100
#define OUTPUT_MOUSE_Y 4101
#define RESET_BTN 4500
#define HELP_BTN 5000
#define FOLDER_BTN 5500

#define T_P_GROUP 6000
#define R_M_L_GROUP 7000

#define STOP_AT_TEXT 8000

#define TOP_MOST_CHK_BOX 9000


LARGE_INTEGER countsOnLastFrame;
LARGE_INTEGER currentCounts;
double frameTime = 0.0;
double frequency = 100.0;
int stopAtNum = 0;
char triggerText[4]="13";
char triggerText2[4] = "32";
double countsToSeconds(__int64 a)
{
	LARGE_INTEGER temp;
	QueryPerformanceFrequency(&temp); // = counts/second
	return (double)(double(a)/double(temp.QuadPart));
}


LRESULT CALLBACK winCallBack(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK winCallBack2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK winCallBack3(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
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

struct BoundingRect
{
	LONG x;
	LONG dx;
	LONG y;
	LONG dy;
	BoundingRect() : x(0), dx(0), y(0), dy(0)  {}
	bool isValid() { return (x >= 0) && (dx > 0) && (y >= 0) && (dy > 0); }
} s_boundRect;

void SetMsgStatus(HWND hWnd, int hId, const char* msg)
{
	char spbuf[256];

	if (s_boundRect.isValid())
		_snprintf(spbuf, 256, "random clicking status: %s", msg);
	else
		_snprintf(spbuf, 256, "clicking status: %s", msg);

	SetDlgItemText(hWnd, hId, spbuf);
}

LONG s_dx = 0;
LONG s_dy = 0;
DWORD s_dwFlags = 0;

int CalculateAbsoluteCoordinateX(int x)
{
	return (x * 65536) / GetSystemMetrics(SM_CXSCREEN);
}

int CalculateAbsoluteCoordinateY(int y)
{
	return (y * 65536) / GetSystemMetrics(SM_CYSCREEN);
}

void my_mouse_event(_In_ DWORD     dwFlags,
					_In_ DWORD     dx,
					_In_ DWORD     dy,
					_In_ DWORD     dwData,
					_In_ ULONG_PTR dwExtraInfo,
					_In_ UINT nCnt = 1)
{
	int local_status = 0;

	mtx.lock();
	local_status = status;
	mtx.unlock();
	if (local_status == 0)
		return;

	INPUT input[4096];
	memset(input, 0, 4096 * sizeof(INPUT));

	if (nCnt > 1000)
		nCnt = 1000; // Avoid input buffer out-of-range ( total number of events may by nCnt * 2 or * 3

	input[0].type = INPUT_MOUSE;

	input[0].mi.dx = dx;
	input[0].mi.dy = dy;
	input[0].mi.dwFlags = dwFlags;

	input[0].mi.mouseData = dwData;
	input[0].mi.time = 0;
	input[0].mi.dwExtraInfo = dwExtraInfo;

	if (!(dwFlags & MOUSEEVENTF_ABSOLUTE)) // Usual follow mouse
	{
		if (
			(dwFlags & MOUSEEVENTF_LEFTDOWN)
			|| (dwFlags & MOUSEEVENTF_MIDDLEDOWN)
			|| (dwFlags & MOUSEEVENTF_RIGHTDOWN)
			)
		{
			// All done above
			UINT nCntExtra = 0;

			UINT ret = SendInput(1 + nCntExtra, input, sizeof(INPUT));

			if (ret != (1 + nCntExtra))
				SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "error in SendInput()");

			return;
		}

		if (
			(dwFlags & MOUSEEVENTF_LEFTUP)
			|| (dwFlags & MOUSEEVENTF_MIDDLEUP)
			|| (dwFlags & MOUSEEVENTF_RIGHTUP)
			)
		{
			UINT nCntExtra = (nCnt - 1) * 2; // reserved index for DOWN, UP

			for (UINT iExtra = 0; iExtra < nCntExtra; iExtra += 2)
			{
				input[1 + iExtra].type = INPUT_MOUSE;

				input[1 + iExtra].mi.dx = dx;
				input[1 + iExtra].mi.dy = dy;

				input[1 + iExtra].mi.mouseData = dwData;
				input[1 + iExtra].mi.time = 0;
				input[1 + iExtra].mi.dwExtraInfo = dwExtraInfo;

				input[1 + iExtra + 1] = input[1 + iExtra];

				if (dwFlags & MOUSEEVENTF_LEFTUP)
				{
					input[1 + iExtra].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
				}

				if (dwFlags & MOUSEEVENTF_MIDDLEUP)
				{
					input[1 + iExtra].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
				}

				if (dwFlags & MOUSEEVENTF_RIGHTUP)
				{
					input[1 + iExtra].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
				}
			}

			mtx.lock();
			local_status = status;
			mtx.unlock();
			if (local_status == 0)
				return;

			UINT ret = SendInput(1 + nCntExtra, input, sizeof(INPUT));

			if (ret != (1 + nCntExtra))
				SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "error in SendInput()");

			return;
		}
	}
	else // (dwFlags & MOUSEEVENTF_ABSOLUTE) is true
	{
		if (
			(dwFlags & MOUSEEVENTF_LEFTDOWN)
			|| (dwFlags & MOUSEEVENTF_MIDDLEDOWN)
			|| (dwFlags & MOUSEEVENTF_RIGHTDOWN)
			)
		{
			input[1] = input[0]; // Preceed DOWN by MOVE

			input[0].mi.dwFlags &= ~MOUSEEVENTF_LEFTDOWN;
			input[0].mi.dwFlags &= ~MOUSEEVENTF_MIDDLEDOWN;
			input[0].mi.dwFlags &= ~MOUSEEVENTF_RIGHTDOWN;
			input[0].mi.dwFlags |= MOUSEEVENTF_MOVE;

			UINT nCntExtra = 1;

			UINT ret = SendInput(1 + nCntExtra, input, sizeof(INPUT));

			if (ret != (1 + nCntExtra))
				SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "error in SendInput()");

			return;
		}

		if (
			(dwFlags & MOUSEEVENTF_LEFTUP)
			|| (dwFlags & MOUSEEVENTF_MIDDLEUP)
			|| (dwFlags & MOUSEEVENTF_RIGHTUP)
			)
		{
			UINT nCntExtra = (nCnt - 1) * 3; // Reserved index for MOVE, DOWN, UP

			LONG cur_dx = 0;
			LONG cur_dy = 0;

			for (UINT iExtra = 0; iExtra < nCntExtra; iExtra += 3) // Add extra MOVE, DOWN, UP
			{
				if ((iExtra % 3) == 0)
				{
					cur_dx = CalculateAbsoluteCoordinateX(s_boundRect.x + (rand() % s_boundRect.dx));
					cur_dy = CalculateAbsoluteCoordinateY(s_boundRect.y + (rand() % s_boundRect.dy));
				}

				input[1 + iExtra].type = INPUT_MOUSE;

				input[1 + iExtra].mi.dx = cur_dx;
				input[1 + iExtra].mi.dy = cur_dy;
				input[1 + iExtra].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

				input[1 + iExtra].mi.mouseData = dwData;
				input[1 + iExtra].mi.time = 0;
				input[1 + iExtra].mi.dwExtraInfo = dwExtraInfo;

				input[1 + iExtra + 1] = input[1 + iExtra];
				input[1 + iExtra + 2] = input[1 + iExtra];

				if (dwFlags & MOUSEEVENTF_LEFTUP)
				{
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
					input[1 + iExtra + 2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
				}

				if (dwFlags & MOUSEEVENTF_MIDDLEDOWN)
				{
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN;
					input[1 + iExtra + 2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN;
				}

				if (dwFlags & MOUSEEVENTF_RIGHTUP)
				{
					input[1 + iExtra + 1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
					input[1 + iExtra + 2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
				}

			}

			mtx.lock();
			local_status = status;
			mtx.unlock();
			if (local_status == 0)
				return;

			UINT ret = SendInput(1 + nCntExtra, input, sizeof(INPUT));

			if (ret != (1 + nCntExtra))
				SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "error in SendInput()");

			return;
		}
	}

	// Control flow should not reach this line
	SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "internal error in my_mouse_event()");

	return;
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

	 return true;
}

void parse_command_line(LPCSTR command_line, double& clicks_per_second, int& trigger_key, int& trigger_key2, int& stop_at, Mode& mode, Button& button, BoundingRect& boundRect, bool& top_most_win)
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

		if (parameters[i] == "-t2")
		{
			if ((i + 1) < parameters.size())
			{
				if (has_only_digits(parameters[i + 1]))
				{
					trigger_key2 = atoi(parameters[i + 1].c_str());
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

		if (parameters[i] == "-r")
		{
			if ((i + 4) < parameters.size())
			{
				boundRect.x = atoi(parameters[i + 1].c_str());
				boundRect.y = atoi(parameters[i + 2].c_str());
				boundRect.dx = atoi(parameters[i + 3].c_str());
				boundRect.dy = atoi(parameters[i + 4].c_str());
			}
		}

		if (parameters[i] == "-w")
		{
			if ((i + 1) < parameters.size())
			{
				if (parameters[i + 1] == "tm")
				{
					top_most_win = true;
				}

				if (parameters[i + 1] == "ntm")
				{
					top_most_win = false;
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

	size_t ncnt = fread(pBuffer, nFileSize, 1, fp);

	if (ncnt != 1)
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
	int n = sprintf(buf, "%5.2f", val);
	int m = n;
}

//////////////////////////////////////////////////////////////////////////////////////////////

static bool s_bHasCommandLine = false;

#include <stdarg.h>  // For va_start, etc.

std::string string_format(const std::string fmt, ...) {
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	std::string str;
	va_list ap;
	while (1) {     // Maximum two passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {  // Everything worked
			str.resize(n);
			return str;
		}
		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}
	return str;
}

//////////////////////////////////////////////////////////////////

#define APPLICATION_INSTANCE_MUTEX_NAME "TheFastestMouseClicker"

void CheckAlreadyRunning()
{
	//Make sure at most one instance of the tool is running
	HANDLE hMutexOneInstance(::CreateMutex(NULL, TRUE, APPLICATION_INSTANCE_MUTEX_NAME));
	bool bAlreadyRunning((::GetLastError() == ERROR_ALREADY_EXISTS));
	if (hMutexOneInstance == NULL || bAlreadyRunning)
	{
		if (hMutexOneInstance)
		{
			::ReleaseMutex(hMutexOneInstance);
			::CloseHandle(hMutexOneInstance);
		}

		MessageBox(hWnd, "Another instance of the application is already running, close it first."
			, "The Fastest Mouse Clicker for Windows", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);

		::ExitProcess(2);
	}
}

static int WriteBinary(const char* cfilename, const unsigned char* pChar, size_t pSize)
{
	FILE* fp;

	fp = fopen(cfilename, "wb");
	if (fp == NULL)
	{
		return __LINE__;
	}

	size_t szw = fwrite(pChar, pSize, 1, fp);
	if (szw != 1)
		return __LINE__;

	fclose(fp);
	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	while (true)
	{
		if (GetAsyncKeyState(atoi(triggerText2)))
		{
			mtx.lock();
			toggleState = 0;
			status = 0;
			prevStatus = 2;
			BlockInput(TRUE);
			Sleep(1000);
			BlockInput(FALSE);
			mtx.unlock();
			SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
		}

		RedrawWindow(outputMouseX, NULL, NULL, RDW_INVALIDATE);
		RedrawWindow(outputMouseY, NULL, NULL, RDW_INVALIDATE);
		POINT point;
		BOOL bGCP = GetCursorPos(&point);
		if (GetAsyncKeyState(VK_F9) && (bGCP == TRUE))
		{
			char winTxt[1024];

			memset(winTxt, 0, 1024);
			GetWindowText(triggerButton2, winTxt, 1024);
			int my_trigger_key2 = atoi(winTxt);

			memset(winTxt, 0, 1024);
			GetWindowText(inputFrequency, winTxt, 1024);
			double my_clicks_per_second = atof(winTxt);

			memset(winTxt, 0, 1024);
			GetWindowText(triggerButton, winTxt, 1024);
			int my_trigger_key = atoi(winTxt);

			memset(winTxt, 0, 1024);
			GetWindowText(stopAt, winTxt, 1024);
			int my_stop_at = atoi(winTxt);

			Mode my_mode = Mode_Press;
			LRESULT chkState = SendMessage(toggle, BM_GETCHECK, 0, 0);
			if (chkState == BST_CHECKED)
				my_mode = Mode_Toggle;

			//Mode my_mode = doToggle ? Mode_Toggle : Mode_Press; // Not good - this is another thread

			Button my_button = (mouseToClick == 0) ? Button_Left : ((mouseToClick == 1) ? Button_Middle : Button_Right);

			bool my_top_most_win = false;
			if (SendDlgItemMessage(hWnd, TOP_MOST_CHK_BOX, BM_GETCHECK, 0, 0))
				my_top_most_win = true;

			char strBatchFolder[MAX_PATH];
			char strBatchFolderFile[MAX_PATH];
			fillAppDataPath(strBatchFolder, "\\run_clicker_with_fixed_position.bat");
			memcpy(strBatchFolderFile, strBatchFolder, MAX_PATH);
			char* posFile = strstr(strBatchFolder, "\\run_clicker_with_fixed_position.bat");
			*posFile = '\0';

			char* outBuffer = new char[1024];
			memset(outBuffer, 0, 1024);

			int npr = _snprintf(outBuffer, 1024, "TheFastestMouseClicker.exe -c %5.2f -t %d -t2 %d -s %d -m %s -b %s -w %s -r %d %d 1 1\r\n", my_clicks_per_second, my_trigger_key, my_trigger_key2, my_stop_at, (my_mode == Mode_Press) ? "p" : "t", (my_button == Button_Left) ? "l" : ((my_button == Button_Middle) ? "m" : "r"), my_top_most_win ? "tm" : "ntm", point.x, point.y);

			if (npr > 0)
			{
				int res = WriteBinary(strBatchFolderFile, (unsigned char*)outBuffer, (size_t)npr);
				if (res == 0)
				{
					ShellExecuteA(NULL, "open", strBatchFolder, NULL, NULL, SW_SHOWNORMAL);
				}
			}
		}

		Sleep(40);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

struct _Sc
{
	int factor;
	_Sc() : factor(1)
	{
		int h, v;
		GetDesktopResolution(h, v);
		if (v > 1440)
			factor = 2;
	}
} _sc;

int Sc(int x)
{
	return x * _sc.factor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	CheckAlreadyRunning();

	memset(g_appRoot, 0, MAX_PATH);

	double my_clicks_per_second = 10;
	int my_trigger_key = 0x0D;
	int my_trigger_key2 = 0x20;
	int my_stop_at = 0;
	Mode my_mode = Mode_Press;
	Button my_button = Button_Left;
	bool my_top_most_win = false;

	char* my_pBuffer = NULL;
	size_t my_szBuffer = 0;

	loadSubFileToMem("\\settings.dat", my_pBuffer, my_szBuffer);

	BoundingRect dummy_boundRect;

	if ((my_pBuffer != NULL) && (my_szBuffer == 1024))
		parse_command_line(my_pBuffer, my_clicks_per_second, my_trigger_key, my_trigger_key2, my_stop_at, my_mode, my_button, dummy_boundRect, my_top_most_win);

	if (strlen(command_line) >= 4) // "-t 1" for example
	{
		parse_command_line(command_line, my_clicks_per_second, my_trigger_key, my_trigger_key2, my_stop_at, my_mode, my_button, s_boundRect, my_top_most_win);
		s_bHasCommandLine = true;
	}

	//copy-pasta windows stuff below
	hInstance=instanceH;
	// Initializing the window class
	memset(&windClass, 0, sizeof(WNDCLASS));
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

	// Initializing the window class
	memset(&windClass2, 0, sizeof(WNDCLASS));
	windClass2.style = CS_HREDRAW | CS_VREDRAW;
	windClass2.lpfnWndProc = winCallBack2;
	//windClass2.cbClsExtra = 0;
	//windClass2.cbWndExtra = 0;
	windClass2.hInstance = instanceH;
	//windClass2.hIcon = LoadIcon(windClass.hInstance, MAKEINTRESOURCE(101));
	windClass2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windClass2.hbrBackground = CreateSolidBrush(0x0099ffcc);
	windClass2.lpszClassName = "Color2";

	//Registering the window class
	RegisterClass(&windClass2);

	// Initializing the window class
	memset(&windClass3, 0, sizeof(WNDCLASS));
	windClass3.style = CS_HREDRAW | CS_VREDRAW;
	windClass3.lpfnWndProc = winCallBack3;
	//windClass3.cbClsExtra = 0;
	//windClass3.cbWndExtra = 0;
	windClass2.hInstance = instanceH;
	//windClass3.hIcon = LoadIcon(windClass.hInstance, MAKEINTRESOURCE(101));
	windClass3.hCursor = LoadCursor(NULL, IDC_ARROW);
	windClass3.hbrBackground = CreateSolidBrush(0x0099ffcc);
	windClass3.lpszClassName = "Color3";

	//Registering the window class
	RegisterClass(&windClass3);

	hWnd = CreateWindow("The Fastest Mouse Clicker for Windows","The Fastest Mouse Clicker for Windows v2.6.1.1", WS_OVERLAPPEDWINDOW, Sc(100), Sc(100), Sc(450), Sc(514), NULL, NULL, instanceH, NULL);

	statusText = CreateWindow("Static", "clicking status: idle", WS_VISIBLE | WS_CHILD, Sc(5), Sc(1), Sc(410), Sc(35), hWnd, 0, 0, 0);
	SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
	numberClicks = CreateWindow("Static", "number of clicks", WS_VISIBLE | WS_CHILD, Sc(5), Sc(40), Sc(410), Sc(20), hWnd, 0, 0, 0);
	mouseCoords = CreateWindow("Static", "mouse pos. (x,y)", WS_VISIBLE | WS_CHILD, Sc(252), Sc(40), Sc(150), Sc(20), hWnd, 0, 0, 0);
	genFixPosBatch = CreateWindow("Static", "F9 = fix. click batch", WS_VISIBLE | WS_CHILD, Sc(252), Sc(80), Sc(163), Sc(20), hWnd, 0, 0, 0);
	clicksPerSecond = CreateWindow("Static", "clicks per second", WS_VISIBLE | WS_CHILD, Sc(5), Sc(60), Sc(410), Sc(20), hWnd, 0, 0, 0);
	triggerKey = CreateWindow("Static", "begin/end trigger keys", WS_VISIBLE | WS_CHILD, Sc(5), Sc(80), Sc(205), Sc(20), hWnd, 0, 0, 0);
	clicksStopAt = CreateWindow("Static", "stop at", WS_VISIBLE | WS_CHILD, Sc(5), Sc(100), Sc(410), Sc(20), hWnd, 0, 0, 0);

	char numStrInputFrequency[9];
	_ftoa_s(my_clicks_per_second, numStrInputFrequency, 9, 10);

	char numStrStopAt[12];
	_itoa_s(my_stop_at, numStrStopAt, 12, 10);

	char numStrTriggerButton[4];
	_itoa_s(my_trigger_key, numStrTriggerButton, 4, 10);
	_itoa_s(my_trigger_key, triggerText, 4, 10);

	char numStrTriggerButton2[4];
	_itoa_s(my_trigger_key2, numStrTriggerButton2, 4, 10);
	_itoa_s(my_trigger_key2, triggerText2, 4, 10);

	outputWindow = CreateWindow("Edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_NUMBER, Sc(170), Sc(40), Sc(80), Sc(20), hWnd, (HMENU)OUTPUT_TEXT, 0, 0);
	outputMouseX = CreateWindow("Color2", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_NUMBER, Sc(252), Sc(60), Sc(80), Sc(20), hWnd, (HMENU)OUTPUT_MOUSE_X, 0, 0);
	outputMouseY = CreateWindow("Color3", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_NUMBER, Sc(334), Sc(60), Sc(80), Sc(20), hWnd, (HMENU)OUTPUT_MOUSE_Y, 0, 0);
	inputFrequency = CreateWindow("Edit", numStrInputFrequency, WS_VISIBLE | WS_CHILD | WS_BORDER, Sc(170), Sc(60), Sc(80), Sc(20), hWnd, (HMENU)INPUT_TEXT, 0, 0);
	triggerButton = CreateWindow("Button", numStrTriggerButton, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, Sc(170), Sc(80), Sc(40), Sc(20), hWnd, (HMENU)TRIGGER_BTN, 0, 0);
	triggerButton2 = CreateWindow("Button", numStrTriggerButton2, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, Sc(210), Sc(80), Sc(40), Sc(20), hWnd, (HMENU)TRIGGER_BTN2, 0, 0);
	stopAt = CreateWindow("Edit", numStrStopAt, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, Sc(170), Sc(100), Sc(80), Sc(20), hWnd, (HMENU)STOP_AT_TEXT, 0, 0);
	stopButton = CreateWindow("Button", "STOP!", WS_VISIBLE | WS_CHILD, Sc(5), Sc(125), Sc(410 / 2 - 3), Sc(50), hWnd, (HMENU)STOP_BTN, 0, 0);
	runGroupAppButton = CreateWindow("Button", "Run group app", WS_VISIBLE | WS_CHILD, Sc(5 + 410 / 2 + 3), Sc(125), Sc(410 / 2 - 3), Sc(50), hWnd, (HMENU)RUNGROUPAPP_BTN, 0, 0);
	resetButton = CreateWindow("Button", "Reset to defaults", WS_VISIBLE | WS_CHILD, Sc(5), Sc(180), Sc(410 / 3), Sc(50), hWnd, (HMENU)RESET_BTN, 0, 0);
	helpButton = CreateWindow("Button", "Help", WS_VISIBLE | WS_CHILD, Sc(5 + 5 + 410 / 3), Sc(180), Sc(410 / 3 - 5), Sc(50), hWnd, (HMENU)HELP_BTN, 0, 0);
	folderButton = CreateWindow("Button", "Batch folder", WS_VISIBLE | WS_CHILD, Sc(5 + 5 + 410 / 3 + 410 / 3), Sc(180), Sc(410 / 3 - 3), Sc(50), hWnd, (HMENU)FOLDER_BTN, 0, 0);

	groupBox = CreateWindow("Button", "trigger key mode", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, Sc(5), Sc(240), Sc(410), Sc(65), hWnd, (HMENU)T_P_GROUP, 0, 0);
	press = CreateWindow("Button", string_format("press (clicking while: key <%d> keeps hit down)", my_trigger_key).c_str(), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, Sc(10), Sc(260), Sc(400), Sc(20), hWnd, (HMENU)T_P_GROUP, 0, 0);
	toggle = CreateWindow("Button", string_format("toggle (clicking begin: hit <%d>, end: hit <%d>)", my_trigger_key, my_trigger_key2).c_str(), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, Sc(10), Sc(280), Sc(400), Sc(20), hWnd, (HMENU)T_P_GROUP, 0, 0);

	rmlGroupBox = CreateWindow("Button", "mouse button to click", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, Sc(5), Sc(310), Sc(410), Sc(85), hWnd, (HMENU)R_M_L_GROUP, 0, 0);
	leftM = CreateWindow("Button", "left", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, Sc(10), Sc(330), Sc(400), Sc(20), hWnd, (HMENU)R_M_L_GROUP, 0, 0);
	middleM = CreateWindow("Button", "middle", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, Sc(10), Sc(350), Sc(400), Sc(20), hWnd, (HMENU)R_M_L_GROUP, 0, 0);
	rightM = CreateWindow("Button", "right", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, Sc(10), Sc(370), Sc(400), Sc(20), hWnd, (HMENU)R_M_L_GROUP, 0, 0);

	topMostChkBox = CreateWindow("Button", "Window Always Top", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, Sc(10), Sc(400), Sc(400), Sc(20), hWnd, (HMENU)TOP_MOST_CHK_BOX, 0, 0);

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

	topMostWindow = my_top_most_win;
	if (topMostWindow)
		SendMessage(topMostChkBox, BM_CLICK, 0, 0);

	ShowWindow(hWnd, show);
	UpdateWindow(hWnd);

	QueryPerformanceCounter(&countsOnLastFrame);

	memset(keyDown,0,VK_OEM_CLEAR);
	memset(keyPrev,0,VK_OEM_CLEAR);
	memset(keyTrig,0,VK_OEM_CLEAR);
	memset(keyUpTrig,0,VK_OEM_CLEAR);

	DWORD dwThreadIdArray = 0;

	HANDLE hThreadArray = (HANDLE)CreateThread(
		NULL                   // default security attributes
		, 0                      // use default stack size
		, MyThreadFunction       // thread function name
		, NULL          // argument to thread function
		, 0                      // use default creation flags
		, &dwThreadIdArray
		);

	while(!quit)
	{	
		if (GetAsyncKeyState(atoi(triggerText2)))
		{
			mtx.lock();
			toggleState = 0;
			status = 0;
			prevStatus = 2;
			mtx.unlock();
			SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
		}

		if (waitingForTrigger || waitingForTrigger2)
		{
			int local_status = 0;
			int local_prevStatus = 0;
			mtx.lock();
			local_status = status = 1;
			local_prevStatus = prevStatus;
			mtx.unlock();

			if(local_status != local_prevStatus)
			{
				SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "please hit the trigger key");
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
					if (waitingForTrigger)
					{
						_itoa_s(i, triggerText, 4, 10);
						SetFocus(outputWindow);
						SetDlgItemText(hWnd, GetDlgCtrlID(triggerButton), triggerText);
						my_trigger_key = atoi(triggerText);
						char winTxt[1024];
						memset(winTxt, 0, 1024);
						GetWindowText(triggerButton2, winTxt, 1024);
						my_trigger_key2 = atoi(winTxt);
						SetWindowTextA(press, string_format("press (clicking while: key <%d> keeps hit down)", my_trigger_key).c_str());
						SetWindowTextA(toggle, string_format("toggle (clicking begin: hit <%d>, end: hit <%d>)", my_trigger_key, my_trigger_key2).c_str());
						waitingForTrigger = false;
					}
					if (waitingForTrigger2)
					{
						_itoa_s(i, triggerText2, 4, 10);
						SetFocus(outputWindow);
						SetDlgItemText(hWnd, GetDlgCtrlID(triggerButton2), triggerText2);
						my_trigger_key2 = atoi(triggerText2);
						char winTxt[1024];
						memset(winTxt, 0, 1024);
						GetWindowText(triggerButton, winTxt, 1024);
						my_trigger_key = atoi(winTxt);
						SetWindowTextA(press, string_format("press (clicking while: key <%d> keeps hit down)", my_trigger_key).c_str());
						SetWindowTextA(toggle, string_format("toggle (clicking begin: hit <%d>, end: hit <%d>)", my_trigger_key, my_trigger_key2).c_str());
						waitingForTrigger2 = false;
					}
					clickedOnceForTriggerFlag = false;
					break;
				}
			}
			if ((waitingForTrigger == false) && (waitingForTrigger2 == false))
				continue;
		}
		else
		{

			QueryPerformanceCounter(&currentCounts);
			frameTime=countsToSeconds(currentCounts.QuadPart-countsOnLastFrame.QuadPart);

			char numStr[12];

			if (!doToggle)
			{
				mtx.lock();
				//to ensure no problems with starting and stopping the toggle state:
				if (toggleState == 0 && GetAsyncKeyState(atoi(triggerText)))
					toggleState = 1;
				if (toggleState == 1 && !GetAsyncKeyState(atoi(triggerText)))
				{
					toggleState = 2;
				}
				if (toggleState == 2 && GetAsyncKeyState(atoi(triggerText)))
					toggleState = 3;
				if (toggleState == 3 && !GetAsyncKeyState(atoi(triggerText)))
				{
					toggleState = 0;
					//BlockInput(TRUE);
					//Sleep(100);
					//BlockInput(FALSE);
				}
				mtx.unlock();
			}
			else
			{
				mtx.lock();
				//to ensure no problems with starting and stopping the toggle state:
				if (toggleState == 0 && GetAsyncKeyState(atoi(triggerText)))
					toggleState = 1;
				if (toggleState == 1 && !GetAsyncKeyState(atoi(triggerText)))
					toggleState = 2;
				if (toggleState == 2 && GetAsyncKeyState(atoi(triggerText2)))
					toggleState = 3;
				if (toggleState == 3 && !GetAsyncKeyState(atoi(triggerText2)))
					toggleState = 0;
				mtx.unlock();
			}

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
						mtx.lock();
						toggleState = 0;
						mtx.unlock();
					}
					else
					{
						waitingForTriggerUp = true;
					}
				}

				unsigned char local_toggleState = 0;
				mtx.lock();
				local_toggleState = toggleState;
				mtx.unlock();

				if((doToggle && local_toggleState>=2) || !doToggle && GetAsyncKeyState(atoi(triggerText)) && !waitingForTriggerUp)
				{
					int local_status = 0;
					int local_prevStatus = 0;
					mtx.lock();
					local_status = status = 2;
					local_prevStatus = prevStatus;
					mtx.unlock();

					if(local_status != local_prevStatus)
					{
						SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "clicking");
					}
					if(switchFlag)
					{
						if (s_boundRect.isValid())
						{
							s_dx = CalculateAbsoluteCoordinateX(s_boundRect.x + (rand() % s_boundRect.dx));
							s_dy = CalculateAbsoluteCoordinateY(s_boundRect.y + (rand() % s_boundRect.dy));
							s_dwFlags = MOUSEEVENTF_ABSOLUTE;
						}
						else
						{
							s_dx = 0;
							s_dy = 0;
							s_dwFlags = 0;
						}

						switch(mouseToClick)
						{
						case 0:
							my_mouse_event(MOUSEEVENTF_LEFTDOWN | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, 1); // On mouse DOWN no extra generation of events - see UP
							break;
						case 1:
							my_mouse_event(MOUSEEVENTF_MIDDLEDOWN | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, 1);
							break;
						case 2:
							my_mouse_event(MOUSEEVENTF_RIGHTDOWN | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, 1);
							break;
						};
						
						switchFlag=false;
					}
					else
					{
						switch(mouseToClick)
						{
						case 0:
							my_mouse_event(MOUSEEVENTF_LEFTUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks); // On mouse UP we DO extra generation of events
							break;
						case 1:
							my_mouse_event(MOUSEEVENTF_MIDDLEUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks);
							break;
						case 2:
							my_mouse_event(MOUSEEVENTF_RIGHTUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks);
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
						my_mouse_event(MOUSEEVENTF_LEFTUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks); // On mouse UP we DO extra generation of events
						break;
					case 1:
						my_mouse_event(MOUSEEVENTF_MIDDLEUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks);
						break;
					case 2:
						my_mouse_event(MOUSEEVENTF_RIGHTUP | s_dwFlags, s_dx, s_dy, 0, (ULONG_PTR)NULL, nBatchClicks);
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
					int local_status = 0;
					int local_prevStatus = 0;
					mtx.lock();
					local_status = status = 0;
					local_prevStatus = prevStatus;
					mtx.unlock();

					if (local_status != local_prevStatus)
					{
						SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
					}

					numClicksSinceStop = 0;
					if(!GetAsyncKeyState(atoi(triggerText)))
					{
						waitingForTriggerUp = false;
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
		mtx.lock();
		prevStatus = status;
		mtx.unlock();

		Sleep(1);
	}
	return 0;
}

LRESULT CALLBACK winCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) 
{
	HDC dc;   
	PAINTSTRUCT ps;
	int local_status = 0;
	switch (msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case RESET_BTN:
			{
				mtx.lock();
				toggleState = 0;
				status = 0;
				prevStatus = 0;
				mtx.unlock();

				waitingForTrigger = false;
				waitingForTrigger2 = false;

				clickedOnceForTriggerFlag = false;
				sameTriggerAndClick = false;
				waitingForTriggerUp = false;

				numClicks = 0;
				numClicksSinceStop = 0;

				double my_clicks_per_second = 10;
				int my_trigger_key = 0x0D;
				int my_trigger_key2 = 0x20;
				int my_stop_at = 0;
				Mode my_mode = Mode_Press;
				Button my_button = Button_Left;

				char numStrInputFrequency[9];
				_ftoa_s(my_clicks_per_second, numStrInputFrequency, 9, 10);

				char numStrStopAt[12];
				_itoa_s(my_stop_at, numStrStopAt, 12, 10);

				char numStrTriggerButton[4];
				_itoa_s(my_trigger_key, numStrTriggerButton, 4, 10);
				_itoa_s(my_trigger_key, triggerText, 4, 10);

				char numStrTriggerButton2[4];
				_itoa_s(my_trigger_key2, numStrTriggerButton2, 4, 10);
				_itoa_s(my_trigger_key2, triggerText2, 4, 10);

				SetWindowTextA(outputWindow, "0");
				SetWindowTextA(inputFrequency, numStrInputFrequency);
				SetWindowTextA(stopAt, numStrStopAt);
				SetWindowTextA(triggerButton, numStrTriggerButton);
				SetWindowTextA(triggerButton2, numStrTriggerButton2);

				SetWindowTextA(press, string_format("press (clicking while: key <%d> keeps hit down)", my_trigger_key).c_str());
				SetWindowTextA(toggle, string_format("toggle (clicking begin: hit <%d>, end: hit <%d>)", my_trigger_key, my_trigger_key2).c_str());

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

				LRESULT chkState = SendMessage(topMostChkBox, BM_GETCHECK, 0, 0);
				if (chkState == BST_CHECKED)
					SendMessage(topMostChkBox, BM_SETCHECK, BST_UNCHECKED, 0);

				//ShowWindow(hWnd, show);
				UpdateWindow(hWnd);
			}
			break;
		case HELP_BTN:
			MessageBox(hWnd, "The Fastest Mouse Clicker for Windows v2.6.1.1 (Independent Keys For Toggle Clicking; Window Always Top; Random Clicking)."
				"\n\nYOU CAN START THE AUTO-CLICKING AT ANY MOMENT BY PRESSING THE <trigger key> (13 = Enter). Reading the entire Help is optional."
				"\n\nTHE FIELDS YOU CAN NOT MODIFY."
				"\n<clicking status> or <random clicking status>, the topmost text field, is either getting 'idle' or 'clicking'."
				" It is shown as <random clicking status> only when all the rectangle sizes to click randomly inside it are specified in the command line correctly."
				" Just press the [Batch folder] button and see the remarks in file run_clicker_with_random_clicking.bat."
				"\n<number of clicks>, the top text field, indicates total number of clicks performed."
				"\n\nTHE FIELDS YOU CAN MODIFY (CALLED THE CLICKING PARAMETERS: THEY COULD BE SET FROM THE COMMAND LINE TOO, SEE BELOW)."
				"\n<clicks per second>, the middle text field, is the frequency of the clicks measured in clicks per second."
				" This frequency can be as high as 100 thousands (100000) clicks per second."
				" FRACTIONAL frequences are supported. For example, 0.5 corresponds to 1 click every 2 seconds, 0.25 - to 1 click every 4 seconds, etc."
				"\n<begin/end trigger keys>, below that, are the keyboard keys to trigger the mouse events. Just click on them and press an arbitrary key (or hit a mouse button)."
				" That key will then trigger the mouse clicks when it remains pressed. If you just press and release the key, only few clicks should be made."
				" This behavior corresponds to <trigger key mode> = 'press', see how it changes on 'toggle' value below."
				" Default number shown in the button, 13, is the 'Enter' key code (for example, 32 is the 'Space' key code, 112 is the 'F1' key code, etc."
				" For all the key codes see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731.aspx)."
				"\n<stop at>, the lower text field, is the number of clicks before the clicking will automatically stop."
				" 0 is the default and means infinity, i.e. clicking will never stop."
				"\n<trigger key mode> is a radio-button group, you can select either 'press' or 'toggle' mode of clicking."
				" In the 'press' mode (default), the mouse events are emitted only when the corresponding trigger key is kept pressed."
				" In the 'toogle' mode, the mouse events are emitted between subsequent short hits to the <begin trigger key> and <end trigger key>."
				"\n<mouse button to click> is a radio-button group too, you can select either 'left', 'middle' or 'right' mouse button that will generate the clicks."
				"\nNote 1: You can't have the same mouse button be the trigger and clicker."
				"\nNote 2: You can't change the <trigger key> if you chose the left mouse button; you must press the [Reset to defaults] button."
				"\nNote 3: The <trigger key> still works when this program is minimized. You must close the program to stop a <trigger key> from clicking."
				"\n*NEW* All the clicking parameters are saved automatically between application run-times."
				"\n\nADDITIONAL BUTTONS AND FEATURES."
				"\n[STOP!] button stops toggled clicking mandatory."
				"\n[Help] button displays this help window."
				"\n*NEW* [Reset to defaults] button sets all the clicking parameters back to their default values."
				"\n*NEW* [Batch folder] button opens the folder in File Explorer where all the batch files reside typically."
				"\n*NEW* To get help on the command line arguments, just press the [Batch folder] button and see the remarks in *.bat files you find there."
				"\n*NEW* Independent keys for <trigger key mode> = 'toggle': if <begin trigger key> begins the clicking, then <end trigger key> stops it."
				"\n*NEW* <Window Always Top> checkbox: if checked, keeps the app's main window at topmost of others."
				"\n*BRAND NEW* The 'Run group app' button: record/play the sequences of mouse clicks."
				"\n\nCopyright (c) 2016-2020 Open Source Developer Masha Novedad."
				"\nhttps://windows-2048.github.io/The-Fastest-Mouse-Clicker-for-Windows/",
				"Help - The Fastest Mouse Clicker for Windows", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
			break;
		case FOLDER_BTN:
			{
				char strBatchFolder[MAX_PATH];
				fillAppDataPath(strBatchFolder, "");
				ShellExecuteA(NULL, "open", strBatchFolder, NULL, NULL, SW_SHOWNORMAL);
			}
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
		case TOP_MOST_CHK_BOX:
			{
				switch (HIWORD(wp))
				{
				case BN_CLICKED:
					if (SendDlgItemMessage(hWnd, TOP_MOST_CHK_BOX, BM_GETCHECK, 0, 0))
					{
						SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						topMostWindow = true;
					}
					else
					{
						SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						topMostWindow = false;
					}
					break;
				}
			}
			break;
		case T_P_GROUP:
			mtx.lock();
			toggleState = 0;
			status = 0;
			prevStatus = 0;
			mtx.unlock();
			//toggleState=0;
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
			mtx.lock();
			toggleState = 0;
			status = 0;
			prevStatus = 2;
			mtx.unlock();
			SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
			break;
		case RUNGROUPAPP_BTN:
			{
				WCHAR path[MAX_PATH];
				memset(path, 0, MAX_PATH * sizeof(WCHAR));
				DWORD plen = GetModuleFileNameW(NULL, path, MAX_PATH);
				if (plen == 0)
					return 0;
				size_t i = MAX_PATH;
				--i;
				while (path[i] != L'\\')
					--i;
				path[i] = L'\0';
				BOOL BRes = SetCurrentDirectoryW(path);
				if (!BRes)
					return 0;

				DWORD dwCreationFlags = 0;
				DWORD waitRes = 0;
				DWORD dwExitCode = 0;

				STARTUPINFOW si;
				PROCESS_INFORMATION pi;

				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				ZeroMemory(&pi, sizeof(pi));

				if (CreateProcessW(
					L"TheFastestMouseGroupClicker.exe"   // Module name (TODO: alternate data stream welcome)
					, L""        // Command line TODO: host, port, weak-encrypted-sha256Image
					, NULL           // Process handle not inheritable
					, NULL           // Thread handle not inheritable
					, FALSE          // Set handle inheritance to FALSE
					, dwCreationFlags              // No creation flags
					, NULL           // Use parent's environment block
					, NULL		// Use parent's starting directory 
					, &si            // Pointer to STARTUPINFO structure
					, &pi
					)           // Pointer to PROCESS_INFORMATION structure
					)
					DestroyWindow(hWnd);
				return 0;
			}
			break;
		case TRIGGER_BTN:
			mtx.lock();
			local_status = status;
			mtx.unlock();
			if(local_status!=2 && !waitingForTrigger)
			{
				mtx.lock();
				toggleState = 0;
				mtx.unlock();
				waitingForTrigger = true;
			}
			break;
		case TRIGGER_BTN2:
			mtx.lock();
			local_status = status;
			mtx.unlock();
			if (local_status != 2 && !waitingForTrigger2)
			{
				mtx.lock();
				toggleState = 0;
				mtx.unlock();
				waitingForTrigger2 = true;
			}
			break;
		}
		break;

	case WM_CREATE:
		{
			InvalidateRect(hWnd,NULL,TRUE);

			const CHAR* fontName = "Helvetica";
			const long nFontSize = Sc(16);

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
			SendMessage(triggerButton2, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(stopButton, WM_SETFONT, (WPARAM)s_hFontBold, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(runGroupAppButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(resetButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(helpButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(folderButton, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(groupBox, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(toggle, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(press, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(statusText, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(rmlGroupBox, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(rightM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(topMostChkBox, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(middleM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(leftM, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(numberClicks, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(clicksPerSecond, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(triggerKey, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(clicksStopAt, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(mouseCoords, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(genFixPosBatch, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(outputMouseX, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
			SendMessage(outputMouseY, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
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
				GetWindowText(triggerButton2, winTxt, 1024);
				int my_trigger_key2 = atoi(winTxt);

				memset(winTxt, 0, 1024);
				GetWindowText(stopAt, winTxt, 1024);
				int my_stop_at = atoi(winTxt);

				Mode my_mode = doToggle ? Mode_Toggle : Mode_Press;

				Button my_button = (mouseToClick == 0) ? Button_Left : ((mouseToClick == 1) ? Button_Middle : Button_Right);

				bool my_top_most_win = false;
				if (SendDlgItemMessage(hWnd, TOP_MOST_CHK_BOX, BM_GETCHECK, 0, 0))
					my_top_most_win = true;

				char* outBuffer = new char[1024];
				memset(outBuffer, 0, 1024);

				int npr = _snprintf(outBuffer, 1024, "-c %5.2f -t %d -t2 %d -s %d -m %s -b %s -w %s", my_clicks_per_second, my_trigger_key, my_trigger_key2, my_stop_at, (my_mode == Mode_Press) ? "p" : "t", (my_button == Button_Left) ? "l" : ((my_button == Button_Middle) ? "m" : "r"), my_top_most_win ? "tm" : "ntm");

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

LRESULT CALLBACK winCallBack2(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc = BeginPaint(hWin, &ps);
		GetClientRect(hWin, &rc);
		SetBkColor(hdc, 0x0099ffcc); // red
		SelectObject(hdc, s_hFont);
		POINT point;
		BOOL bGCP = GetCursorPos(&point);
		if (bGCP == TRUE)
		{
			char numStr[12];
			memset(numStr, 0, 12);
			_itoa_s(point.x, numStr, 12, 10);
			ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, numStr, 5, 0);
		}
		EndPaint(hWin, &ps);
	}
	break;

	default:
		return DefWindowProc(hWin, msg, wp, lp);
	}

	return 0;
}

LRESULT CALLBACK winCallBack3(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc = BeginPaint(hWin, &ps);
		GetClientRect(hWin, &rc);
		SetBkColor(hdc, 0x0099ffcc); // red
		SelectObject(hdc, s_hFont);
		POINT point;
		BOOL bGCP = GetCursorPos(&point);
		if (bGCP == TRUE)
		{
			char numStr[12];
			memset(numStr, 0, 12);
			_itoa_s(point.y, numStr, 12, 10);
			ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, numStr, 5, 0);
		}
		EndPaint(hWin, &ps);
	}
	break;

	default:
		return DefWindowProc(hWin, msg, wp, lp);
	}

	return 0;
}
