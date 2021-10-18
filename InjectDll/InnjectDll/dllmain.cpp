// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "stdio.h"
#include "stdarg.h"
#include "windows.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include <deque>
#include <string>
#include <unordered_set>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"detours.lib")
#pragma comment (lib, "ws2_32.lib")

using json = nlohmann::json;
using namespace std;
std::deque<json> logs;

SYSTEMTIME st;

//Socket通信
SOCKET ClientSocket;
SOCKADDR_IN sockAddr;

int SocketCreate() {
    WORD wdVersion = MAKEWORD(2, 2);
    WSADATA wdSockMsg;
    if (WSAStartup(wdVersion, &wdSockMsg) != 0) {
        WSACleanup();
        return 0;
    }
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(9999);
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (connect(ClientSocket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        return 0;
    }
    return 1;
}

void SocketClose() {
    closesocket(ClientSocket);
    WSACleanup();
}

//发送log日志
void sendlogs() {
    std::ofstream file("info.log", std::ios::app);
    while (!logs.empty()) {
        file << logs.front() << std::endl;
        logs.pop_front();
    }
    file.close();
}

//弹窗操作
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {
    char time[70] = { 0 };
    char to_string[4][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("MessageBoxA Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hWnd);
    sprintf_s(to_string[1], "%s", lpText);
    sprintf_s(to_string[2], "%s", lpCaption);
    sprintf_s(to_string[3], "%08X", uType);

    json log = {
        {"type", "MessageBoxA"},
        {"time", time},
        {"arg", {
            {"hWnd", to_string[0]},
            {"LpText", to_string[1]},
            {"lpCaption", to_string[2]},
            {"uType",to_string[3]}
        }}
    };
    json info = {
            {"typ", 3},
            {"num1", 1},
            {"num2", 1},
            {"unu", {{"u1", 1}, {"u2", 0}, {"u3", 0}}},
            {"api", {{"n1", 1}, {"n2", 0}, {"n3", 0}, {"n4", 0}}}
    };

    send(ClientSocket, info.dump().c_str(), 1023, 0);

    logs.emplace_back(log);
    //return OldMessageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);
    return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
}

static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
    char time[70] = { 0 };
    char to_string[4][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringW(L"MessageBoxW Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hWnd);
    sprintf_s(to_string[1], "%ws", lpText);
    sprintf_s(to_string[2], "%ws", lpCaption);
    sprintf_s(to_string[3], "%08X", uType);

    json log = {
        {"type", "MessageBoxW"},
        {"time", time},
        {"arg", {
            {"hWnd", to_string[0]},
            {"LpText", to_string[1]},
            {"lpCaption", to_string[2]},
            {"uType",to_string[3]}
        }}
    };
    json info = {
            {"typ", 3},
            {"num1", 1},
            {"num2", 1},
            {"unu", {{"u1", 1}, {"u2", 0}, {"u3", 0}}},
            {"api", {{"n1", 2}, {"n2", 0}, {"n3", 0}, {"n4", 0}}}
    };

    send(ClientSocket, info.dump().c_str(), 1023, 0);

    logs.emplace_back(log);
    //return OldMessageBoxW(NULL, L"new MessageBoxW", L"Hooked", MB_OK);
    return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
}

// 文件操作
typedef struct skeptical { //可能产生危险的文件
    string name;
    int time;
}skeptical;

typedef struct tree { //文件树
    string now;
    int next;
}tree;

typedef struct cpyfile { //记录cpoyfile函数的使用
    string A;
    string B;
    bool w;
}cpyfile;

typedef struct rtfile { //记录writefile函数的使用
    string content;
    HANDLE hfile;
}rtfile;

vector<skeptical> ff;
vector<string> file;
vector<cpyfile> cpy;
vector<rtfile> rt;
vector<int> next_P;
vector<string> deletefilec;
vector<string> temporary;
bool func[12] = { false };
bool output[14] = { false };
char* out_time[14][70] = { 0 };

vector<vector<tree>> trie;
int file_num = 0;
int countn = 0;
int number = 0;
int c_num = 0;
HANDLE tem = NULL;

extern "C" __declspec(dllexport)int change()
{
    GetLocalTime(&st);
    return(st.wHour * 3600 + st.wMinute * 60 + st.wSecond);
}

//<10s进行判定
//findfirstfile,findnextfile,createfile,readfile:警告文件信息可能泄露
//GetProcAddress,LoadLibraryA,CreateFileMappingA,MapViewOfFile,SetFilePointer,SetEndOfFile:警告很可能是有传播性的病毒在感染其它文件
//deletefile,writefile:警告文件可能被恶意篡改

extern "C" __declspec(dllexport)bool* virus()
{
    int timeb = change();
    memset(func, 0, sizeof(func));
    for (int i = ff.size() - 1; i >= 0; i--)
    {
        if (ff[i].time + 10 >= timeb)
        {
            if (ff[i].name == "findfirstfile") func[0] = output[6] = true;
            else if (ff[i].name == "findnextfile") func[1] = output[7] = true;
            else if (ff[i].name == "createfile") func[2] = output[0] = true;
            else if (ff[i].name == "readfile") func[3] = output[2] = true;
            else if (ff[i].name == "GetProcAddress") func[4] = true;
            else if (ff[i].name == "LoadLibraryA") func[5] = output[10] = true;
            else if (ff[i].name == "CreateFileMappingA") func[6] = output[8] = true;
            else if (ff[i].name == "MapViewOfFile") func[7] = output[9] = true;
            else if (ff[i].name == "SetFilePointer") func[8] = output[11] = true;
            else if (ff[i].name == "SetEndOfFile") func[9] = true;
            else if (ff[i].name == "deletefile") func[10] = output[4] = true;
            else if (ff[i].name == "writefile") func[11] = output[3] = true;
        }
        else
        {
            break;
        }
    }
    bool ans[3] = { false };
    if (func[0] && func[1] && func[2] && func[3]) ans[0] = true;
    if (func[4] && func[5] && func[6] && func[7] && func[8] && func[9]) ans[1] = true;
    if (func[10] && func[11]) ans[2] = true;
    return ans;
}

//处理文件数量

extern "C" __declspec(dllexport)string make(string x)
{
    if (x.size() <= 2) return NULL;
    if (x[x.size() - 1] == '/' && x[x.size() - 2] == '/') return x;
    else
    {
        int i;
        for (i = x.size() - 1; i > 0; i--)
        {
            if (x[i] == '/' && x[i - 1] == '/') break;
        }
        string xx(x, 0, i + 1);
        return xx;
    }
}

extern "C" __declspec(dllexport)vector<string> nnew(string x)
{
    vector<string> temp;
    string t;
    if ((int)x.size() <= 2)
    {
        temp.push_back(NULL);
        return temp;
    }
    int q, p;
    t.push_back(x[0]); temp.push_back(t);
    for (q = 1, p = 1; q < (int)x.size(); q++)
    {
        if (x[q] == '/' && x[q + 1] == '/' && x[p] == '/' && x[p + 1] == '/')
        {
            string k(x, p + 2, q - p - 2);
            temp.push_back(k);
        }
        if (x[q] == '/' && x[q + 1] == '/') p = q;
    }
    return temp;
}

extern "C" __declspec(dllexport)void build_tree(string x, int root)
{
    vector<string> temp = nnew(x);
    bool have_c = false, build = false;
    for (int i = 0; i < (int)temp.size(); i++)
    {
        have_c = false;
        for (int j = 0; j < (int)trie[root].size(); j++)
        {
            if (trie[root][j].now == temp[i])
            {
                have_c = true;
                break;
            }
        }
        if (have_c != true)
        {
            vector<tree> tre;
            tree te;
            te.now = temp[i];
            te.next = ++number;
            trie[root].push_back(te);
            trie.push_back(tre);
            build = true;
        }
        root = trie[root][trie[root].size() - 1].next;
    }
    if (build == true) file_num++;
}

extern "C" __declspec(dllexport)bool manyfiles()
{
    int fnum = 0;
    for (int i = 0; i < 14; i++) {
        if (output[i]) fnum++;
    }
    if (fnum >= 4) return true;
    else return false;
    int root = 0;
    vector<tree> rot;
    trie.push_back(rot);
    file_num = 0;
    for (int i = 0; i < (int)file.size(); i++)
    {
        string a = make(file[i]);
        if ((int)a.size() > 0) build_tree(a, root);
    }
    if (file_num > 1) return true;
    return false;
}

//判断文件是否自我复制
//如果有不同文件夹下存在相同文件，那么算为自我复制
//如果有使用到函数copyfile，且参数为自身，那么算为自我复制
//如果有使用到函数createfile并且是创建文件操作，那么检查文件内容是否和自身大部分一致，如果一致算为自我复制
//如果有使用到函数writefile，并且写的内容与自身文件内容大部分一致，那么视为自我复制

extern "C" __declspec(dllexport)void build_next(string P)
{
    int len = P.length();
    for (int i = 1; i <= len; i++)
    {
        next_P.push_back(0);
    }
    next_P[0] = -1; //next第一位的值赋为-1，next为上一位为止，后缀和前缀的最高重合度
    int i = 0, j = -1; //第一位前只有一个字母，字符串本身不能算作自己的后缀，前缀同理
    while (i < len)
    {
        if (j == -1 || P[i] == P[j]) //-1为了满足next[1]为0的情况 
        {
            i++;
            j++;
            next_P[i] = j;
        }
        else
        {
            j = next_P[j]; //本身就是一个kmp匹配，碰到不匹配的字符退回上一个前后缀重合的点，一直不符就一直退，直到j==-1
        }
    }
}

extern "C" __declspec(dllexport)int match(string P, string target2)
{
    int i = 0, j = 0;
    while (i < (int)P.length() && j < (int)target2.length()) //一定要类型转换，因为不转换的话length返回值相当于无符号，当i为负数时相当于无符号的最大
    {
        if (i == -1 || P[i] == target2[j])
        {
            i++;
            j++;
        }
        else i = next_P[i]; //退回上一个前后缀重合点
    }
    if (i == P.length()) return j - i; //如果有匹配的话，i一定是到达了P的长度，这样while循环退出
    else return -1;
}

extern "C" __declspec(dllexport)bool is_c1(string target)
{
    int k;
    if ((int)target.length() < 3) return false;
    for (k = target.size() - 1; k > 0; k--)
    {
        if (target[k] == '/' && target[k - 1] == '/') break;
    }
    string k1(target, 0, k - 1);
    string k2(target, k + 1, target.size() - k - 1);
    for (int i = 0; i < (int)file.size(); i++)
    {
        int j;
        if ((int)file[i].size() < 3) break;
        for (j = file[i].size() - 1; j > 0; j--)
        {
            if (file[i][j] == '/' && file[i][j - 1] == '/') break;
        }
        string temp1(file[i], 0, j - 1);
        string temp2(file[i], j + 1, file[i].size() - j - 1);
        if (temp2 == k2 && temp1 != k1) return true;
    }
    return false;
}

extern "C" __declspec(dllexport)bool is_c2(string target)
{
    for (int i = 0; i < (int)cpy.size(); i++)
    {
        if (cpy[i].A == target) return true;
    }
    return false;
}

extern "C" __declspec(dllexport)bool is_c3(string target)
{
    LPCSTR buffer = target.c_str();
    HANDLE Hfile = CreateFileA(buffer, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int filesize = 0;
    filesize = GetFileSize(Hfile, NULL);
    char* buff;
    buff = (char*)malloc(filesize);
    DWORD dwRead;
    if (!ReadFile(Hfile, buff, filesize, &dwRead, NULL))
    {
        printf("error!\n");
    }
    buff[filesize - 1] = '\0';
    for (int i = 0; i < (int)rt.size(); i++)
    {
        if (rt[i].content.size() < 130)
        {
            build_next(rt[i].content);
            if (match(rt[i].content, buff) != -1) return true;
        }
        else
        {
            if (strlen(buff) <= 30) return false;
            string t1(rt[i].content, 0, 30);
            string t2(rt[i].content, rt[i].content.size() - 15, 30);
            string t3(rt[i].content, rt[i].content.size() - 31, 30);
            build_next(t1);
            if (match(t1, buff) != -1)
            {
                build_next(t2);
                if (match(t2, buff) != -1)
                {
                    build_next(t3);
                    if (match(t3, buff) != -1) return true;
                }
            }
        }
    }
    return false;
}

extern "C" __declspec(dllexport)bool self_copy()
{
    if (output[5] || c_num > 3) {
        c_num = 0;
        return true;
    }
    else return false;
    for (int i = 0; i < (int)temporary.size(); i++)
    {
        if (is_c1(temporary[i]) || is_c2(temporary[i]) || is_c3(temporary[i]))
        {
            return true;
        }
    }
    return false;
}

//判断是否修改可执行文件
//如果是删除文件，直接判断参数的后缀是否满足可执行文件
//如果是writefile,那么需要判断文件的后缀
extern "C" __declspec(dllexport)bool is_wrt()
{
    for (int i = 0; i < (int)rt.size(); i++)
    {
        int filesize = 0;
        filesize = GetFileSize(rt[i].hfile, NULL);
        char* buff;
        buff = (char*)malloc(filesize);
        DWORD dwRead;
        if (!ReadFile(rt[i].hfile, buff, filesize, &dwRead, NULL))
        {
            return false;
        }
        buff[filesize] = '\0';
        if (buff[0] == 'M' && buff[1] == 'Z') return true;
    }
    return false;
}

extern "C" __declspec(dllexport)bool is_del()
{
    for (int i = 0; i < (int)deletefilec.size(); i++)
    {
        if (deletefilec[i].size() > 3)
        {
            string temp(deletefilec[i], deletefilec[i].size() - 3, 3);
            if (temp == "exe" || temp == "com" || temp == "ocx" || temp == "dll" || temp == "EXE" || temp == "COM" || temp == "OCX" || temp == "DLL") return true;
        }
    }
    return false;
}

extern "C" __declspec(dllexport)bool revise()
{
    if (output[3] && output[4]) return true;
    else return false;
    if (is_del() || is_wrt())
        return true;
    else return false;
}

//总体处理异常的函数
extern "C" __declspec(dllexport)void accident()
{
    int ot[4] = { 0 }, ott = 0;
    bool ans[6] = { false };
    int acc = 0;
    bool* ans2 = virus();
    ans[3] = ans2[0];
    ans[4] = ans2[1];
    ans[5] = ans2[2];
    ans[0] = manyfiles();
    ans[1] = self_copy();
    ans[2] = revise();
    for (int i = 0; i < 6; i++)
    {
        if (ans[i]) acc++;
    }
    for (int i = 0; i < 14; i++)
    {
        if (output[i])
        {
            ot[ott] = i;
            ott++;
            if (ott >= 4) break;
        }
    }
    if (acc > 3)
    {
        int num[3], res = 0;
        for (int i = 0; i < 6; i++)
        {
            if (ans[i] && res < 3)
            {
                num[res] = i + 1;
                res++;
            }
        }
        json info = {
            {"typ",2},
            {"num1",3},
            {"num2",ott},
            {"unu", {
                    {"u1",num[0]},
                    {"u2",num[1]},
                    {"u3",num[2]},
                }
            },
            {"api",{{"n1",output[ot[0]] ? ot[0] + 1 : 0},{"n2",output[ot[1]] ? ot[1] + 1 : 0},{"n3",output[ot[2]] ? ot[2] + 1 : 0},{"n4",output[ot[3]] ? ot[3] + 1 : 0}}}
        };
        send(ClientSocket, info.dump().c_str(), 1023, 0);
    }
    else
    {
        int num[3], res = 0;
        for (int i = 0; i < 6; i++)
        {
            if (ans[i])
            {
                num[res] = i + 1;
                res++;
            }
        }
        for (int j = res; j < 3; j++) num[j] = 0;
        json info = {
            {"typ",2},
            {"num1",acc},
            {"num2",ott},
            {"unu", {
                    {"u1",num[0]},
                    {"u2",num[1]},
                    {"u3",num[2]},
                 }
            },
            {"api",{{"n1",output[ot[0]] ? ot[0] + 1 : 0},{"n2",output[ot[1]] ? ot[1] + 1 : 0},{"n3",output[ot[2]] ? ot[2] + 1 : 0},{"n4",output[ot[3]] ? ot[3] + 1 : 0}}}
        };
        send(ClientSocket, info.dump().c_str(), 1023, 0);
    }
    ff.clear(); file.clear(); rt.clear(); cpy.clear(); deletefilec.clear(); temporary.clear(); //将当前储存的数据清除，以免妨碍下一次测试
    memset(output, 0, sizeof(output));
}

extern "C" __declspec(dllexport)void counter()
{
    countn++;
    if (countn > 10)
    {
        accident();
        countn = 0;
    }
}

static HANDLE(WINAPI* OldCreateFileA)(
    LPCSTR                lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
    ) = CreateFileA;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
)
{
    char* ta = (char*)lpFileName;
    int num = strlen(ta);
    for (int i = 0; i < num; i++)
    {
        if (ta[i] == '.')
        {
            if (strcmp(ta + i, "json") == 0) return tem = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    for (int i = num - 1; i > 0; i--) {
        if (ta[i] == '/' && ta[i - 1] == '/') {
            if (strcmp(ta + i, "http.txt") == 0) return tem = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    skeptical a;
    a.name = "createfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    char szbuffer[7][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%d", dwDesiredAccess);
    sprintf_s(szbuffer[1], "%d", dwShareMode);
    sprintf_s(szbuffer[2], "%p", lpSecurityAttributes);
    sprintf_s(szbuffer[3], "%d", dwCreationDisposition);
    sprintf_s(szbuffer[4], "%d", dwFlagsAndAttributes);
    sprintf_s(szbuffer[5], "%p", hTemplateFile);
    sprintf_s(szbuffer[6], 70, "%s", (char*)lpFileName);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CreateFile"},
        {"time",time},
        {"arg",{
            {"lpFileName",szbuffer[6]},
            {"dwDesiredAccess",szbuffer[0]},
            {"dwShareMode",szbuffer[1]},
            {"lpSecurityAttributes",szbuffer[2]},
            {"dwCreationDisposition",szbuffer[3]},
            {"dwFlagsAndAttributes",szbuffer[4]},
            {"hTemplateFile",szbuffer[5]}
         }}
    };

    temporary.push_back(szbuffer[6]);
    output[12] = true;
    counter();
    logs.emplace_back(log);
    return OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static HANDLE(WINAPI* OldCreateFileW)(
    LPCWSTR               lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
    ) = CreateFileW;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFileW(
    LPCWSTR               lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
)
{
    char* ta = (char*)lpFileName;
    int num = strlen(ta);
    for (int i = 0; i < num; i++)
    {
        if (ta[i] == '.')
        {
            if (strcmp(ta + i, "json") == 0) return tem = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    for (int i = num - 1; i > 0; i--) {
        if (ta[i] == '/' && ta[i - 1] == '/') {
            if (strcmp(ta + i, "http.txt") == 0) return tem = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    skeptical a;
    a.name = "createfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    char szbuffer[7][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%d", dwDesiredAccess);
    sprintf_s(szbuffer[1], "%d", dwShareMode);
    sprintf_s(szbuffer[2], "%p", lpSecurityAttributes);
    sprintf_s(szbuffer[3], "%d", dwCreationDisposition);
    sprintf_s(szbuffer[4], "%d", dwFlagsAndAttributes);
    sprintf_s(szbuffer[5], "%p", hTemplateFile);
    sprintf_s(szbuffer[6], 70, "%s", (char*)lpFileName);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CreateFile"},
        {"time",time},
        {"arg",{
            {"lpFileName",szbuffer[6]},
            {"dwDesiredAccess",szbuffer[0]},
            {"dwShareMode",szbuffer[1]},
            {"lpSecurityAttributes",szbuffer[2]},
            {"dwCreationDisposition",szbuffer[3]},
            {"dwFlagsAndAttributes",szbuffer[4]},
            {"hTemplateFile",szbuffer[5]}
         }}
    };

    temporary.push_back(szbuffer[6]);
    output[13] = true;
    counter();
    logs.emplace_back(log);
    return OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName,  //指向文件名的指针
    DWORD dwDesiredAccess,  //访问模式（读/写）
    DWORD dwShareMode,  //共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,  //指向安全模式的指针
    DWORD dwCreationDisposition,  //如何创建
    DWORD dwFlagsAndAttributes, //文件属性
    HANDLE hTemplateFile //用于复制文件句柄
    ) = CreateFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
)
{
    GetLocalTime(&st);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE); //此函数把宽字符串转换成指定的新的字符串，如ANSI，UTF8等，新字符串不必是多字节字符集。 
    //num是缓冲区大小，之后开辟pchar容纳新的字符串
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    //CP_OEMCP:当前系统OEM代码页，一种原始设备制造商硬件扫描码
    //dwFlags：指定如何处理没有转换的字符，但不设此参数函数会运行的更快一些，我都是把它设为0。这里是NULL
    //lpWideCharStr： 待转换的宽字符串。这里即文件名
    //cchWideChar： 待转换宽字符串的长度，-1表示转换到字符串结尾。
    //lpMultiByteStr： 接收转换后输出新串的缓冲区。
    //cbMultiByte：输出缓冲区大小，如果为0，lpMultiByteStr将被忽略，函数将返回所需缓冲区大小而不使用lpMultiByteStr。
    //lpDefaultChar： 指向字符的指针
    //lpUsedDefaultChar：开关变量的指针，用以表明是否使用过默认字符
    for (int i = 0; i < num; i++) {
        if (pchar[i] == '.') {
            if (strcmp(pchar + i, "json") == 0) return tem = OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    for (int i = num - 1; i > 0; i--) {
        if (pchar[i] == '/' && pchar[i - 1] == '/') {
            if (strcmp(pchar + i, "http.txt") == 0) return tem = OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    skeptical a;
    a.name = "createfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    char szbuffer[6][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%d", dwDesiredAccess);
    sprintf_s(szbuffer[1], "%d", dwShareMode);
    sprintf_s(szbuffer[2], "%p", lpSecurityAttributes);
    sprintf_s(szbuffer[3], "%d", dwCreationDisposition);
    sprintf_s(szbuffer[4], "%d", dwFlagsAndAttributes);
    sprintf_s(szbuffer[5], "%p", hTemplateFile);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CreateFile"},
        {"time",time},
        {"arg",{
            {"lpFileName",pchar},
            {"dwDesiredAccess",szbuffer[0]},
            {"dwShareMode",szbuffer[1]},
            {"lpSecurityAttributes",szbuffer[2]},
            {"dwCreationDisposition",szbuffer[3]},
            {"dwFlagsAndAttributes",szbuffer[4]},
            {"hTemplateFile",szbuffer[5]}
         }}
    };

    temporary.push_back(pchar);
    c_num++;
    counter();
    logs.emplace_back(log);
    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static BOOL(WINAPI* OldCloseHandle)(HANDLE hObject) = CloseHandle;

extern "C" __declspec(dllexport)BOOL WINAPI NewCloseHandle(HANDLE hObject)
{
    GetLocalTime(&st);
    char szbuffer[70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer, "%p", hObject);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CloseHandle"},
        {"time",time},
        {"arg",{
            {"hObject",szbuffer}
         }}
    };

    //logs.emplace_back(log);
    return OldCloseHandle(hObject);
}

static BOOL(WINAPI* OldReadFile)(HANDLE hfile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = ReadFile;
//文件句柄，用来接收从文件中读出的数据的缓冲区指针，指明要读的字节总数，实际传输的字节总数
extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(HANDLE hfile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "readfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[5][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hfile);
    sprintf_s(szbuffer[1], "%p", lpBuffer);
    sprintf_s(szbuffer[2], "%d", nNumberOfBytesToRead);
    sprintf_s(szbuffer[3], "%d", lpNumberOfBytesRead);
    sprintf_s(szbuffer[4], "%p", lpOverlapped);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","ReadFile"},
        {"time",time},
        {"arg",{
            {"hfile",szbuffer[0]},
            {"lpBuffer",szbuffer[1]},
            {"nNumberOfBytesToRead",szbuffer[2]},
            {"lpNumberOfBytesRead",szbuffer[3]},
            {"lpOverlapped",szbuffer[4]}
         }}
    };

    counter();
    logs.emplace_back(log);
    return OldReadFile(hfile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

static BOOL(WINAPI* OldWriteFile)(HANDLE hFile,                   // handle to file
    LPCVOID lpBuffer,               // data buffer
    DWORD nNumberOfBytesToWrite,     // number of bytes towrite
    LPDWORD lpNumberOfBytesWritten, // number of bytes written
    LPOVERLAPPED lpOverlapped        //overlapped buffer
    ) = WriteFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
    HANDLE hFile,                   // handle to file
    LPCVOID lpBuffer,               // data buffer
    DWORD nNumberOfBytesToWrite,     // number of bytes towrite
    LPDWORD lpNumberOfBytesWritten, // number of bytes written
    LPOVERLAPPED lpOverlapped        //overlapped buffer
)
{
    if (hFile == tem) return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    GetLocalTime(&st);
    skeptical a;
    a.name = "writefile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    rtfile b;
    string ktemp;
    if (strlen((char*)lpBuffer) > 100) {
        string ttemp((char*)lpBuffer, 0, 100);
        ktemp = ttemp;
    }
    else ktemp = (char*)lpBuffer;
    b.content = ktemp;
    b.hfile = hFile;
    rt.push_back(b);
    char szbuffer[5][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hFile);
    sprintf_s(szbuffer[2], "%d", nNumberOfBytesToWrite);
    sprintf_s(szbuffer[3], "%d", lpNumberOfBytesWritten);
    sprintf_s(szbuffer[4], 70, "%p", lpOverlapped);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","WriteFile"},
        {"time",time},
        {"arg",{
            {"hFile",szbuffer[0]},
            {"lpBuffer",ktemp},
            {"nNumberOfBytesToWrite",szbuffer[2]},
            {"lpNumberOfBytesWritten",szbuffer[3]},
            {"lpOverlapped",szbuffer[4]}
         }}
    };

    counter();
    logs.emplace_back(log);
    return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

static BOOL(WINAPI* OldDeleteFile)(_In_ LPCWSTR lpFileName) = DeleteFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewDeleteFile(_In_ LPCWSTR lpFileName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "deletefile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    string b = (char*)lpFileName;
    deletefilec.push_back(b);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE); //此函数把宽字符串转换成指定的新的字符串，如ANSI，UTF8等，新字符串不必是多字节字符集。 
    //num是缓冲区大小，之后开辟pchar容纳新的字符串
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    char time[70] = { 0 };
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","DeleteFile"},
        {"time",time},
        {"arg",{
            {"hObject",pchar}
         }}
    };

    counter();
    logs.emplace_back(log);
    return OldDeleteFile(lpFileName);
}

static BOOL(WINAPI* OldCopyFile)(LPCWSTR lpexistingfilename, LPCWSTR lpnewfilename, BOOL bfailifexists) = CopyFile;

extern "C" __declspec(dllexport)BOOL NewCopyFile(LPCWSTR lpexistingfilename, LPCWSTR lpnewfilename, BOOL bfailifexists)
{
    GetLocalTime(&st);
    cpyfile a;
    a.A = (char*)lpexistingfilename;
    a.B = (char*)lpnewfilename;
    a.w = bfailifexists;
    cpy.push_back(a);
    char szbuffer[3][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], 70, "%s", (char*)lpexistingfilename);
    sprintf_s(szbuffer[1], 70, "%s", (char*)lpnewfilename);
    sprintf_s(szbuffer[2], "%d", bfailifexists);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CopyFile"},
        {"time",time},
        {"arg",{
            {"lpexistingfilename",szbuffer[0]},
            {"lpnewfilename",szbuffer[1]},
            {"bfailifexists",szbuffer[2]},
         }}
    };
    output[5] = true;
    temporary.push_back(szbuffer[0]);
    counter();
    logs.emplace_back(log);
    return OldCopyFile(lpexistingfilename, lpnewfilename, bfailifexists);
}

static HANDLE(WINAPI* OldFindFirstFile)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData) = FindFirstFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewFindFirstFile(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "findfirstfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    char szbuffer[2][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], 70, "%s", (char*)lpFileName);
    sprintf_s(szbuffer[1], "%p", lpFindFileData);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","FindFirstFile"},
        {"time",time},
        {"arg",{
            {"lpFileName",szbuffer[0]},
            {"lpFindFileData",szbuffer[1]},
         }}
    };
    temporary.push_back(szbuffer[0]);
    logs.emplace_back(log);
    counter();
    return OldFindFirstFile(lpFileName, lpFindFileData);
}

static BOOL(WINAPI* OldFindNextFile)(_In_   HANDLE hFindFile, _Out_  LPWIN32_FIND_DATA lpFindFileData) = FindNextFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewFindNextFile(_In_   HANDLE hFindFile, _Out_  LPWIN32_FIND_DATA lpFindFileData)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "findnextfile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[2][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hFindFile);
    sprintf_s(szbuffer[1], "%p", lpFindFileData);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","FindNextFile"},
        {"time",time},
        {"arg",{
            {"hFindFile",szbuffer[0]},
            {"lpFindFileData",szbuffer[1]},
         }}
    };

    logs.emplace_back(log);
    counter();
    return OldFindNextFile(hFindFile, lpFindFileData);
}

static FARPROC(WINAPI* OldGetProcAddress)(HMODULE hModule, LPCSTR lpProcName) = GetProcAddress;

extern "C" __declspec(dllexport) FARPROC WINAPI NewGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "GetProcAddress";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[2][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hModule);
    sprintf_s(szbuffer[1], 70, "%s", lpProcName);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","GetProcAddress"},
        {"time",time},
        {"arg",{
            {"hModule",szbuffer[0]},
            {"lpProcName",szbuffer[1]},
         }}
    };
    //logs.emplace_back(log);
    return OldGetProcAddress(hModule, lpProcName);
}

static HMODULE(WINAPI* OldLoadLibraryA)(_In_ LPCSTR lpFileName) = LoadLibraryA;

extern "C" __declspec(dllexport) HMODULE WINAPI NewLoadLibraryA(_In_ LPCSTR lpFileName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "LoadLibraryA";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    char szbuffer[70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer, 70, "%s", lpFileName);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","LoadLibraryA"},
        {"time",time},
        {"arg",{
            {"lpFileName",szbuffer},
         }}
    };

    logs.emplace_back(log);
    counter();
    return OldLoadLibraryA(lpFileName);
}

static HMODULE(WINAPI* OldLoadLibraryW)(_In_ LPCTSTR lpFileName) = LoadLibraryW;

extern "C" __declspec(dllexport) HMODULE WINAPI NewLoadLibraryW(_In_ LPCTSTR lpFileName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "LoadLibraryA";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    string f = (char*)lpFileName;
    file.push_back(f);
    counter();
    return OldLoadLibraryW(lpFileName);
}

static HANDLE(WINAPI* OldCreateFileMappingA)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName) = CreateFileMappingA;

extern "C" __declspec(dllexport) HANDLE NewCreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "CreateFileMappingA";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[6][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hFile);
    sprintf_s(szbuffer[1], "%p", lpAttributes);
    sprintf_s(szbuffer[2], "%d", flProtect);
    sprintf_s(szbuffer[3], "%d", dwMaximumSizeHigh);
    sprintf_s(szbuffer[4], "%d", dwMaximumSizeLow);
    sprintf_s(szbuffer[5], 70, "%s", lpName);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","CreateFileMappingA"},
        {"time",time},
        {"arg",{
            {"hFile",szbuffer[0]},
            {"lpAttributes",szbuffer[1]},
            {"flProtect",szbuffer[2]},
            {"dwMaximumSizeHigh",szbuffer[3]},
            {"dwMaximumSizeLow",szbuffer[4]},
            {"lpName",szbuffer[5]},
         }}
    };

    logs.emplace_back(log);
    counter();
    return OldCreateFileMappingA(hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}

static HANDLE(WINAPI* OldCreateFileMappingW)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCTSTR lpName) = CreateFileMappingW;

extern "C" __declspec(dllexport) HANDLE NewCreateFileMappingW(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCTSTR lpName)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "CreateFileMappingA";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    counter();
    return OldCreateFileMappingW(hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}

static LPVOID(WINAPI* OldMapViewOfFile)(__in HANDLE hFileMappingObject, __in DWORD dwDesiredAccess, __in DWORD dwFileOffsetHigh, __in DWORD dwFileOffsetLow, __in SIZE_T dwNumberOfBytesToMap) = MapViewOfFile;

extern "C" __declspec(dllexport) LPVOID NewMapViewOfFile(__in HANDLE hFileMappingObject, __in DWORD dwDesiredAccess, __in DWORD dwFileOffsetHigh, __in DWORD dwFileOffsetLow, __in SIZE_T dwNumberOfBytesToMap)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "MapViewOfFile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[5][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hFileMappingObject);
    sprintf_s(szbuffer[1], "%d", dwDesiredAccess);
    sprintf_s(szbuffer[2], "%d", dwFileOffsetHigh);
    sprintf_s(szbuffer[3], "%d", dwFileOffsetLow);
    sprintf_s(szbuffer[4], "%d", dwNumberOfBytesToMap);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","MapViewOfFile"},
        {"time",time},
        {"arg",{
            {"hFileMappingObject",szbuffer[0]},
            {"dwDesiredAccess",szbuffer[1]},
            {"dwFileOffsetHigh",szbuffer[2]},
            {"dwFileOffsetLow",szbuffer[3]},
            {"dwNumberOfBytesToMap",szbuffer[4]},
         }}
    };

    logs.emplace_back(log);
    counter();
    return OldMapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
}

static DWORD(WINAPI* OldSetFilePointer)(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod) = SetFilePointer;

extern "C" __declspec(dllexport) DWORD NewSetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "SetFilePointer";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[4][70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer[0], "%p", hFile);
    sprintf_s(szbuffer[1], "%d", lDistanceToMove);
    sprintf_s(szbuffer[2], "%d", lpDistanceToMoveHigh);
    sprintf_s(szbuffer[3], "%d", dwMoveMethod);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","SetFilePointer"},
        {"time",time},
        {"arg",{
            {"hFile",szbuffer[0]},
            {"lDistanceToMove",szbuffer[1]},
            {"lpDistanceToMoveHigh",szbuffer[2]},
            {"dwMoveMethod",szbuffer[3]},
         }}
    };

    logs.emplace_back(log);
    counter();
    return OldSetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}

static BOOL(WINAPI* OldSetEndOfFile)(__in HANDLE hFile) = SetEndOfFile;

extern "C" __declspec(dllexport) BOOL NewSetEndOfFile(__in HANDLE hFile)
{
    GetLocalTime(&st);
    skeptical a;
    a.name = "SetEndOfFile";
    a.time = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
    ff.push_back(a);
    char szbuffer[70] = { 0 };
    char time[70] = { 0 };
    sprintf_s(szbuffer, "%p", hFile);
    sprintf_s(time, "%d-%d-%d %02d: %02d: %03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    json log = {
        {"type","SetEndOfFile"},
        {"time",time},
        {"arg",{
            {"hFile",szbuffer},
         }}
    };
    logs.emplace_back(log);
    counter();
    return OldSetEndOfFile(hFile);
}

// 注册表操作--------------------------------------------------------------------------------------
vector<pair<int, int>> CreateOrOpen;
const string selfRunKey[9] = {
    "CurrentVersion\\Run",
    "CurrentVersion\\Windows\\load",
    "CurrentVersion\\Winlogon\\Userinit",
    "CurrentVersion\\Policies\\Explorer\\Run",
    "CurrentVersion\\RunServicesOnce",
    "CurrentVersion\\RunServices",
    "CurrentVersion\\RunOnce\\Setup",
    "CurrentVersion\\RunOnce",
    "CurrentVersion\\RunOnceEx"
    //"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
};
/**
 * @brief 注册表异常检测, 只有发生设置操作才会检测
*/
void regCheck() {
    // 最新打开或创建api数据
    if (CreateOrOpen.empty()) return;
    int api = CreateOrOpen.back().first;
    int idx = CreateOrOpen.back().second;
    string subKeyName = logs[idx]["arg"]["lpSubKey"].get<string>();
    
 
    // 判断是否修改自启动
    for (int i = 0; i < 9; i++) {
        if (subKeyName.find(selfRunKey[i]) != subKeyName.npos) {
            json info = {
                {"typ", 4},
                {"num1",1},
                {"num2",2},
                {"unu", {{"u1", 1},{"u2",0}, {"u3",0}}},
                {"api", {{"n1", api}, {"n2", 3}, {"n3", 0},{"n4", 0}}}
            };
            send(ClientSocket, info.dump().c_str(), 1023, 0);
            return;
        }
    }
    // 判断是否强制隐藏exe文件扩展名
    if (subKeyName.find("exefile") != subKeyName.npos) {
        json info = {
            {"typ", 4},
            {"num1",1},
            {"num2",2},
            {"unu", {{"u1", 2},{"u2",0}, {"u3",0}}},
            {"api", {{"n1", api}, {"n2", 3}, {"n3", 0},{"n4", 0}}}
        };
        send(ClientSocket, info.dump().c_str(), 1023, 0);
        return;
    }

    // 判断是否通过注册表禁用程序
    if (subKeyName.find("CurrentVersion\\Image File Execution Options\\") != subKeyName.npos) {
        json info = {
            {"typ", 4},
            {"num1",1},
            {"num2",2},
            {"unu", {{"u1", 3},{"u2",0}, {"u3",0}}},
            {"api", {{"n1", api}, {"n2", 3}, {"n3", 0},{"n4", 0}}}
        };
        send(ClientSocket, info.dump().c_str(), 1023, 0);
        return;
    }
    // 可能禁止编辑注册表
    if (subKeyName.find("CurrentVersion\\Policies\\System") != subKeyName.npos) {
        json info = {
            {"typ", 4},
            {"num1",1},
            {"num2",2},
            {"unu", {{"u1", 4},{"u2",0}, {"u3",0}}},
            {"api", {{"n1", api}, {"n2", 3}, {"n3", 0},{"n4", 0}}}
        };
        send(ClientSocket, info.dump().c_str(), 1023, 0);
        return;
    }
}

/**
 * @brief 打开指定的键或子键。如果要打开的键不存在的话，本函数会试图建立它
 * @param hKey                  主键
 * @param lpSubKey              指向以零结尾的字符串的指针，包含将要创建或打开的子键的名称
 * @param Reserved              保留值，设为0
 * @param lpClass               指向包含键类型的字符串。如果该键已经存在，则忽略该参数。
 * @param dwOptions             为新创建的键设置一定的属性
 * @param samDesired            设置对键访问的权限
 * @param lpSecurityAttributes  一个指向SECURITY_ATTRIBUTES结构的指针，确定返回的句柄是否被子处理过程继承
 * @param phkResult             一个指向新创建或打开的键的句柄的指针
 * @param lpdwDisposition       明键是被创建还是被打开的
 * @return
*/
static LSTATUS(WINAPI* OldRegCreateKeyEx)(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, const LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition) {
    // 保存API信息
    //hKeys.insert(hKey);
    char to_string[9][70] = { 0 };
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hKey);
    sprintf_s(to_string[1], "%ws", lpSubKey);
    sprintf_s(to_string[2], "%08X", Reserved);
    sprintf_s(to_string[3], "%ws", lpClass);
    sprintf_s(to_string[4], "%08X", dwOptions);
    sprintf_s(to_string[5], "%08X", samDesired);
    sprintf_s(to_string[6], "%08X", lpSecurityAttributes);
    sprintf_s(to_string[7], "%08X", phkResult);
    sprintf_s(to_string[8], "%08X", lpdwDisposition);

    // 写入日志
    json log = {
    {"type", "RegCreateKeyEx"},
    {"time", time},
    {"arg", {
        {"hKey", to_string[0]},
        {"lpSubKey", to_string[1]},
        {"Reserved", to_string[2]},
        {"lpClass", to_string[3]},
        {"dwOption", to_string[4]},
        {"samDesired", to_string[5]},
        {"lpSecurityAttributes", to_string[6]},
        {"phkResult", to_string[7]},
        {"lpdwDisposition", to_string[8]}}
    }
    };
    logs.emplace_back(log);
    CreateOrOpen.push_back({ 1, logs.size() - 1 });
    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

/**
 * @brief 打开注册表中的一个子键
 * @param hKey          主键句柄
 * @param lpSubKey      Reserved为保留值，必须设置为0
 * @param ulOptions     指向包含值名的字符串指针
 * @param samDesired    确定了设置的值的类型
 * @param phkResult     指向包含数据的缓冲区的指针
 * @return              以字节为单位，指定数据的长度
*/
static LSTATUS(WINAPI* OldRegOpenKeyEx)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) = RegOpenKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult) {
    // 保存API信息
    //hKeys.insert(hKey);
    char to_string[5][70] = { 0 };
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hKey);
    sprintf_s(to_string[1], "%ws", lpSubKey);
    sprintf_s(to_string[2], "%08X", ulOptions);
    sprintf_s(to_string[3], "%08X", samDesired);
    sprintf_s(to_string[4], "%08X", phkResult);
    // 写入日志
    json log = {
        {"type", "RegOpenKeyEx"},
        {"time", time},
        {"arg", {
            {"hKey",        to_string[0]},
            {"lpSubKey",    to_string[1]},
            {"ulOptions",   to_string[2]},
            {"samDesired",  to_string[3]},
            {"phkResult",   to_string[4]}
        }}
    };
    logs.emplace_back(log);
    CreateOrOpen.push_back({ 2, logs.size() - 1 });
    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}


/**
 * @brief
 * @param hKey 设置注册表中的一个键值
 * @param lpValueName 主键
 * @param Reserved    指向包含值名的字符串指针
 * @param dwType      Reserved为保留值，必须设置为0
 * @param lpData      确定了设置的值的类型
 * @param cbData      指向包含数据的缓冲区的指针
 * @return            以字节为单位，指定数据的长度
*/
static LSTATUS(WINAPI* OldRegSetValueEx)(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) = RegSetValueEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE * lpData,
    DWORD      cbData) {
    // 保存API信息
    char to_string[6][70] = { 0 };
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hKey);
    sprintf_s(to_string[1], "%ws", lpValueName);
    sprintf_s(to_string[2], "%08X", Reserved);
    sprintf_s(to_string[3], "%08X", dwType);
    sprintf_s(to_string[4], "%ws", lpData);
    sprintf_s(to_string[5], "%08X", cbData);
    // 写入日志
    json log = {
        {"type", "RegSetValueEx"},
        {"time", time},
        {"arg", {
            {"hKey",        to_string[0]},
            {"lpValueName", to_string[1]},
            {"Reserved",    to_string[2]},
            {"dwType",      to_string[3]},
            {"lpData",      to_string[4]},
            {"cbData",      to_string[5]}
        }}
    };
    logs.emplace_back(log);
    // 异常检测
    regCheck();
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

/**
 * @brief 查询一个subKey的value
 * @param hKey          主键句柄
 * @param lpValueName   key名
 * @param lpReserved    保留值0
 * @param lpType        值类型
 * @param lpData        数据缓冲区指针
 * @param lpcbData      字节为单位，数据长度
 * @return
*/
static LSTATUS(WINAPI* OldRegQueryValueEx)(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) = RegQueryValueEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegQueryValueEx(
    HKEY    hKey,
    LPCWSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE  lpData,
    LPDWORD lpcbData) {
    // 保存API信息
    //if (hKeys.find(hKey) == hKeys.end())
    //    return OldRegQueryValueEx(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);

    char to_string[6][70] = { 0 };
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hKey);
    sprintf_s(to_string[1], "%ws", lpValueName);
    sprintf_s(to_string[2], "%08X", lpReserved);
    sprintf_s(to_string[3], "%08X", lpType);
    sprintf_s(to_string[4], "%ws", lpData);
    sprintf_s(to_string[5], "%08X", lpcbData);
    // 写入日志
    json log = {
        {"type", "RegQueryValueEx"},
        {"time", time},
        {"arg", {
            {"hKey",        to_string[0]},
            {"lpValueName", to_string[1]},
            {"lpReserved",  to_string[2]},
            {"lpType",      to_string[3]},
            {"lpData",      to_string[4]},
            {"lpcbData",    to_string[5]}
        }}
    };
    logs.emplace_back(log);
    return OldRegQueryValueEx(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

/**
 * @brief 关闭hkey指向的句柄
 * @param hKey 主键句柄
 * @return
*/
static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey) {
    // 保存API信息
    char to_string[70] = { 0 };
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string, "%08X", hKey);
    // 写入日志
    json log = {
        {"type", "RegCloseKey"},
        {"time", time},
        {"arg", {
            {"hKey", to_string},
        }}
    };
    logs.emplace_back(log);
    return OldRegCloseKey(hKey);
}

// 堆操作
static std::unordered_map<HANDLE, bool> hHeaps;

//struct AllocInfo {
//    std::string time;
//    std::string hHeap;
//    std::string dwFlags;
//    std::string dwBytes;
//};
//
//struct FreeInfo {
//    std::string time;
//    std::string hHeap;
//    std::string dwFlags;
//    std::string lpMem;
//};
//
//static std::unordered_map<HANDLE, AllocInfo> AllocInfos;
//static std::unordered_map<HANDLE, FreeInfo> FreeInfos;

static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) {
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    char time[70] = { 0 };
    char to_string[4][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("HeapCreate Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", fIOoptions);
    sprintf_s(to_string[1], "%08X", dwInitialSize);
    sprintf_s(to_string[2], "%08X", dwMaximumSize);
    sprintf_s(to_string[3], "%08X", hHeap);
    hHeaps.insert(std::pair<HANDLE, bool>(hHeap, false));

    json log = {
        {"type","HeapCreate"},
        {"time",time},
        {"arg", {
            {"fIOoptions", to_string[0]},
            {"dwInitialSize", to_string[1]},
            {"dwMaximumSize", to_string[2]},
            {"HANDLE", to_string[3]}
        }}
    };

    logs.emplace_back(log);
    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap) {
    char time[70] = { 0 };
    char to_string[70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("HeapDestroy Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string, "%08X", hHeap);
    if (hHeaps.find(hHeap) == hHeaps.end()) {
        return OldHeapDestroy(hHeap);
    }
    else {
        hHeaps.erase(hHeap);
    }

    json log = {
       {"type","HeapDestroy"},
       {"time",time},
       {"arg", {
           {"HANDLE",to_string}
        }}
    };

    logs.emplace_back(log);
    return OldHeapDestroy(hHeap);
}

static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;

extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {
    if (hHeaps.find(hHeap) == hHeaps.end()) {
        return OldHeapAlloc(hHeap, dwFlags, dwBytes);
    }
    else {
        hHeaps.find(hHeap)->second = true;
    }
    char time[70] = { 0 };
    char to_string[3][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("HeapAlloc Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hHeap);
    sprintf_s(to_string[1], "%08X", dwFlags);
    sprintf_s(to_string[2], "%08X", dwBytes);
    //AllocInfo info = { time, to_string[0], to_string[1], to_string[2] };
    //AllocInfos[hHeap] = info;

    json log = {
    { "type","HeapAlloc" },
    {"time",time},
    { "args", {
        {"hHeap",to_string[0]},
        {"dwFlags",to_string[1]},
        {"lpMem",to_string[2]}
    }}
    };

    logs.emplace_back(log);
    return OldHeapAlloc(hHeap, dwFlags, dwBytes);
}

static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;

extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) {
    char time[70] = { 0 };
    char to_string[3][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("HeapFree Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", hHeap);
    sprintf_s(to_string[1], "%08X", dwFlags);
    sprintf_s(to_string[2], "%08X", lpMem);
    //FreeInfo info = { time, to_string[0], to_string[1], to_string[2] };
    //FreeInfos[hHeap] = info;
    if (hHeaps.find(hHeap) == hHeaps.end()) {
        return OldHeapFree(hHeap, dwFlags, lpMem);
    }
    else if (hHeaps.find(hHeap)->second == true) {
        hHeaps.find(hHeap)->second = false;
    }
    else {
        //std::string arg1 = "hHeap:" + AllocInfos.find(hHeap)->second.hHeap + " " +
        //    "dwFlags:" + AllocInfos.find(hHeap)->second.dwFlags + " " +
        //    "dwBytes:" + AllocInfos.find(hHeap)->second.dwBytes;
        //
        //std::string arg2 = "hHeap:" + FreeInfos.find(hHeap)->second.hHeap + " " +
        //    "dwFlags:" + FreeInfos.find(hHeap)->second.dwFlags + " " +
        //    "lpMem:" + FreeInfos.find(hHeap)->second.lpMem;
        //
        //std::string arg3 = "hHeap:" + (std::string)to_string[0] + " " +
        //    "dwFlags:" + (std::string)to_string[1] + " " +
        //    "lpMem:" + (std::string)to_string[2];

        //std::string str = time;

        json info = {
            {"typ", 0},
            {"num1", 1},
            {"num2", 3},
            {"unu", {{"u1", 1}, {"u2", 0}, {"u3", 0}}},
            {"api", {{"n1", 3}, {"n2", 4}, {"n3", 4}, {"n4", 0}}}
        };

        send(ClientSocket, info.dump().c_str(), 1023, 0);
    }

    json log = {
        {"type","HeapFree" },
        {"time",time},
        {"args", {
            {"hHeap", (std::string)to_string[0]},
            {"dwFlags", (std::string)to_string[1]},
            {"lpMem", (std::string)to_string[2]}
        }}
    };
    logs.emplace_back(log);
    return OldHeapFree(hHeap, dwFlags, lpMem);
}

// 网络操作
static SOCKET(WINAPI* Oldsocket)(
    int af,
    int type,
    int protocol
    ) = socket;

extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol) {
    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    char to_string[3][70] = { 0 };
    sprintf_s(to_string[0], "%08X", af);
    sprintf_s(to_string[1], "%08X", type);
    sprintf_s(to_string[2], "%08X", protocol);

    json log = {
        {"type", "socket"},
        {"time", time},
        {"arg", {
            {"af", to_string[0]},
            {"type", to_string[1]},
            {"protocol", to_string[2]},
        }}
    };

    json info = {
        {"typ", 1},
        {"num1",1},
        {"num2",1},
        {"unu", {{"u1", 1},{"u2",0}, {"u3",0}}},
        {"api", {{"n1", 1}, {"n2", 0}, {"n3", 0},{"n4", 0}}}
    };

    send(ClientSocket, info.dump().c_str(), 1024, 0);
    logs.emplace_back(log);
    return Oldsocket(af, type, protocol);
}

static int (WINAPI* Oldbind)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = bind;

extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen) {
    char time[70] = { 0 };
    char to_string[5][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("Bind Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", s);
    sprintf_s(to_string[1], "%08X", name);
    sprintf_s(to_string[2], "%08X", namelen);
    //sprintf_s(to_string[3], "%s", inet_ntoa(sock->sin_addr));
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    inet_ntop(AF_INET, (void*)&sock->sin_addr, to_string[3], 16);
    sprintf_s(to_string[4], "%d", ntohs(sock->sin_port));

    json log = {
        {"type", "Bind"},
        {"time", time},
        {"arg", {
            {"s", to_string[0]},
            {"name", to_string[1]},
            {"namelen", to_string[2]},
            {"IP",to_string[3]},
            {"port",to_string[4]}
        }}
    };

    logs.emplace_back(log);
    return Oldbind(s, name, namelen);
}

static int (WINAPI* Oldsend)(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
    ) = send;

extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
    if (buf[0] == '{') return Oldsend(s, buf, len, flags);
    char time[70] = { 0 };
    char to_string[5][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("Send Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", s);
    sprintf_s(to_string[1], "%08X", buf);
    sprintf_s(to_string[2], "%08X", len);
    sprintf_s(to_string[3], "%08X", flags);
    sprintf_s(to_string[4], "%s", buf);

    json log = {
        {"type", "Send"},
        {"time", time},
        {"arg", {
            {"s", to_string[0]},
            {"buf", to_string[1]},
            {"len", to_string[2]},
            {"flags",to_string[3]},
            {"data",to_string[4]}
        }}
    };
    json info = {
        {"typ", 1},
        {"num1",1},
        {"num2",1},
        {"unu", {{"u1", 3},{"u2",0}, {"u3",0}}},
        {"api", {{"n1", 3}, {"n2", 0}, {"n3", 0},{"n4", 0}}}
    };

    send(ClientSocket, info.dump().c_str(), 1024, 0);

    logs.emplace_back(log);
    return Oldsend(s, buf, len, flags);
}

static int (WINAPI* Oldconnect)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = connect;

extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
    char time[70] = { 0 };
    char to_string[5][70] = { 0 };
    GetLocalTime(&st);
    OutputDebugStringA("Connect Hooked\n");
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    sprintf_s(to_string[0], "%08X", s);
    sprintf_s(to_string[1], "%08X", name);
    sprintf_s(to_string[2], "%08X", namelen);
    //sprintf_s(to_string[3], "%s", inet_ntoa(sock->sin_addr));
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    inet_ntop(AF_INET, (void*)&sock->sin_addr, to_string[3], 16);
    sprintf_s(to_string[4], "%d", ntohs(sock->sin_port));

    json log = {
        {"type", "Connect"},
        {"time", time},
        {"arg", {
            {"s", to_string[0]},
            {"name", to_string[1]},
            {"namelen", to_string[2]},
            {"IP",to_string[3]},
            {"port",to_string[4]}
        }}
    };
    json info = {
        {"typ", 1},
        {"num1",1},
        {"num2",1},
        {"unu", {{"u1", 2},{"u2",0}, {"u3",0}}},
        {"api", {{"n1", 2}, {"n2", 0}, {"n3", 0},{"n4", 0}}}
    };

    send(ClientSocket, info.dump().c_str(), 1024, 0);
    logs.emplace_back(log);
    return Oldconnect(s, name, namelen);
}

char httpText[65535] = { '0' };
bool flag = false;

static int (WINAPI* Oldrecv)(
    SOCKET     s,
    char* buf,
    int        len,
    int        flags
    ) = recv;

extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags) {

    char time[70] = { 0 };
    GetLocalTime(&st);
    sprintf_s(time, "%d-%d-%d %02d:%02d:%02d:%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    char to_string[4][70] = { 0 };
    sprintf_s(to_string[0], "%08X", s);
    sprintf_s(to_string[1], "%08X", buf);
    sprintf_s(to_string[2], "%08X", len);
    sprintf_s(to_string[3], "%08x", flags);

    int httpflag = 0;
    if (buf[0] == 'H' && buf[1] == 'T' && buf[2] == 'T' && buf[3] == 'P') httpflag = 1;

    json log = {
        {"type", "Receive"},
        {"time", time},
        {"arg", {
            {"sock", to_string[0]},
            {"buf", to_string[1]},
            {"len", to_string[2]},
            {"flags", to_string[3]},
        }}
    };

    json info = {
        {"typ", 1},
        {"num1",(httpflag ? 2 : 1)},
        {"num2",1},
        {"unu", {{"u1", 4},{"u2",(httpflag ? 5 : 0)}, {"u3",0}}},
        {"api", {{"n1", 4}, {"n2", 0}, {"n3", 0},{"n4", 0}}}
    };
    sprintf_s(httpText, "%s", buf);
    flag = httpflag ? true : false;

    send(ClientSocket, info.dump().c_str(), 1024, 0);
    logs.emplace_back(log);
    return Oldrecv(s, buf, len, flags);
}

void saveHttpText() {
    std::ofstream fp("http.txt", std::ios::out);
    fp << httpText << std::endl;
    fp.close();
}

static void* (__cdecl* Oldmemcpy)(void* _Dst, const void* _Src, size_t _Size) = memcpy;

extern "C" __declspec(dllexport)void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size) {
    printf("memcpy hooked\n");
    return Oldmemcpy(_Dst, _Src, _Size);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SocketCreate();
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldCreateFileA, NewCreateFileA);
        DetourAttach(&(PVOID&)OldCreateFileW, NewCreateFileW);
        DetourAttach(&(PVOID&)OldCloseHandle, NewCloseHandle);
        DetourAttach(&(PVOID&)OldDeleteFile, NewDeleteFile);
        DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
        DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourAttach(&(PVOID&)OldFindFirstFile, NewFindFirstFile);
        DetourAttach(&(PVOID&)OldFindNextFile, NewFindNextFile);
        DetourAttach(&(PVOID&)OldGetProcAddress, NewGetProcAddress);
        DetourAttach(&(PVOID&)OldLoadLibraryA, NewLoadLibraryA);
        DetourAttach(&(PVOID&)OldLoadLibraryW, NewLoadLibraryW);
        DetourAttach(&(PVOID&)OldCreateFileMappingA, NewCreateFileMappingA);
        DetourAttach(&(PVOID&)OldCreateFileMappingW, NewCreateFileMappingW);
        DetourAttach(&(PVOID&)OldMapViewOfFile, NewMapViewOfFile);
        DetourAttach(&(PVOID&)OldSetFilePointer, NewSetFilePointer);
        DetourAttach(&(PVOID&)OldSetEndOfFile, NewSetEndOfFile);
        DetourAttach(&(PVOID&)OldCopyFile, NewCopyFile);

        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourAttach(&(PVOID&)OldRegQueryValueEx, NewRegQueryValueEx);

        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

        DetourAttach(&(PVOID&)Oldsocket, Newsocket);
        //DetourAttach(&(PVOID&)Oldbind, Newbind);
        DetourAttach(&(PVOID&)Oldsend, Newsend);
        DetourAttach(&(PVOID&)Oldconnect, Newconnect);
        DetourAttach(&(PVOID&)Oldrecv, Newrecv);
        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

        DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourDetach(&(PVOID&)OldCreateFileA, NewCreateFileA);
        DetourDetach(&(PVOID&)OldCreateFileW, NewCreateFileW);
        DetourDetach(&(PVOID&)OldCloseHandle, NewCloseHandle);
        DetourDetach(&(PVOID&)OldDeleteFile, NewDeleteFile);
        DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
        DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourDetach(&(PVOID&)OldFindFirstFile, NewFindFirstFile);
        DetourDetach(&(PVOID&)OldFindNextFile, NewFindNextFile);
        DetourDetach(&(PVOID&)OldGetProcAddress, NewGetProcAddress);
        DetourDetach(&(PVOID&)OldLoadLibraryA, NewLoadLibraryA);
        DetourDetach(&(PVOID&)OldLoadLibraryW, NewLoadLibraryW);
        DetourDetach(&(PVOID&)OldCreateFileMappingA, NewCreateFileMappingA);
        DetourDetach(&(PVOID&)OldCreateFileMappingW, NewCreateFileMappingW);
        DetourDetach(&(PVOID&)OldMapViewOfFile, NewMapViewOfFile);
        DetourDetach(&(PVOID&)OldSetFilePointer, NewSetFilePointer);
        DetourDetach(&(PVOID&)OldSetEndOfFile, NewSetEndOfFile);
        DetourDetach(&(PVOID&)OldCopyFile, NewCopyFile);

        DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourDetach(&(PVOID&)OldRegQueryValueEx, NewRegQueryValueEx);

        DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

        DetourDetach(&(PVOID&)Oldsocket, Newsocket);
        //DetourDetach(&(PVOID&)Oldbind, Newbind);
        DetourDetach(&(PVOID&)Oldsend, Newsend);
        DetourDetach(&(PVOID&)Oldconnect, Newconnect);
        DetourDetach(&(PVOID&)Oldrecv, Newrecv);

        DetourTransactionCommit();
        sendlogs();
        if (flag) saveHttpText();
        SocketClose();
        break;
    }
    return TRUE;
}

