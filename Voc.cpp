#include "pch.h"
#include "Voc.h"
#include <assert.h>
#include <stack>
#include <ppltasks.h>
#include "AppShell.xaml.h"
#include "LoadingPage.xaml.h"
#include "NullPage.xaml.h"

inline bool map_comp::operator() (const std::wstring& lhs, const std::wstring& rhs) const {
	return _wcsicmp(lhs.c_str(), rhs.c_str()) < 0;
}

void OnMessageReceived(Windows::Networking::Sockets::DatagramSocket ^sender, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs ^args);
bool inited;
using namespace CutTheWords;
/*
*   AsyncHelper
*
*   Date: 1st July, 2014   Author: David Huang
*   (C) 2014 Light Studio. All Rights Reserved.
*/
inline void AWait(Windows::Foundation::IAsyncAction^ asyncAc)
{
	HANDLE handle = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
	Exception^ exceptionObj = nullptr;
	asyncAc->Completed = ref new Windows::Foundation::AsyncActionCompletedHandler([&]
		(Windows::Foundation::IAsyncAction^ asyncInfo, Windows::Foundation::AsyncStatus asyncStatus)
	{
		try
		{
			asyncInfo->GetResults();
		}
		catch (Exception^ e)
		{
			exceptionObj = e;
		}
		SetEvent(handle);
	});
	WaitForSingleObjectEx(handle, INFINITE, FALSE);
	if (exceptionObj != nullptr) throw exceptionObj;
}
/*
*   End of AsyncHelper
*/
void DumpAppFile(wstring fil) {
	stringstream ss;
	ifstream fin(fil, ios::binary);
	ss << fin.rdbuf(); // dump file contents into a stringstream
	string const &s = ss.str();
	wstring ws;
	ws.resize(s.size() / sizeof(wchar_t));
	std::memcpy(&ws[0], s.c_str(), s.size()* sizeof(char)); // copy data into wstring
	if (ws[0] == 0xfffe || ws[0] == 0xfeff)ws = ws.substr(1);
	auto folder = ApplicationData::Current->LocalFolder;
	AWait(FileIO::WriteTextAsync(AWait(folder->CreateFileAsync(ref new String(fil.c_str()), CreationCollisionOption::ReplaceExisting)), ref new String(ws.c_str())));
}
wstring FileToStr(wstring fil) {
	auto folder = ApplicationData::Current->LocalFolder;
	if (AWait(folder->TryGetItemAsync(ref new String(fil.c_str()))) != nullptr)
		return AWait(FileIO::ReadTextAsync(AWait(folder->GetFileAsync(ref new String(fil.c_str())))))->Data();
	else
		return L"";
}
void StrToFile(wstring ws, wstring fil) {
	auto folder = ApplicationData::Current->LocalFolder;
	AWait(FileIO::WriteTextAsync(AWait(folder->CreateFileAsync(ref new String(fil.c_str()), CreationCollisionOption::ReplaceExisting)), ref new String(ws.c_str())));
}
void AppendStrToFile(wstring ws, wstring fil) {
	auto folder = ApplicationData::Current->LocalFolder;
	AWait(FileIO::AppendTextAsync(AWait(folder->GetFileAsync(ref new String(fil.c_str()))), ref new String(ws.c_str())));
}
inline std::wstring &ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
inline std::wstring &rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
inline std::wstring trim(std::wstring s) {
	return (Helpers::Trim(ref new String(s.c_str())))->Data();
}
void get_doc(wstring inp, DataMap &words, DataMap &ok, bool user) {
	words.clear();
	ok.clear();
	wstringstream str;
	auto ps = inp.find(L".txt");
	wstring tmp;
	if (ps != wstring::npos&&user)
		tmp += FileToStr(inp) +L"\n"+ FileToStr(inp.substr(0, ps) + L"_user.txt");
	else
		tmp += FileToStr(inp);
	/*int pos;
	while ((pos = tmp.find(L"\n,")) != wstring::npos)
		tmp.erase(pos,1);
	while ((pos = tmp.find(L"\r,")) != wstring::npos)
		tmp.erase(pos,1);
	StrToFile(tmp, L"good_" + inp);*/
	str << tmp;
	wstring s;
	while (getline(str, s)) {
		vector<wstring>a;
		a.push_back(L"");
		wstring b, d;
		int tag = 0, l = (int)s.length(), star = 0, dis = 0;
		for (int i = 0;i < l;i++)
			//if (s[i] == L'=')break;//else 
			if (tag)b += s[i];
			else if (s[i] == L'$')dis = 1;
			else if (s[i] == L'*')star = 1;
			else if (star&&s[i] == L' ')d += L' ';
			else if (s[i] == L',')tag = 1;
			else if (s[i] == L'/')a.push_back(L"");
			//else if (s[i] >= L'A'&&s[i] <= L'Z') a.back() += s[i] - L'A' + L'a', d += s[i] - L'A' + L'a';
			else a.back() += s[i], d += s[i];
			for (auto &c : a) {
				if (trim(c) == L"")continue;
				if (dis) { 
					if (ok.find(c) != ok.end())
						ok.erase(c);
					words.erase(c); 
					continue; 
				}
				words[c] = b;
				if (star)ok[c] = d;
			}
	}
	return;
}
void get_doc(wstring inp, DataMap &words, bool user) {
	words.clear();
	wstringstream str;
	auto ps = inp.find(L".txt");
	if (ps != wstring::npos&&user)
		str << FileToStr(inp) + FileToStr(inp.substr(0, ps) + L"_user.txt");
	else
		str << FileToStr(inp);
	wstring s;
	while (getline(str, s)) {
		vector<wstring>a;
		a.push_back(L"");
		wstring b, d;
		int tag = 0, l = (int)s.length(), star = 0, dis = 0;
		for (int i = 0;i < l;i++)
			//if (s[i] == L'=')break;
			//else 
			if (tag)b += s[i];
			else if (s[i] == L'$')dis = 1;
			else if (s[i] == L'*')star = 1;
			else if (star&&s[i] == L' ')d += L' ';
			else if (s[i] == L',')tag = 1;
			else if (s[i] == L'/')a.push_back(L"");
			//else if (s[i] >= L'A'&&s[i] <= L'Z') a.back() += s[i] - L'A' + L'a', d += s[i] - L'A' + L'a';
			else a.back() += s[i], d += s[i];
			for (auto &c : a) {
				if (trim(c) == L"")continue;
				if (dis) { words.erase(c); continue; }
				words[c] = b;
			}
	}
	return;
}
wstring dump_doc(DataMap &words, DataMap &ok) {
	wstring out;
	for (auto x : words) {
		if (ok_words.find(x.f) != ok_words.end())
			out += L"*" + ok[x.f] + L"," + trim(x.s) + L"\n";
		else
			out += x.f + L"," + trim(x.s) + L"\n";
	}
	return out;
}
wstring dump_doc(DataMap &words) {
	wstring out;
	for (auto x : words)
		out += x.f + L"," + trim(x.s) + L"\n";
	return out;
}
DataMap ok_words;
DataMap words, prefix, suffix, root, favorite, setting, note;
//set<string> vocs;

std::wstring& trimx(std::wstring &s)
{
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(L" "));
	s.erase(s.find_last_not_of(L" ") + 1);
	return s;
}

void  match(wstring match, vector<wstring>&ve, wstring beg) {
	ve.clear();
	try {
		wstring reg_wstring;
		wstring mi, ma;
		bool tg = 1;
		bool noteng = false;
		bool star = false;
		if (match.length() && match[0] == '^')
			noteng = true,match=match.substr(1);
		for (int i = 0;match[i];i++) {
			if (match[i] == L'*'/* || match[i] == L'-'*/) {
				if (i&&match[i-1] == '*')continue;
				if (i&&star&&match[i-1] == '?')continue;
				ma += (wchar_t)0xeffff;
				reg_wstring += L".*";
				tg = 0;
				star = true;
			}
			else if (match[i] == L'?') {
				if (tg)mi += (wchar_t)0;
				ma += (wchar_t)0xeffff;
				reg_wstring += L".";
			}
			else if (match[i] >= 'a'&&match[i] <= L'z') {
				reg_wstring += wstring(L"[") + match[i] + wchar_t(match[i] - 'a' + 'A') + wstring(L"]");
				ma += match[i];
				if (tg)mi += match[i];
				star = false;
			}
			else if (match[i] >= 'A'&&match[i] <= L'Z') {
				reg_wstring += wstring(L"[") + match[i] + wchar_t(match[i] - 'A' + 'a') + wstring(L"]");
				ma += match[i];
				if (tg)mi += match[i];
				star = false;
			}
			else if ((match[i]=='['||match[i]==']'||match[i]=='.')) {
				reg_wstring += wstring(L"\\")+match[i];
				ma += match[i];
				if (tg)mi += match[i];
				star = false;
			}
			else if(match[i]>=0&&match[i]<=128){
				reg_wstring += match[i];
				ma += match[i];
				if (tg)mi += match[i];
				star = false;
			}
			else {
				noteng = true;
				reg_wstring += match[i];
				ma += match[i];
				if (tg)mi += match[i];
				star = false;
			}
		}
		wregex reg(reg_wstring);
		wregex reg2(L".*"+reg_wstring+ L".*");
		int cnt = 0;
		auto be = words.lower_bound(mi), ed = words.upper_bound(ma);
		if (noteng)
			be = words.begin(), ed = words.end();
		if (beg != L"")
			be = words.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->first, reg)) {
				ve.push_back(it->first);
				if (++cnt == 30)break;
			}
			else if (noteng&&regex_match(it->second, reg2)) {
				ve.push_back(it->first);
				if (++cnt == 30)break;
			}
	}
	catch (regex_error& error) {
		error;

	}
	return;
}

void  match_rot(wstring match, vector<wstring>&ve, wstring beg) {
	ve.clear();
	try {
		wstring reg_wstring;
		wstring mi, ma;
		bool tg = 1;
		for (int i = 0;match[i];i++) {
			if (match[i] == L'*'/* || match[i] == L'-'*/) {
				ma += 0xEFFF;
				reg_wstring += L".*";
				tg = 0;
			}
			else if (match[i] == L'?') {
				if (tg)mi += (wchar_t)0;
				ma += (wchar_t)0xeffff;
				reg_wstring += L"[.";
			}
			else if (match[i] >= 'A'&&match[i] <= L'Z') {
				reg_wstring += (char)match[i] - 'A' + 'a';
				ma += (char)match[i] - 'A' + 'a';
				if (tg)mi += (char)match[i] - 'A' + 'a';
			}
			else {
				reg_wstring += (char)match[i];
				ma += (char)match[i];
				if (tg)mi += (char)match[i];
			}
		}
		wregex reg(reg_wstring);
		int cnt = 0;
		auto be = prefix.lower_bound(mi), ed = prefix.upper_bound(ma);
		if (beg != L"")be = prefix.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->f, reg)) {
				ve.push_back(it->f + L"-");
				if (++cnt == 30)break;
			}
		be = root.lower_bound(mi), ed = root.upper_bound(ma);
		if (beg != L"")be = root.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->f, reg)) {
				ve.push_back(L"-" + it->f + L"-");
				if (++cnt == 60)break;
			}
		be = suffix.lower_bound(mi), ed = suffix.upper_bound(ma);
		if (beg != L"")be = suffix.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->f, reg)) {
				ve.push_back(L"-" + it->f);
				if (++cnt == 90)break;
			}
		sort(ve.begin(), ve.end(), [](wstring a, wstring b) {
			if (a[0] == '-'&&b[0] == '-')return a < b;
			else if (a[0] == '-')return a.substr(1) < b;
			else if (b[0] == '-')return a < b.substr(1);
			else return a < b;
		});
	}
	catch (regex_error& error) {
		error;
	}
	return;
}

vector<wstring> Show2(wstring s) {
	s = trim(s);
	int len = (int)s.length();
	bool sp = false;
	for (int i = 0; i < len; i++)
		if (s[i] >= 'A'&&s[i] <= 'Z')
			s[i] = s[i];
		else if (s[i] >= 'a'&&s[i] <= 'z')
			s[i] = s[i];
		else
			sp = true, s[i] = ' ';
	if(sp){
		wstringstream str(s);
		vector<wstring> ret;
		while (str >> s)
			ret.push_back(s);
		return ret;
	}
	else if (ok_words.find(s)!=ok_words.end()) {
		wstringstream str(ok_words[s]);
		deque<wstring>dq;
		int cnt = 0;
		while (str >> s) {
			cnt++;
			dq.push_back(s);
		}
		if (cnt == 1) {
			vector<wstring> ret;
			ret.push_back(s);
			return ret;
		}
		else if (cnt >= 2) {
			wstring bes, eds;
			vector<wstring> mds;
			if (prefix.find(dq.front()) != prefix.end())
				bes += dq.front() + L"-", dq.pop_front();
			if (suffix.find(dq.back()) != suffix.end())
				eds += L"-" + dq.back(), dq.pop_back();
			bool nbe = 0;
			for (auto &x : dq) {
				nbe = 1;
				if (root.find(x) != root.end())
					mds.push_back(L"-" + x + L"-");
				else if (words.find(x) != words.end())
					mds.push_back(x);
				else if (words.find(x+L'e') != words.end())
					mds.push_back(x);
				else if (prefix.find(x) != prefix.end())
					mds.push_back(x + L"-"); 
				else if (suffix.find(x) != suffix.end())
					mds.push_back(L"-" + x);
				else
					mds.push_back(x);
			}
			vector<wstring> ret;
			if (bes != L"")
				ret.push_back(bes);
			int sz = (int)mds.size();
			for (int i = 0; i < sz; i++)
				ret.push_back(mds[i]);
			if (eds != L"")
				ret.push_back(eds);
			return ret;
		}
		else {
			return vector<wstring>();
		}
	}
	else {
		vector<wstring> ret;
		if (s.length() >=5 && s.substr(s.length() - 3, 3) == L"ing") {
			if (words.find(s.substr(0, s.length() - 3) + L"e") != words.end()) {
				ret.push_back(s.substr(0, s.length() - 3));
				ret.push_back(L"-ing");
				return ret;
			}
			if (words.find(s.substr(0, s.length() - 3)) != words.end()) {
				ret.push_back(s.substr(0, s.length() - 3));
				ret.push_back(L"-ing");
				return ret;
			}
			if (s.length() >= 6 && words.find(s.substr(0, s.length() - 4)) != words.end()) {
				ret.push_back(s.substr(0, s.length() - 4));
				ret.push_back(s.substr(s.length() - 4, 1));
				ret.push_back(L"-ing");
				return ret;
			}
		}
		vector<vector<int>>dp(len);
		vector<vector<wstring>>dps(len);
		for (auto &x : dp)
			x.resize(len);
		for (auto &x : dps)
			x.resize(len);
		for (int i = 0; i < len; i++)
			for (int j = i; j < len; j++) {
				wstring now = s.substr(i, j - i + 1);
				dp[i][j] = j - i - 1;
				dps[i][j] = now;
				if (i == 0 && j == len - 1) {
					dp[i][j] = j-i + max(17 / (j - i + 1), 1);
					dps[i][j] = now;
					continue;
				}
				if ((i != 0 || j != len - 1) && prefix.find(now) != prefix.end()) {
					int score = i == 0 ? (j - i)*2+1 : (j - i - 1)*2;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = now + L"-";
				}
				if ((i != 0 || j != len - 1) && suffix.find(now) != suffix.end()) {
					int score = j == len - 1 ? (j - i)*2+1 : (j - i - 1)*2;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = L"-" + now;
				}
				if ((i != 0 || j != len - 1) && root.find(now) != root.end()) {
					int score = (j - i)*2;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = L"-" + now + L"-";
				}
				if ((i != 0 || j != len - 1) && (j - i >= 3 || (j - i == 2 && (j == len - 1 || i == 0))) && words.find(now) != words.end()) {
					int score = (j - i) * 2;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = now;
				}
				if ((i != 0 || j != len - 2) && (j - i >= 3 || (j - i == 2 && (j == len - 1 || i == 0))) && words.find(now + L"e") != words.end()) {
					int score = (j - i) * 2 - 1;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = now;
				}
				if ((i != 0 || j != len - 1) && (j - i >= 3 || (j - i == 2 && (j == len - 1 || i == 0))) && now.back()=='i' && words.find(now.substr(0,now.length()-1) + L"y") != words.end()) {
					int score = (j - i) * 2 - 1;
					if (dp[i][j] < score)
						dp[i][j] = score, dps[i][j] = now;
				}
			}
		vector<int>d(len);
		vector<int>f(len);
		/*wstring out;
		for (int i = 0; i < len; i++,out+=L'\n')
			for (int j = 0; j < len; j++)
				out += IntToStr(dp[i][j]) +L' ';
		if(s==L"simplicity")
			ShowMsg(out);*/
		for (int i = 0; i < len; i++) {
			d[i] = dp[0][i];
			f[i] = -1;
			for (int j = 0; j < i; j++)
				if (d[i] <= d[j] + dp[j + 1][i])
					d[i] = d[j] + dp[j + 1][i],
					f[i] = j;
		}
		deque<wstring>dq;
		int now = len - 1;
		while (~now)
			dq.push_front(dps[f[now] + 1][now]), now = f[now];
		for (auto &x : dq)
			ret.push_back(x);
		return ret;
	}
}
vector<pair<wstring, wstring>> Show(wstring s) {
	auto ve = Show2(s);
	vector<pair<wstring, wstring>>ret;//chinese eng
	for (auto &x : ve) {
		int len = (int)x.length();
		if (x[0] == '-'||x[len - 1] == '-') {
			wstring now = x;
			if (now[0] == '-')
				now = now.substr(1);
			if (now[now.length() - 1] == '-')
				now = now.substr(0, now.length() - 1);
			ret.push_back(make_pair(GetRootExp(x), now));
		}
		else {
			auto now = x;
			ret.push_back(make_pair(WordRotToExp(now).f,now));
		}
	}
	return ret;
}
pair<wstring, wstring> WordRotToExp(wstring now) {
	wstring nw;
	if (words.find(now) != words.end())
		return (make_pair(words[now], now));
	else if (now.length()>2 && words.find(now + L"e") != words.end())
		return (make_pair(words[now + L"e"], now + L"e"));
	else if (now.length()>2 && now.back() == 'i' && (nw = now.substr(0, now.length() - 1) + L"y", words.find(nw) != words.end()))
		return (make_pair(words[nw], nw));
	else
		return (make_pair(now, now));
}
wstring IntToStr(int x) {
	wstringstream str;
	wstring s;
	str << x;
	str >> s;
	return s;
}
int StrToInt(wstring x) {
	wstringstream str;
	int s=0;
	str << x;
	str >> s;
	return s;
}
wstring FromPlatStr(wstring ws) {
	return wstring(ws.begin(), ws.end());
}
Platform::String^ ToPlatStr(wstring s) {
	std::wstring s_str = std::wstring(s.c_str());
	std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
	const wchar_t* w_char = wid_str.c_str();
	return ref new Platform::String(w_char);
}

string w2s(wstring ws) {
	string s;
	for (auto &x : ws)s += (char)x;
	return s;
}
wstring s2w(string s) {
	wstring ws;
	for (auto x : s)ws += (wchar_t)x;
	return ws;
}
void ShowMsg(wstring msg) {
	(ref new MessageDialog(ref new String(msg.c_str())))->ShowAsync();
	//AWait(tmp);
}
void ShowMsgMenu() {
	//(ref new MessageDialog(ref new String(msg.c_str())))->ShowAsync();
	auto menu = (ref new PopupMenu());
	menu->Commands->Append(ref new UICommand("Open with"));
	menu->Commands->Append(ref new UICommandSeparator);
	menu->Commands->Append(ref new UICommand("Open with2"));
	auto tmp = menu->ShowAsync(Point(0, 0));
	create_task(tmp).then([](IUICommand ^ rt) {
		(ref new MessageDialog(rt->Label))->ShowAsync();
	});
	//AWait(tmp);
}
//template<typename Fnc>

void post(wstring url, wstring data, function<void(wstring)> fnc, function<void()> fnc2) {
	Uri^ resourceAddress = ref new Uri(ref new String(url.c_str()));
	auto httpClient = Helpers::CreateHttpClient();
	//HttpMultipartFormDataContent^ post_data = ref new HttpMultipartFormDataContent();
	//post_data->Add(ref new HttpStringContent(ref new String(data.c_str())));
	auto post_data = ref new HttpStringContent(ref new String(data.c_str()));
	create_task(httpClient->PostAsync(resourceAddress, post_data)).
		then([=](HttpResponseMessage^ response) {
		task<String^> readAsStringTask(response->Content->ReadAsStringAsync());
		return readAsStringTask.then([=](String^ responseBodyAsText) {
			wstring ws = responseBodyAsText->Data();
			fnc(ws);
			return response;
		});
	}).
		then([=](task<HttpResponseMessage^> previousTask) {
		try {
			previousTask.get();
		}
		catch (const task_canceled&) {
		}
		catch (Exception^ ex) {
			fnc2();
		}
	});
}
void get(wstring url, function<void(wstring)> fnc, function<void()> fnc2) {
	Uri^ resourceAddress = ref new Uri(ref new String(url.c_str()));
	auto httpClient = Helpers::CreateHttpClient();
	create_task(httpClient->GetAsync(resourceAddress)).
		then([=](HttpResponseMessage^ response) {
		task<String^> readAsStringTask(response->Content->ReadAsStringAsync());
		return readAsStringTask.then([=](String^ responseBodyAsText) {
			wstring ws = responseBodyAsText->Data();
			fnc(ws);
			return response;
		});
	}).
		then([=](task<HttpResponseMessage^> previousTask) {
		try {
			previousTask.get();
		}
		catch (const task_canceled&) {
		}
		catch (Exception^ ex) {
			fnc2();
		}
	});
}

void post(wstring url, HttpMultipartFormDataContent^ data, function<void(wstring)> fnc, function<void()> fnc2) {
	Uri^ resourceAddress = ref new Uri(ref new String(url.c_str()));
	auto httpClient = Helpers::CreateHttpClient();
	create_task(httpClient->PostAsync(resourceAddress, data)).
		then([=](HttpResponseMessage^ response) {
		task<String^> readAsStringTask(response->Content->ReadAsStringAsync());
		return readAsStringTask.then([=](String^ responseBodyAsText) {
			wstring ws = responseBodyAsText->Data();
			fnc(ws);
			return response;
		});
	}).
		then([=](task<HttpResponseMessage^> previousTask) {
		try {
			previousTask.get();
		}
		catch (const task_canceled&) {
		}
		catch (Exception^ ex) {
			fnc2();
		}
	});
}
/*
void post2(wstring url, wstring data, function<void(wstring)> fnc, function<void(wstring)> fnc2) {
Uri^ resourceAddress = ref new Uri(ref new String(url.c_str()));
auto httpClient = Helpers::CreateHttpClient();
HttpMultipartFormDataContent^ post_data = ref new HttpMultipartFormDataContent();
post_data->Add(ref new HttpStringContent(ref new String(data.c_str())), "post");
create_task(httpClient->PostAsync(resourceAddress, post_data)).
then([=](HttpResponseMessage^ response) {
task<String^> readAsStringTask(response->Content->ReadAsStringAsync());
return readAsStringTask.then([=](String^ responseBodyAsText) {
wstring ws = responseBodyAsText->Data();
fnc(ws);
return response;
});
}).
then([=](task<HttpResponseMessage^> previousTask) {
try {
previousTask.get();
}
catch (const task_canceled&) {
}
catch (Exception^ ex) {
fnc2(ex->Message->Data());
}
});
}*/
void LogMessage(Object^ parameter)
{
	auto paraString = parameter->ToString();
	auto formattedText = std::wstring(paraString->Data()).append(L"\r\n");
	OutputDebugString(formattedText.c_str());
}
void ShowLoading() {
	auto shell = dynamic_cast<AppShell^>(Window::Current->Content);
	shell->AppTopFrame->Navigate(TypeName(Views::LoadingPage::typeid));
}
void HideLoading() {
	auto shell = dynamic_cast<AppShell^>(Window::Current->Content);
	shell->AppTopFrame->Navigate(TypeName(Views::NullPage::typeid));
}
pair<wstring, vector<int>> GetExp(wstring s) {
	auto pos = s.find(L",");
	if (pos == wstring::npos)return make_pair(s, vector<int>());
	wstringstream str(s.substr(pos + 1));
	int i;
	vector<int>ve;
	while (str >> i)ve.push_back(i);
	s = s.substr(0, pos);
	if (s.length()>1 && s[0] == '='&&words.find(trim(s.substr(1))) != words.end())
		return make_pair(GetExpSimple(words[trim(s.substr(1))]), ve);
	return make_pair(s, ve);
}
wstring GetExpSimple(wstring s,int cnt) {
	auto pos = s.find(L",");
	if (pos == wstring::npos)return s;
	s = s.substr(0, pos);
	if (cnt<20&&s.length()>1 && s[0] == '='&&words.find(trim(s.substr(1))) != words.end())
		return GetExpSimple(words[trim(s.substr(1))],cnt+1);
	return s;
}
wstring GetExpSimpleOrg(wstring s) {
	auto pos = s.find(L",");
	if (pos == wstring::npos)return s;
	return s.substr(0, pos);
}
vector<wstring> CutExp(wstring s) {
	s = GetExp(s).f;
	wstring s2 = s;
	vector<wstring>ve;
	wstring::size_type pos;
	while ((pos = s.find(L"/")) != wstring::npos) {
		if (pos)
			ve.push_back(trim(s.substr(0, pos)));
		while (pos + 1 < s.length() && s[pos + 1] == '/')pos++;
		if (pos + 1 != s.length())
			s = s.substr(pos + 1);
		else
			s = L"";
	}
	if (s != L"")
		ve.push_back(trim(s));
	if (ve.size() == 1 && s[0] == '[') {
		s = s2;
		ve.clear();
		while (s.length()>1 && (pos = s.find(L"[", 1)) != wstring::npos) {
			ve.push_back(trim(s.substr(0, pos)));
			s = s.substr(pos);
		}
		if (s != L"")
			ve.push_back(trim(s));
		return ve;
	}
	return ve;
}

wstring MakeExp(pair<wstring, vector<int>> p) {
	wstring ret(p.f + L",");
	bool nf = 0;
	for (auto &x : p.s) {
		if (nf)ret += L" ";nf = 1;
		ret += IntToStr(x);
	}
	return ret;
}
wstring MergeExp(vector<wstring> v) {
	wstring ret;
	bool nf = 0;
	for (auto &x : v) {
		if (nf)ret += L"/";nf = 1;
		ret += x;
	}
	return ret;
}
wstring GetRootExp(wstring s, int cnt) {
	if (s == L"")return L"";
	if (s.front() == '-'&&s.back() == '-') {
		s = s.substr(1, s.length() - 2);
		if (root.find(s) != root.end()) {
			wstring exp = root[s];
			if (cnt < 20 && exp.length()>1 && exp[0] == '='&&root.find(trim(exp.substr(1))) != root.end())
				return GetRootExp(L"-" + trim(exp.substr(1)) + L"-", cnt + 1);
			return exp;
		}
		else
			return L"";
	}
	else if (s.front() == '-') {
		s = s.substr(1, s.length() - 1);
		if (suffix.find(s) != suffix.end()) {
			wstring exp = suffix[s];
			if (cnt < 20 && exp.length()>1 && exp[0] == '='&&suffix.find(trim(exp.substr(1))) != suffix.end())
				return GetRootExp(L"-" + trim(exp.substr(1)), cnt + 1);
			return exp;
		}
		else
			return L"";
	}
	else if (s.back() == '-') {
		s = s.substr(0, s.length() - 1);
		if (prefix.find(s) != prefix.end()) {
			wstring exp = prefix[s];
			if (cnt < 20 && exp.length()>1 && exp[0] == '='&&prefix.find(trim(exp.substr(1))) != prefix.end())
				return GetRootExp(trim(exp.substr(1))+ L"-" , cnt + 1);
			return exp;
		}
		else
			return L"";
	}
	else {
		return GetExpSimple(s);
	}
}
wstring GetRootExpOrg(wstring s) {
	if (s == L"")return L"";
	if (s.front() == '-'&&s.back() == '-') {
		s = s.substr(1, s.length() - 2);
		if (root.find(s) != root.end()) {
			return root[s];
		}
		else
			return L"";
	}
	else if (s.front() == '-') {
		s = s.substr(1, s.length() - 1);
		if (suffix.find(s) != suffix.end()) {
			return suffix[s];
		}
		else
			return L"";
	}
	else if (s.back() == '-') {
		s = s.substr(0, s.length() - 1);
		if (prefix.find(s) != prefix.end()) {
			return prefix[s];
		}
		else
			return L"";
	}
	else {
		return GetExpSimpleOrg(s);
	}
}
//unique_ptr<map<wstring, set<wstring>>> rtp=nullptr;
map<wstring, set<wstring>>rt;
int loading_cnt;
wstring LowwerCase(wstring s) {
	for (auto &x : s)
		if (x >= 'A'&&x <= 'Z')
			x += 'a' - 'A';
	return s;
}
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
//opencc::SimpleConverter mOpenCC("s2twp.json");
wstring s2t(wstring s) {
	opencc::SimpleConverter mOpenCC("s2twp.json");
	return utf8_decode(mOpenCC.Convert(utf8_encode(s)));
}
/*
sqlite3 *db;
void open_db(string dbn) {
if (sqlite3_open_v2((dbn+".db3").c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL))ShowMsg(L"error");
}
void insert_db(string dbn, wstring data) {
"CREATE TABLE Contact("
"ID INTEGER PRIMARY KEY,"
"Name VARCHAR(10),"
"PhoneNumber VARCHAR(10));"
sqlite3_exec(db, createsql, 0, 0, &errMsg);
char* query = sqlite3_mprintf("insert into tbl5 values ('%q');", utf8_encode(data).c_str());
sqlite3_free(sql);
}

//char s[20] = "some string";
//char* query = sqlite3_mprintf("insert into tbl5 values ('%q');", s);
*/

unordered_map<long long int, bool> scroll_load_not_finish ;
wstring erase_quote(wstring s) {
	s = trim(s);
	if (s.front() == '"'&&s.back() == '"') {
		if (s.length() > 2)s = s.substr(1, s.length() - 2);
		else s = L"";
	}
	return s;
}
void Read_Json(wstring s, Json_Node& now) {
	s = trim(s);
	if (s.front() == '{'&&s.back() == '}') {
		if (s.length()>2)s = s.substr(1, s.length() - 2);
		else s = L"";
		now.type = 1;
		wstring nw;
		int ntp = 0;
		stack<wchar_t>st;
		int beg;
		int len = (int)(s.length());
		for (int i = 0;i <= len;i++) {
			if (ntp == 0) {
				if (s[i] == ':')ntp = 1, beg = i + 1;
				else nw += s[i];
			}
			else if (ntp == 1) {
				if (s[i] == '{')
					st.push('{');
				else if (s[i] == '[')
					st.push('[');
				else if (s[i] == '}') {
					if (st.top() == '{')
						st.pop();
				}
				else if (s[i] == ']') {
					if (st.top() == '[')
						st.pop();
				}
				else if (s[i] == '"') {
					if (!st.empty() && st.top() == '"')
						st.pop();
					else
						st.push('"');
				}
				else if (s[i] == '\\') {
					i++;
				}
				else if (s[i] == ',' || s[i] == 0) {
					if (st.empty())
						Read_Json(s.substr(beg, i - beg), now.child[erase_quote(nw)]), ntp = 0, nw = L"";
				}
			}
		}
	}
	else if (s.front() == '['&&s.back() == ']') {
		if (s.length()>2)s = s.substr(1, s.length() - 2);
		else s = L"";
		now.type = 2;
		wstring nw;
		stack<wchar_t>st;
		int beg = 0;
		int len = (int)(s.length());
		for (int i = 0;i <= len;i++) {
			if (s[i] == '{')
				st.push('{');
			else if (s[i] == '[')
				st.push('[');
			else if (s[i] == '}') {
				if (st.top() == '{')
					st.pop();
			}
			else if (s[i] == ']') {
				if (st.top() == '[')
					st.pop();
			}
			else if (s[i] == '"') {
				if (!st.empty() && st.top() == '"')
					st.pop();
				else
					st.push('"');
			}
			else if (s[i] == '\\') {
				i++;
			}
			else if (s[i] == ',' || s[i] == 0) {
				if (st.empty()) {
					now.ary.push_back(Json_Node()), Read_Json(s.substr(beg, i - beg), now.ary.back()), beg = i + 1;
				}
			}
		}
	}
	else {
		now.type = 0;
		now.data = erase_quote(s);
	}
}

Json_Node Read_Json(wstring s) {
	s = trim(s);
	Json_Node now;
	if (s.front() == '{'&&s.back() == '}') {
		now.type = 1;
		if (s.length()>2)s = s.substr(1, s.length() - 2);
		else s = L"";
		wstring nw;
		int ntp = 0;
		stack<wchar_t>st;
		int beg;
		int len = (int)(s.length());
		for (int i = 0;i <= len;i++) {
			if (ntp == 0) {
				if (s[i] == ':')ntp = 1, beg = i + 1;
				else nw += s[i];
			}
			else if (ntp == 1) {
				if (s[i] == '{')
					st.push('{');
				else if (s[i] == '[')
					st.push('[');
				else if (s[i] == '}') {
					if (st.top() == '{')
						st.pop();
				}
				else if (s[i] == ']') {
					if (st.top() == '[')
						st.pop();
				}
				else if (s[i] == '"') {
					if (!st.empty() && st.top() == '"')
						st.pop();
					else
						st.push('"');
				}
				else if (s[i] == '\\') {
					i++;
				}
				else if (s[i] == ',' || s[i] == 0) {
					if (st.empty())
						Read_Json(s.substr(beg, i - beg), now.child[erase_quote(nw)]), ntp = 0, nw = L"";
				}
			}
		}
	}
	else if (s.front() == '['&&s.back() == ']') {
		if (s.length()>2)s = s.substr(1, s.length() - 2);
		else s = L"";
		now.type = 2;
		wstring nw;
		stack<wchar_t>st;
		int beg = 0;
		int len = (int)(s.length());
		for (int i = 0;i <= len;i++) {
			if (s[i] == '{')
				st.push('{');
			else if (s[i] == '[')
				st.push('[');
			else if (s[i] == '}') {
				if (st.top() == '{')
					st.pop();
			}
			else if (s[i] == ']') {
				if (st.top() == '[')
					st.pop();
			}
			else if (s[i] == '"') {
				if (!st.empty() && st.top() == '"')
					st.pop();
				else
					st.push('"');
			}
			else if (s[i] == '\\') {
				i++;
			}
			else if (s[i] == ',' || s[i] == 0) {
				if (st.empty())
					now.ary.push_back(Json_Node()), Read_Json(s.substr(beg, i - beg), now.ary.back()), beg = i + 1;
			}
		}
	}
	else {
		now.type = 0;
		now.data = erase_quote(s);
	}
	return now;
}
Json_Node user_data;
wstring key_main;
void kalisin(wstring path, wstring snd, bool show_error, function<void(wstring)> fnc) {
	post(L"http://app.kairisei-ma.jp" + path, key_main + snd, [=](wstring s) {
		if (s.length() <= 1) { ShowMsg(L"KeyError"); return; }
		wstring nk = s.substr(0, 30);

		if (nk != key_main&&nk[0] != '{') {
			key_main = nk;
		}
		int len = int(s.length()), a = -1, b = len;
		int cnt = 0;
		if (nk[0] != '{') {
			for (int i = 30;i < len;i++) {
				if (s[i] == '}') {
					cnt--;
					if (!cnt)a = i;
				}
				else if (s[i] == '{') {
					if (!cnt&&~a) {
						b = i;
						break;
					}
					cnt++;
				}
			}
		}
		//puts(http.Type["HTML"].substr(30,b-30));
		if (show_error || nk[0] == '{') {
			wstring res_data;
			if (nk[0] != '{')
				res_data = s.substr(30, b - 30);
			else
				res_data = s;
			auto rd = Read_Json(res_data);
			if (rd.child[L"res_code"].data != L"0") {
				wstring out;
				function<void(Json_Node&, int)> f = [&](Json_Node& now, int l) {
					if (now.type == 0)out += now.data;
					else if (now.type == 1) {
						out += L"{\n";
						for (auto &x : now.child) {
							for (int i = 0;i <= l;i++)
								out += L"  ";
							out += x.f + L": ";
							f(x.s, l + 1);
							out += L"\n";
						}
						for (int i = 0;i < l;i++)
							out += L"  ";
						out += L"}\n";
					}
					else if (now.type == 2) {
						out += L"[\n";
						for (auto &x : now.ary) {
							for (int i = 0;i <= l;i++)
								out += L"  ";
							f(x, l + 1);
							out += L"\n";
						}
						for (int i = 0;i < l;i++)
							out += L"  ";
						out += L"]\n";
					}
				};
				f(rd, 0);
				ShowMsg(out);
				return;
			}
		}
		if (b != len)fnc(s.substr(b));
		else fnc(L"");
		//return s.substr(b);
	}, [=] {ShowMsg(L"Network Error!");});
	//printf("Key: %s\n",key_main.c_str());
}

DatagramSocket^ NetCreat(wstring host,wstring port,bool DontFragment=1){
	HostName^ hostName;
	try{
		hostName = ref new HostName(ref new String(host.c_str()));
		}
	catch (InvalidArgumentException^ e){
		ShowMsg(L"Error: Invalid host name.");
		return nullptr;
		}
	/*if (CoreApplication::Properties->HasKey("clientSocket")){
		rootPage->NotifyUser(
			"This step has already been executed. Please move to the next one.",
			NotifyType::ErrorMessage);
		return;
	}*/

	DatagramSocket^ socket = ref new DatagramSocket();
	// Set the IP DF (Don't Fragment) flag.
	// This won't have any effect when running both client and server on localhost.
	// Refer to the DatagramSocketControl class' MSDN documentation for the full list of control options.
	socket->Control->DontFragment = DontFragment;
	socket->MessageReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::DatagramSocket ^, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs ^>(&OnMessageReceived);
	
	//SocketContext^ socketContext = ref new SocketContext(rootPage, socket);
	//socket->MessageReceived += ref new TypedEventHandler<DatagramSocket^, DatagramSocketMessageReceivedEventArgs^>(
		//socketContext,
		//&SocketContext::OnMessage);

	// Events cannot be hooked up directly to the ScenarioInput2 object, as the object can fall out-of-scope and be
	// deleted. This would render any event hooked up to the object ineffective. The SocketContext guarantees that
	// both the socket and object that serves its events have the same lifetime.
	//CoreApplication::Properties->Insert("clientSocket", socketContext);

	//rootPage->NotifyUser("Connecting to: " + HostNameForConnect->Text, NotifyType::StatusMessage);

	// Connect to the server (by default, the listener we created in the previous step).
	create_task(socket->ConnectAsync(hostName, ref new String(port.c_str()))).then(
		[](task<void> previousTask){
		try{
			// Try getting an exception.
			previousTask.get();
			//rootPage->NotifyUser("Connected", NotifyType::StatusMessage);
			//socketContext->SetConnected();
			}
		catch (Exception^ exception){
			ShowMsg(("Connect failed with error: " + exception->Message)->Data());
			//rootPage->NotifyUser("Connect failed with error: " + exception->Message, NotifyType::ErrorMessage);
			}
	});
	return socket;
}

void OnMessageReceived(Windows::Networking::Sockets::DatagramSocket ^sender, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs ^eventArguments)
{
	try
	{
		// Interpret the incoming datagram's entire contents as a string.
		unsigned int stringLength = eventArguments->GetDataReader()->UnconsumedBufferLength;
		String^ receivedMessage = eventArguments->GetDataReader()->ReadString(stringLength);

		//ShowMsg(("Received data from remote peer: \"" + receivedMessage + "\"")->Data());
	}
	catch (Exception^ exception)
	{
		SocketErrorStatus socketError = SocketError::GetStatus(exception->HResult);
		if (socketError == SocketErrorStatus::ConnectionResetByPeer)
		{
			// This error would indicate that a previous send operation resulted in an ICMP "Port Unreachable" message.
			ShowMsg(L"Peer does not listen on the specific port. Please make sure that you run step 1 first or you have a server properly working on a remote server.");
		}
		else if (socketError != SocketErrorStatus::Unknown)
		{
			ShowMsg(("Error happened when receiving a datagram: " + socketError.ToString())->Data());
		}
		else
		{
			throw;
		}
	}
}
void SavingSetting() {
	wstring out;
	for (auto &x : setting)
		out += x.f + L"," + x.s + L"\n";
	StrToFile(out, L"setting.txt");
}

int LevenshteinDistance(wstring s, wstring t)
{
	// degenerate cases
	if (s == t) return 0;
	int n = (int)s.length(), m = (int)t.length();
	if (n == 0) return m;
	if (m == 0) return n;

	// create two work vectors of integer distances
	int *v0 = new int[m + 1];
	int *v1 = new int[m + 1];

	// initialize v0 (the previous row of distances)
	// this row is A[0][i]: edit distance for an empty s
	// the distance is just the number of characters to delete from t
	for (int i = 0; i <= m; i++)
		v0[i] = i;

	for (int i = 0; i < n; i++)
	{
		// calculate v1 (current row distances) from the previous row v0

		// first element of v1 is A[i+1][0]
		//   edit distance is delete (i+1) chars from s to match empty t
		v1[0] = i + 1;

		// use formula to fill in the rest of the row
		for (int j = 0; j < m; j++)
		{
			int cost = (s[i] == t[j]) ? 0 : 1;
			v1[j + 1] = min(min(v1[j] + 1, v0[j + 1] + 1), v0[j] + cost);
		}

		// copy v1 (current row) to v0 (previous row) for next iteration
		for (int j = 0; j <= m; j++)
			v0[j] = v1[j];
	}

	return v1[m];
}
long long int GetCurrentID() {
	//return 0;
	return Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->Id;
}

int RandomInt(int n){
		return ((rand() << 16) + rand()) % n; 
}
Windows::ApplicationModel::Store::LicenseInformation^ licenseInformation;
bool IsTrial=true;
void licenseChangedEventHandler()
{
	if (licenseInformation->IsActive)
	{
		if (licenseInformation->IsTrial)
		{
			// Show the features that are available during trial only.
			//auto longDateFormat = ref new Windows::Globalization::DateTimeFormatting::DateTimeFormatter("longdate");

			// Display the expiration date using the DateTimeFormatter. 
			// For example, longDateFormat.Format(licenseInformation.ExpirationDate)

			//auto daysRemaining = (licenseInformation->ExpirationDate - DateTime->Now)->Days;
			IsTrial = true;
		}
		else
		{
			// Show the features that are available only with a full license.
			IsTrial = false;
		}
	}
	else
	{
		// A license is inactive only when there' s an error.
	}

}
void InitializeLicense()
{
	// Initialize the license info for use in the app that is uploaded to the Store.
	// uncomment for release
	//   licenseInformation = CurrentApp.LicenseInformation;

	// Initialize the license info for testing.
	// comment the next line for release
	//licenseInformation = Windows::ApplicationModel::Store::CurrentAppSimulator::LicenseInformation;
	licenseInformation = Windows::ApplicationModel::Store::CurrentApp::LicenseInformation;
	if (licenseInformation->IsActive&&licenseInformation->IsTrial)IsTrial = true;
	else IsTrial = false;
	licenseInformation->LicenseChanged += ref new Windows::ApplicationModel::Store::LicenseChangedEventHandler(&licenseChangedEventHandler);
}

StackPanel^ ExpStack(wstring s,int fontsize) {
	StackPanel^ expst=ref new StackPanel();
	//expst->Children->Clear();
	s = trim(s);
	wstring vol, pre;
	bool iv = 0;
	auto stp = ref new StackPanel();
	stp->Orientation = Orientation::Horizontal;
	for (auto x : s)
		if (!iv&&x == '[') {
			pre = trim(pre);
			if (pre != L"") {
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(trim(pre).c_str());
				tmp->FontSize = fontsize;
				tmp->Margin = Thickness(10, 0, 0, 0);
				tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
				tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
				stp->Children->Append(tmp);
				expst->Children->Append(stp);
				stp = ref new StackPanel();
				stp->Orientation = Orientation::Horizontal;
				pre.clear();
			}
			iv = 1;
		}
		else if (iv&&x == ']') {
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(trim(pre).c_str());
			tmp->FontSize = fontsize;
			tmp->FontWeight = Text::FontWeights::Bold;
			tmp->Margin = Thickness(2, 1, 2, 1);
			tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
			tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
			auto bor = ref new Border();
			bor->MinWidth = 50;
			bor->Margin = Thickness(10, 0, 0, 4);
			bor->BorderBrush = ref new SolidColorBrush(Windows::UI::Colors::Gray);
			bor->BorderThickness = 2;
			bor->Child = tmp;

			stp->Children->Append(bor);
			pre.clear();
			iv = 0;
		}
		else pre += x;
		if (pre != L"") {
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(trim(pre).c_str());
			tmp->FontSize = fontsize;
			tmp->Margin = Thickness(10, 0, 0, 0);
			tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
			tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
			stp->Children->Append(tmp);
			expst->Children->Append(stp);
		}
		return expst;
}
