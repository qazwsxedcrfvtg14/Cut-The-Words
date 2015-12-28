#include "pch.h"
#include "Voc.h"
#include <assert.h>
#include <stack>
#include <ppltasks.h>
#include "AppShell.xaml.h"
#include "LoadingPage.xaml.h"
#include "NullPage.xaml.h"
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
void get_doc(wstring inp, map<wstring, wstring> &words, map<wstring, wstring> &ok, bool user) {
	words.clear();
	ok.clear();
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
			if (s[i] == L'=')break;
			else if (tag)b += s[i];
			else if (s[i] == L'$')dis = 1;
			else if (s[i] == L'*')star = 1;
			else if (star&&s[i] == L' ')d += L' ';
			else if (s[i] == L',')tag = 1;
			else if (s[i] == L'/')a.push_back(L"");
			else if (s[i] >= L'A'&&s[i] <= L'Z') a.back() += s[i] - L'A' + L'a', d += s[i] - L'A' + L'a';
			else a.back() += s[i], d += s[i];
			for (auto &c : a) {
				if (trim(c) == L"")continue;
				if (dis) { words.erase(c); continue; }
				words[c] = b;
				if (star)ok[c] = d;
			}
	}
	return;
}
void get_doc(wstring inp, map<wstring, wstring> &words, bool user) {
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
			if (s[i] == L'=')break;
			else if (tag)b += s[i];
			else if (s[i] == L'$')dis = 1;
			else if (s[i] == L'*')star = 1;
			else if (star&&s[i] == L' ')d += L' ';
			else if (s[i] == L',')tag = 1;
			else if (s[i] == L'/')a.push_back(L"");
			else if (s[i] >= L'A'&&s[i] <= L'Z') a.back() += s[i] - L'A' + L'a', d += s[i] - L'A' + L'a';
			else a.back() += s[i], d += s[i];
			for (auto &c : a) {
				if (trim(c) == L"")continue;
				if (dis) { words.erase(c); continue; }
				words[c] = b;
			}
	}
	return;
}
map<wstring, wstring> ok_words;
map<wstring, wstring> words, prefix, suffix, root, favorite, setting, note;
set<string> vocs;

std::wstring& trimx(std::wstring &s)
{
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(L" "));
	s.erase(s.find_last_not_of(L" ") + 1);
	return s;
}

wstring make_tail2(wstring org, wstring tail) {
	wstring all = org;
	auto st = all.find(L'['), ed = all.find(L']');
	if (st == wstring::npos || ed == wstring::npos)return L"";
	wstring x = all.substr(st, ed - st + 1);
	auto pos = tail.find(x);
	if (pos == wstring::npos) {
		if (ed + 1 < all.length())
			return make_tail2(all.substr(ed + 1), tail);
		else
			return L"";
	}
	wstring ret;
	for (int i = (int)pos - 1;i >= 0 && tail[i] != L'/';i--)
		ret = tail[i] + ret;
	ret += x;
	return L"/" + trimx(ret);
}
wstring make_tail(wstring org, wstring tail) {
	if (words.find(org) == words.end())return tail;
	wstring all = words[org];
	auto st = all.find(L'['), ed = all.find(L']');
	if (st == wstring::npos || ed == wstring::npos)return tail;
	wstring x = all.substr(st, ed - st + 1);
	auto pos = tail.find(x);
	if (pos == wstring::npos) {
		if (ed + 1 < all.length()) {
			auto tmp = make_tail2(all.substr(ed + 1), tail);
			if (tmp != L"")
				return tmp.substr(1);
			else
				return tail;
		}
		else
			return tail;
	}
	wstring ret;
	for (int i = (int)pos - 1;i >= 0 && tail[i] != L'/';i--)
		ret = tail[i] + ret;
	ret += x;
	if (ed + 1 < all.length())
		return trimx(ret) + make_tail2(all.substr(ed + 1), tail);
	else
		return trimx(ret);
}
void  match(wstring match, vector<wstring>&ve, wstring beg) {
	ve.clear();
	try {
		string reg_wstring;
		string mi, ma;
		bool tg = 1;
		for (int i = 0;match[i];i++) {
			if (match[i] == L'*' || match[i] == L'-') {
				ma += "zzzzzzzzzzzzzzzzz";
				reg_wstring += ".*";
				tg = 0;
			}
			else if (match[i] == L'?') {
				if (tg)mi += "a";
				ma += "z";
				reg_wstring += ".";
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
		regex reg(reg_wstring);
		int cnt = 0;
		auto be = vocs.lower_bound(mi), ed = vocs.upper_bound(ma);
		if (beg != L"")
			be = vocs.upper_bound(w2s(beg));
		for (auto it = be;it != ed;++it)
			if (regex_match(*it, reg)) {
				ve.push_back(s2w(*it));
				if (++cnt == 50)break;
			}
	}
	catch (std::regex_error& error) {
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
			if (match[i] == L'*' || match[i] == L'-') {
				ma += 0xEFFF;
				reg_wstring += L".*";
				tg = 0;
			}
			else if (match[i] == L'?') {
				if (tg)mi += L"a";
				ma += L"z";
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
				if (++cnt == 15)break;
			}
		be = root.lower_bound(mi), ed = root.upper_bound(ma);
		if (beg != L"")be = root.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->f, reg)) {
				ve.push_back(L"-" + it->f + L"-");
				if (++cnt == 30)break;
			}
		be = suffix.lower_bound(mi), ed = suffix.upper_bound(ma);
		if (beg != L"")be = suffix.upper_bound(beg);
		for (auto it = be;it != ed;++it)
			if (regex_match(it->f, reg)) {
				ve.push_back(L"-" + it->f);
				if (++cnt == 45)break;
			}
		sort(ve.begin(), ve.end(), [](wstring a, wstring b) {
			if (a[0] == '-'&&b[0] == '-')return a < b;
			else if (a[0] == '-')return a.substr(1) < b;
			else if (b[0] == '-')return a < b.substr(1);
			else return a < b;
		});
	}
	catch (std::regex_error& error) {
		error;
	}
	return;
}
vector<wstring> match_via_reg(wstring match) {
	vector<wstring>ve;
	try {
		for (int i = 0; match[i]; i++)
			if (match[i] >= 'A'&&match[i] <= L'Z')
				match[i] = match[i] - 'A' + 'a';
		regex reg(w2s(match));
		int cnt = 0;
		for (auto &x : vocs)
			if (regex_match(x, reg)) {
				ve.push_back(s2w(x));
				if (++cnt == 50)break;
			}
	}
	catch (std::regex_error& error) {
		error;

	}
	return ve;
}

vector<wstring> Show2(wstring s) {
	wstring input = s;
	int len = (int)s.length();
	bool sp = 0;
	for (int i = 0;i < len;i++)
		if (s[i] >= 'A'&&s[i] <= 'Z')
			s[i] = s[i] - 'A' + 'a';
		else if (s[i] >= 'a'&&s[i] <= 'z')
			s[i] = s[i];
		else
			sp = 1, s[i] = ' ';
		if (sp) {
			wstringstream str(s);
			vector<wstring> ret;
			while (str >> s)
				ret.push_back(s);
			return ret;
		}
		if (ok_words.find(s) != ok_words.end()) {
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
					else
						mds.push_back(x);
				}
				vector<wstring> ret;
				if (bes != L"")
					ret.push_back(bes);
				int sz = (int)mds.size();
				for (int i = 0;i < sz;i++)
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
			wstring bes, eds;
			vector<wstring> mds;
			for (int i = len;i > 0;i--) {
				wstring sr = s.substr(0, i);
				if (prefix.find(sr) != prefix.end() && prefix[sr] != L"") {
					bes = sr + L"-";
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if (i >= 3 && i != len&&root.find(sr) != root.end() && root[sr] != L"") {
					bes = L"-" + sr + L"-";
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if (i >= 4 && i != len&&words.find(sr) != words.end() && words[sr] != L"") {
					bes = sr;
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
			}
			for (int i = 0;i < len;i++) {
				wstring sr = s.substr(i, len - i);
				if ((len - i != len || bes != L"") && suffix.find(sr) != suffix.end() && suffix[sr] != L"") {
					eds = L"-" + sr;
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if ((len - i != len || bes != L"") && len - i >= 4 && root.find(sr) != root.end() && root[sr] != L"") {
					eds = L"-" + sr + L"-";
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if ((len - i != len || bes != L"") && len - i >= 5 && words.find(sr) != words.end() && words[sr] != L"") {
					eds = sr;
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
			}
			bool addp = 1;
			for (int i = 0;i < len;i++) {
			flag:;
				for (int j = len - 1;j >= i;j--) {
					wstring sr = s.substr(i, j - i + 1);
					if (root.find(sr) != root.end() && root[sr] != L"") {
						addp = 1;
						mds.push_back(L"-" + sr + L"-");
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}
					else if ((j - i + 1) >= 3 && words.find(sr) != words.end() && words[sr] != L"") {
						addp = 1;
						mds.push_back(sr);
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}
					else if ((j - i + 1) >= 4 && words.find(sr + L"e") != words.end() && words[sr + L"e"] != L"") {
						addp = 1;
						mds.push_back(sr);
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}

				}
				if (i < len) {
					if (addp)mds.push_back(L"");
					mds.back() += s[i];
					addp = 0;
				}
			}
			vector< wstring> ret;
			if (bes != L"")
				ret.push_back(bes);
			int sz = (int)mds.size();
			for (int i = 0;i < sz;i++)
				ret.push_back(mds[i]);
			if (eds != L"")
				ret.push_back(eds);
			return ret;
		}
}
vector<pair<wstring, wstring>> Show(wstring s) {
	s = trim(s);
	wstring input = s;
	int len = (int)s.length();
	bool sp = 0;
	for (int i = 0;i < len;i++)
		if (s[i] >= 'A'&&s[i] <= 'Z')
			s[i] = s[i] - 'A' + 'a';
		else if (s[i] >= 'a'&&s[i] <= 'z')
			s[i] = s[i];
		else
			sp = 1, s[i] = ' ';
		if (sp) {
			wstringstream str(s);
			vector<pair<wstring, wstring>> ret;
			while (str >> s)
				if (words.find(s) == words.end())
					ret.push_back(make_pair(s, s));
				else
					ret.push_back(make_pair(words[s], s));
			return ret;
		}
		if (ok_words.find(s) != ok_words.end()) {
			wstringstream str(ok_words[s]);
			deque<wstring>dq;
			int cnt = 0;
			while (str >> s) {
				cnt++;
				dq.push_back(s);
			}
			if (cnt == 1) {
				vector<pair<wstring, wstring>> ret;
				ret.push_back(make_pair(words[s], s));
				return ret;
			}
			else if (cnt >= 2) {
				wstring bes, eds;
				wstring be, ed;
				vector<wstring> mds, md;
				if (prefix.find(dq.front()) != prefix.end())
					bes += dq.front(), be = prefix[dq.front()], dq.pop_front();
				if (suffix.find(dq.back()) != suffix.end())
					eds += dq.back(), ed = make_tail(input, suffix[dq.back()]), dq.pop_back();
				bool nbe = 0;
				for (auto &x : dq) {
					nbe = 1;
					if (root.find(x) != root.end())
						mds.push_back(x), md.push_back(root[x]);
					else if (words.find(x) != words.end())
						mds.push_back(x), md.push_back(words[x]);
					else
						mds.push_back(x), md.push_back(x);
				}
				vector<pair<wstring, wstring>> ret;
				if (be != L"")
					ret.push_back(make_pair(be, bes));
				int sz = (int)md.size();
				for (int i = 0;i < sz;i++)
					ret.push_back(make_pair(md[i], mds[i]));
				if (ed != L"")
					ret.push_back(make_pair(ed, eds));
				return ret;
			}
			else {
				return vector<pair<wstring, wstring>>();
			}
		}
		else {
			wstring bes, eds;
			wstring be, ed;
			vector<wstring> mds, md;
			for (int i = len;i > 0;i--) {
				wstring sr = s.substr(0, i);
				if (prefix.find(sr) != prefix.end() && prefix[sr] != L"") {
					be = prefix[sr];
					bes = sr;
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if (i >= 3 && i != len&&root.find(sr) != root.end() && root[sr] != L"") {
					be = root[sr];
					bes = sr;
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if (i >= 4 && i != len&&words.find(sr) != words.end() && words[sr] != L"") {
					be = words[sr];
					bes = sr;
					if (i != len)s = s.substr(i);
					else s = L"";
					len = (int)s.length();
					break;
				}
			}
			for (int i = 0;i < len;i++) {
				wstring sr = s.substr(i, len - i);
				if ((len - i != len || be != L"") && suffix.find(sr) != suffix.end() && suffix[sr] != L"") {
					ed = make_tail(input, suffix[sr]);
					eds = sr;
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if ((len - i != len || be != L"") && len - i >= 4 && root.find(sr) != root.end() && root[sr] != L"") {
					ed = root[sr];
					eds = sr;
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
				else if ((len - i != len || be != L"") && len - i >= 5 && i != len&&words.find(sr) != words.end() && words[sr] != L"") {
					ed = words[sr];
					eds = sr;
					if (i)s = s.substr(0, i);
					else s = L"";
					len = (int)s.length();
					break;
				}
			}
			bool addp = 1;
			for (int i = 0;i < len;i++) {
			flag:;
				for (int j = len - 1;j >= i;j--) {
					wstring sr = s.substr(i, j - i + 1);
					if (root.find(sr) != root.end() && root[sr] != L"") {
						addp = 1;
						md.push_back(root[sr]);
						mds.push_back(sr);
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}
					else if ((j - i + 1) >= 3 && words.find(sr) != words.end() && words[sr] != L"") {
						addp = 1;
						md.push_back(words[sr]);
						mds.push_back(sr);
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}
					else if ((j - i + 1) >= 5 && words.find(sr + L"e") != words.end() && words[sr + L"e"] != L"") {
						addp = 1;
						md.push_back(words[sr + L"e"]);
						mds.push_back(sr);
						wstring a, b;
						if (i)a = s.substr(0, i);
						if (j + 1 != len)b = s.substr(j + 1);
						s = a + b;
						len = (int)s.length();
						goto flag;
					}

				}
				if (i < len) {
					if (addp)md.push_back(L""), mds.push_back(L"");
					mds.back() += s[i];
					md.back() += s[i];
					addp = 0;
				}
			}
			vector<pair<wstring, wstring>> ret;
			if (be != L"")
				ret.push_back(make_pair(be, bes));
			int sz = (int)md.size();
			for (int i = 0;i < sz;i++)
				ret.push_back(make_pair(md[i], mds[i]));
			if (ed != L"")
				ret.push_back(make_pair(ed, eds));
			return ret;
		}
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
	int s;
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
	return make_pair(s.substr(0, pos), ve);
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
map<wstring, set<wstring>>rt;
int loading_cnt;
wstring LowwerCase(wstring s) {
	for (auto &x : s)
		x |= 32;
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

bool scroll_load_not_finish = 0;
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
