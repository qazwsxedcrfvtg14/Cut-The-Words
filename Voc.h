#pragma once
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <stack>
#include <functional> 
#include <cctype>
#include <locale>
#include <locale>
#include <codecvt>
#include <ppltasks.h>
#include <functional>
//#include <boost/regex.hpp>
#include "Helpers.h"
#include "AppShell.xaml.h"
#include <thread>
#include "opencc/SimpleConverter.hpp"
//#include <sqlite3.h>
#define f first
#define s second
using namespace concurrency;
using namespace Windows::Devices::Enumeration;
using namespace std;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Web;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;
using namespace Windows::Web::Http::Headers;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

struct map_comp {
	inline bool operator() (const std::wstring& lhs, const std::wstring& rhs) const;
};
typedef map<wstring, wstring,map_comp> DataMap;
extern DataMap ok_words;
extern DataMap words, prefix, suffix, root, favorite, setting, note;
//extern set<string> vocs;

template <class TResult>
inline TResult AWait(Windows::Foundation::IAsyncOperation<TResult>^ asyncOp)
{
	HANDLE handle = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
	TResult result;
	Exception^ exceptionObj = nullptr;
	asyncOp->Completed = ref new Windows::Foundation::AsyncOperationCompletedHandler<TResult>([&]
		(Windows::Foundation::IAsyncOperation<TResult>^ asyncInfo, Windows::Foundation::AsyncStatus asyncStatus)
	{
		try
		{
			result = asyncInfo->GetResults();
		}
		catch (Exception^ e)
		{
			exceptionObj = e;
		}
		SetEvent(handle);
	});
	WaitForSingleObjectEx(handle, INFINITE, FALSE);
	if (exceptionObj != nullptr) throw exceptionObj;
	return result;
}
inline void AWait(Windows::Foundation::IAsyncAction^ asyncAc);
void DumpAppFile(wstring fil);
wstring FileToStr(wstring fil);
void StrToFile(wstring s, wstring fil);
void AppendStrToFile(wstring ws, wstring fil);
inline std::wstring &ltrim(std::wstring &s);
inline std::wstring &rtrim(std::wstring &s);
inline std::wstring trim(std::wstring s);
void get_doc(wstring inp, DataMap &words, DataMap &ok, bool user = 1);
void get_doc(wstring inp, DataMap &words, bool user = 1);
wstring dump_doc(DataMap &words, DataMap &ok);
wstring dump_doc(DataMap &words);
void match(wstring match, vector<wstring>& ve, wstring beg = L"");
void  match_rot(wstring match, vector<wstring>&ve, wstring beg = L"");
vector<pair<wstring, wstring>> Show(wstring s);
vector<wstring> Show2(wstring s);
wstring IntToStr(int x);
int StrToInt(wstring x);
wstring FromPlatStr(wstring ws);
Platform::String^ ToPlatStr(wstring s);
string w2s(wstring ws);
wstring s2w(string s);
void ShowMsg(wstring msg);
//template<typename Fnc>
void post(wstring url, wstring data, function<void(wstring)> fnc, function<void()> fnc2);
void get(wstring url, function<void(wstring)> fnc, function<void()> fnc2);
void post(wstring url, HttpMultipartFormDataContent^ data, function<void(wstring)> fnc, function<void()> fnc2);
//void post2(wstring url, wstring data, function<void(wstring)> fnc, function<void(wstring)> fnc2);
void ShowLoading();
void HideLoading();

pair<wstring, vector<int>> GetExp(wstring s);
wstring GetRootExp(wstring s, int cnt=0);
wstring GetRootExpOrg(wstring s);
wstring GetExpSimple(wstring s,int cnt=0);
wstring GetExpSimpleOrg(wstring s);
vector<wstring> CutExp(wstring s);
wstring MakeExp(pair<wstring, vector<int>> p);
wstring MergeExp(vector<wstring> v);
wstring LowwerCase(wstring s);
//extern unique_ptr<map<wstring, set<wstring>>> rtp;
extern map<wstring, set<wstring>>rt;
extern bool inited;
extern int loading_cnt;
std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
wstring s2t(wstring s);

extern unordered_map<long long int,bool> scroll_load_not_finish;
struct Json_Node {
	int type;
	wstring data;
	map<wstring, Json_Node>child;
	vector<Json_Node>ary;
};
extern Json_Node user_data;
Json_Node Read_Json(wstring s);
void Read_Json(wstring s, Json_Node & now);


extern wstring key_main;
void kalisin(wstring path, wstring snd = L"", bool show_error = 1, function<void(wstring)> fnc = [](wstring s) {});
void SavingSetting();
int LevenshteinDistance(wstring s, wstring t);
long long int GetCurrentID();
int RandomInt(int n=0x7fffffff);
extern bool IsTrial;
void InitializeLicense();
pair<wstring, wstring> WordRotToExp(wstring now);
StackPanel^ ExpStack(wstring s,int fontsize=16);
