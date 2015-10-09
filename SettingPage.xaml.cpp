//
// SettingPage.xaml.cpp
// SettingPage 類別的實作
//

#include "pch.h"
#include "SettingPage.xaml.h"
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

// 空白頁項目範本已記錄在 http://go.microsoft.com/fwlink/?LinkId=234238

SettingPage::SettingPage()
{
	InitializeComponent();
}

void SettingPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	auto str=dynamic_cast<String^>(e->ClickedItem);
	if (str == "重置單字庫") {
		if (!inited) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		create_task([] {
			//DumpAppFile(L"words.txt");
			//DumpAppFile(L"prefix.txt");
			//DumpAppFile(L"suffix.txt");
			//DumpAppFile(L"root.txt");
			StrToFile(L"", L"words_user.txt");
			StrToFile(L"", L"prefix_user.txt");
			StrToFile(L"", L"suffix_user.txt");
			StrToFile(L"", L"root_user.txt");
			get_doc(L"words.txt", words, ok_words);
			get_doc(L"prefix.txt", prefix, nil_words);
			get_doc(L"suffix.txt", suffix, nil_words);
			get_doc(L"root.txt", root, nil_words);
			vocs.clear();
			for (auto x : words) {
				string s;
				for (auto y : x.f)
					s += (char)y;
				vocs.insert(s);
			}
			inited = 0;
			create_task([=] {
				for (auto &x : words) {
					auto ve = Show2(x.f);
					for (auto &y : ve) {
						if (y != x.f)rt[y].insert(x.f);
					}
				}
				inited = 1;
			});
			}).then(HideLoading, task_continuation_context::use_current());
	}
	else if (str == "重置應用程式") {
		if (!inited) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		create_task([] {
			DumpAppFile(L"setting.txt");
			DumpAppFile(L"favorite.txt");
			DumpAppFile(L"words.txt");
			DumpAppFile(L"prefix.txt");
			DumpAppFile(L"suffix.txt");
			DumpAppFile(L"root.txt");
			DumpAppFile(L"note.txt");
			StrToFile(L"", L"words_user.txt");
			StrToFile(L"", L"prefix_user.txt");
			StrToFile(L"", L"suffix_user.txt");
			StrToFile(L"", L"root_user.txt");
			get_doc(L"setting.txt", setting, nil_words);
			get_doc(L"favorite.txt", favorite, nil_words);
			get_doc(L"note.txt", note, nil_words);
			get_doc(L"words.txt", words, ok_words);
			get_doc(L"prefix.txt", prefix, nil_words);
			get_doc(L"suffix.txt", suffix, nil_words);
			get_doc(L"root.txt", root, nil_words);
			vocs.clear();
			for (auto x : words) {
				string s;
				for (auto y : x.f)
					s += (char)y;
				vocs.insert(s);
			}
			inited = 0;
			create_task([=] {
				for (auto &x : words) {
					auto ve = Show2(x.f);
					for (auto &y : ve) {
						if (y != x.f)rt[y].insert(x.f);
					}
				}
				inited = 1;
			});
			}).then(HideLoading, task_continuation_context::use_current());
	}
	else if (str == "上傳單字庫") {
		if (!inited) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		loading_cnt = 0;
		post(website+L"/words.php", FileToStr(L"words.txt") + FileToStr(L"words_user.txt"), [=](wstring s) {
			loading_cnt++;
		}, [=] {ShowMsg(L"Upload Words Error!");loading_cnt++;});
		post(website+L"/prefix.php", FileToStr(L"prefix.txt") + FileToStr(L"prefix_user.txt"), [=](wstring s) {
			loading_cnt++;
		}, [=] {ShowMsg(L"Upload Prefix Error!");loading_cnt++;});
		post(website+L"/suffix.php", FileToStr(L"suffix.txt") + FileToStr(L"suffix_user.txt"), [=](wstring s) {
			loading_cnt++;
		}, [=] {ShowMsg(L"Upload Suffix Error!");loading_cnt++;});
		post(website+L"/root.php", FileToStr(L"root.txt") + FileToStr(L"root_user.txt"), [=](wstring s) {
			loading_cnt++;
		}, [=] {ShowMsg(L"Upload Root Error!");loading_cnt++;});
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 10000;
		tempdispatchertime->Interval = time;
		auto timerDelegate = [=](Object^ e, Object^ ags) {
			if (loading_cnt == 4) {
				HideLoading();
				tempdispatchertime->Stop();
			}
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();

	}
	else if (str == "更新單字庫") {
		if (!inited) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		loading_cnt = 0;
		post(website+L"/words.php", L"", [=](wstring s) {
			StrToFile(s, L"words.txt");
			get_doc(L"words.txt", words, ok_words);
			vocs.clear();
			for (auto x : words) {
				string s;
				for (auto y : x.f)
					s += (char)y;
				vocs.insert(s);
			}
			loading_cnt++;
		}, [=] {ShowMsg(L"Download Words Error!");loading_cnt++;});
		post(website+L"/prefix.php", L"", [=](wstring s) {
			StrToFile(s, L"prefix.txt");
			get_doc(L"prefix.txt", prefix, nil_words1);
			loading_cnt++;
		}, [=] {ShowMsg(L"Download Prefix Error!");loading_cnt++;});
		post(website+L"/suffix.php", L"", [=](wstring s) {
			StrToFile(s, L"suffix.txt");
			get_doc(L"suffix.txt", suffix, nil_words2);
			loading_cnt++;
		}, [=] {ShowMsg(L"Download Suffix Error!");loading_cnt++;});
		post(website+L"/root.php", L"", [=](wstring s) {
			StrToFile(s, L"root.txt");
			get_doc(L"root.txt", root, nil_words3);
			loading_cnt++;
		}, [=] {ShowMsg(L"Download Root Error!");loading_cnt++;});
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 10000;
		tempdispatchertime->Interval = time;
		auto timerDelegate = [=](Object^ e, Object^ ags) {
			if (loading_cnt == 4) {
				HideLoading();
				inited = 0;
				create_task([=] {
					for (auto &x : words) {
						auto ve = Show2(x.f);
						for (auto &y : ve) {
							if (y != x.f)rt[y].insert(x.f);
						}
					}
					inited = 1;
				});
				tempdispatchertime->Stop();
			}
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();

	}
	else if (str == "設定為淡色主題") {
		setting[L"theme"] = L"light";
		wstring out;
		for (auto &x : setting)
			out += x.f + L"," + x.s + L"\n";
		StrToFile(out, L"setting.txt");
		ShowMsg(L"設定完成，重開應用程式後生效");
	}
	else if (str == "設定為深色主題") {
		setting[L"theme"] = L"dark";
		wstring out;
		for (auto &x : setting)
			out += x.f + L"," + x.s + L"\n";
		StrToFile(out, L"setting.txt");
		ShowMsg(L"設定完成，重開應用程式後生效");
	}
	else if (str == "單字庫選項") {
		set_list->Items->Clear();
		set_list->Items->Append("上傳單字庫");
		set_list->Items->Append("更新單字庫");
		set_list->Items->Append("重置單字庫");
		set_list->Items->Append("重置應用程式");
		set_list->Items->Append("回設定主頁");
	}
	else if (str == "設定主題色彩") {
		set_list->Items->Clear();
		set_list->Items->Append("設定為淡色主題");
		set_list->Items->Append("設定為深色主題");
		set_list->Items->Append("回設定主頁");
	}
	else if (str == "回設定主頁") {
		set_list->Items->Clear();
		set_list->Items->Append("設定主題色彩");
		set_list->Items->Append("單字庫選項");
	}
	else if (str == "關於") {
		ShowMsg(L"版本號:ver 1.0.14");
	}
	else if (str == "Test") {

		auto ocrLanguage = ref new Windows::Globalization::Language("en");
		auto ocrEngine = Windows::Media::Ocr::OcrEngine::TryCreateFromLanguage(ocrLanguage);
	}
	else {
		throw("Error");
	}
	//if()
	/*Frame->Navigate(
		TypeName(BasicSubPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	*/
}
