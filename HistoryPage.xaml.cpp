//
// HistoryPage.xaml.cpp
// Implementation of the HistoryPage class
//

#include "pch.h"
#include "BasicSubPage.xaml.h"
#include "HistoryPage.xaml.h"
#include "Voc.h"

using namespace CutTheWords::Views;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

HistoryPage::HistoryPage()
{
	

	InitializeComponent();
}

void HistoryPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	Page::OnNavigatedTo(e);
	ShowLoading();
	create_task([=] {
		//ShowLoading();
		map<wstring, wstring> *old_words_p = new map<wstring, wstring>();
		map<wstring, wstring> *old_ok_p=new map<wstring, wstring>();
		get_doc(L"words.txt", *old_words_p, *old_ok_p, 0);
		return make_pair(old_words_p, old_ok_p);
	}, task_continuation_context::use_current()).then([=](pair<map<wstring, wstring>* ,map<wstring, wstring>*> old){
		map<wstring, wstring> &old_words=*(old.f), &old_ok = *(old.s);
		lis->Items->Clear();

		wstringstream str;
		str << FileToStr(L"words_user.txt");
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
					if (dis) {
						lis->Items->Append(ref new String((L"刪除 " + c).c_str()));
						old_words.erase(c); continue;
					}
					if (old_words.find(c) == old_words.end())
						lis->Items->Append(ref new String((L"增加 " + c).c_str()));
					else {
						auto x = GetExp(old_words[c]), y = GetExp(b);
						if (x.f != y.f)
							lis->Items->Append(ref new String((L"修改 " + c).c_str()));
						else if (x.s != y.s)
							lis->Items->Append(ref new String((L"選項修改 " + c).c_str()));
					}
					old_words[c] = b;
					if (star&&old_ok.find(c) == old_ok.end())
						lis->Items->Append(ref new String((L"固定拆法 " + c).c_str()));
					else if (star&&old_ok[c] != d)
						lis->Items->Append(ref new String((L"修改拆法 " + c).c_str()));
					if (star)old_ok[c] = d;
				}
		}
		HideLoading();
		delete old.f;
		delete old.s;
	}, task_continuation_context::use_current());

}
void HistoryPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	/*Frame->Navigate(
		TypeName(BasicSubPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());*/
}
