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
	host_name->Text = ref new String(setting[L"website"].c_str());
}


void SettingPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	String^ str;
	if (e != nullptr)
		str=dynamic_cast<String^>(e->ClickedItem);
	else
		str= dynamic_cast<String^>(sender);
	if (str == nullptr)
		return;
	if (str == "重置單字庫") {
		//if (rtp==nullptr) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
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
			get_doc(L"prefix.txt", prefix);
			get_doc(L"suffix.txt", suffix);
			get_doc(L"root.txt", root);
			
			}).then(HideLoading, task_continuation_context::use_current());
	}
	else if (str == "重置應用程式") {
		//if (rtp==nullptr) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
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
			StrToFile(L"", L"note_user.txt");
			get_doc(L"setting.txt", setting);
			get_doc(L"favorite.txt", favorite);
			get_doc(L"note.txt", note);
			get_doc(L"words.txt", words, ok_words);
			get_doc(L"prefix.txt", prefix);
			get_doc(L"suffix.txt", suffix);
			get_doc(L"root.txt", root);
			
			}).then(HideLoading, task_continuation_context::use_current());
	}
	else if (str == "上傳單字庫") {
		//if (rtp==nullptr) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		loading_cnt = 0;
		post(setting[L"website"] + L"/version.php", L"", [=](wstring s) {
			if (StrToInt(setting[L"data_version"]) < StrToInt(s)){
				ShowMsg(L"Version Error!");
				loading_cnt = 6;
			}
			else {
				setting[L"data_version"] = IntToStr(StrToInt(setting[L"data_version"]) + 1);
				SavingSetting();
				HttpMultipartFormDataContent^ post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String(setting[L"data_version"].c_str())), "post");
				post(setting[L"website"] + L"/version.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Update Version Error!"); loading_cnt++; });
				post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String((dump_doc(words,ok_words)).c_str())), "post");
				post(setting[L"website"] + L"/words.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Upload Words Error!"); loading_cnt++; });
				post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String((dump_doc(prefix)).c_str())), "post");
				post(setting[L"website"] + L"/prefix.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Upload Prefix Error!"); loading_cnt++; });
				post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String((dump_doc(suffix)).c_str())), "post");
				post(setting[L"website"] + L"/suffix.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Upload Suffix Error!"); loading_cnt++; });
				post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String((dump_doc(root)).c_str())), "post");
				post(setting[L"website"] + L"/root.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Upload Root Error!"); loading_cnt++; });
				post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String((dump_doc(note)).c_str())), "post");
				post(setting[L"website"] + L"/note.php", post_data, [=](wstring s) {
					loading_cnt++;
				}, [=] {ShowMsg(L"Upload Note Error!"); loading_cnt++; });
			}
		}, [=] {ShowMsg(L"Network Error!"); loading_cnt = 6; });
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 1000000;
		tempdispatchertime->Interval = time;
		auto timerDelegate = [=](Object^ e, Object^ ags) {
			if (loading_cnt == 6) {
				HideLoading();
				tempdispatchertime->Stop();
			}
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();
	}
	else if (str == "更新單字庫") {
		//if (rtp == nullptr) { ShowMsg(L"應用程式初始化中，請稍後重試。"); return; }
		ShowLoading();
		loading_cnt = 0;
		post(setting[L"website"] + L"/version.php", L"", [=](wstring s) {
			if (StrToInt(setting[L"data_version"]) < StrToInt(s)) {
				setting[L"data_version"] = s;
				SavingSetting();
				post(setting[L"website"] + L"/words.php", L"", [=](wstring s) {
					StrToFile(s, L"words.txt");
					get_doc(L"words.txt", words, ok_words);
					loading_cnt++;
				}, [=] {ShowMsg(L"Download Words Error!"); loading_cnt++; });
				post(setting[L"website"] + L"/prefix.php", L"", [=](wstring s) {
					StrToFile(s, L"prefix.txt");
					get_doc(L"prefix.txt", prefix);
					loading_cnt++;
				}, [=] {ShowMsg(L"Download Prefix Error!"); loading_cnt++; });
				post(setting[L"website"] + L"/suffix.php", L"", [=](wstring s) {
					StrToFile(s, L"suffix.txt");
					get_doc(L"suffix.txt", suffix);
					loading_cnt++;
				}, [=] {ShowMsg(L"Download Suffix Error!"); loading_cnt++; });
				post(setting[L"website"] + L"/root.php", L"", [=](wstring s) {
					StrToFile(s, L"root.txt");
					get_doc(L"root.txt", root);
					loading_cnt++;
				}, [=] {ShowMsg(L"Download Root Error!"); loading_cnt++; });
				post(setting[L"website"] + L"/note.php", L"", [=](wstring s) {
					StrToFile(s, L"note.txt");
					get_doc(L"note.txt", note);
					loading_cnt++;
				}, [=] {ShowMsg(L"Download Note Error!"); loading_cnt++; });
			}
			else {
				ShowMsg(L"You already have the latest version of words.");
				loading_cnt = 5;
			}
		}, [=] {ShowMsg(L"Network Error!"); loading_cnt = 5; });
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 1000000;
		tempdispatchertime->Interval = time;
		auto timerDelegate = [=](Object^ e, Object^ ags) {
			if (loading_cnt == 5) {
				HideLoading();
				
				tempdispatchertime->Stop();
			}
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();

	}
	else if (str == "設定為淡色主題") {
		setting[L"theme"] = L"light";
		SavingSetting();
		ShowMsg(L"設定完成，重開應用程式後生效");
	}
	else if (str == "設定為深色主題") {
		setting[L"theme"] = L"dark";
		SavingSetting();
		ShowMsg(L"設定完成，重開應用程式後生效");
	}
	else if (str == "單字庫選項") {
		set_list->Items->Clear();
		if(setting[L"website"]==L"http://192.168.1.110/words")
			set_list->Items->Append("上傳單字庫");
		set_list->Items->Append("更新單字庫");
		//set_list->Items->Append("重置單字庫");
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
		set_list->Items->Append("聲音選項");
		set_list->Items->Append("單字庫選項");
		set_list->Items->Append("測驗選項");
		set_list->Items->Append("心智圖選項");
		set_list->Items->Append("啟動選項");
		set_list->Items->Append("網路選項");
		set_list->Items->Append("關於");
	}
	else if (str == "聲音選項") {
		set_list->Items->Clear();
		set_list->Items->Append("自動播放開啟");
		set_list->Items->Append("自動播放關閉");
		set_list->Items->Append("發音:Dictionary.com");
		set_list->Items->Append("發音:Bing 美國");
		set_list->Items->Append("發音:Bing 英國");
		set_list->Items->Append("回設定主頁");
	}
	else if (str == "自動播放開啟") {
		setting[L"auto_play"] = L"On";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "自動播放關閉") {
		setting[L"auto_play"] = L"Off";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "發音:Dictionary.com") {
		setting[L"sound_url"] = L"http://dictionary.reference.com/browse/";
		setting[L"sound_url2"] = L"http://static.sfdict.com/staticrep/dictaudio";
		setting[L"sound_type"] = L".mp3";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "發音:Bing 美國") {
		setting[L"sound_url"] = L"http://cn.bing.com/dict/search?mkt=zh-cn&q=";
		setting[L"sound_url2"] = L"https://dictionary.blob.core.chinacloudapi.cn/media/audio/tom";
		setting[L"sound_type"] = L".mp3";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "發音:Bing 英國") {
		setting[L"sound_url"] = L"http://cn.bing.com/dict/search?mkt=zh-cn&q=";
		setting[L"sound_url2"] = L"https://dictionary.blob.core.chinacloudapi.cn/media/audio/george";
		setting[L"sound_type"] = L".mp3";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "關於") {

		auto package = Package::Current;
		auto packageId = package->Id;
		auto version = packageId->Version;

		//return string.Format("{0}.{1}.{2}.{3}", version.Major, version.Minor, version.Build, version.Revision);
		//auto major = version.Major;

		ShowMsg(L"版本號:ver " + IntToStr(version.Major) + L"." + IntToStr(version.Minor) + L"." + IntToStr(version.Build) + L"." + IntToStr(version.Revision) );
	}
	else if (str == "Test") {
		auto ocrLanguage = ref new Windows::Globalization::Language("en");
		auto ocrEngine = Windows::Media::Ocr::OcrEngine::TryCreateFromLanguage(ocrLanguage);
	}
	else if (str == "心智圖選項") {
		set_list->Items->Clear();
		set_list->Items->Append("關閉心智圖選項");
		auto s = ref new StackPanel();
		s->Orientation = Orientation::Horizontal;
		auto tp = ref new TextBlock();
		tp->Text = "節點數量：";
		auto tmp = ref new TextBox();
		tmp->Text = ref new String(setting[L"mind_map_cnt"].c_str());
		tmp->TextChanged += ref new TextChangedEventHandler([=](Object^ sender, TextChangedEventArgs^ e) {
			setting[L"mind_map_cnt"] = tmp->Text->Data();
			SavingSetting();
		});
		s->Children->Append(tp);
		s->Children->Append(tmp);
		st->Children->Append(s);
	}
	else if (str == "關閉心智圖選項") {
		st->Children->RemoveAtEnd();
		ListView_ItemClick(ref new String(L"回設定主頁"),nullptr);
	}
	else if (str == "測驗選項") {
		set_list->Items->Clear();
		set_list->Items->Clear();
		set_list->Items->Append("關閉測驗選項");
		auto s = ref new StackPanel();
		s->Orientation = Orientation::Horizontal;
		auto tp = ref new TextBlock();
		tp->Text = "選擇題選項數量(2~100)：";
		auto tmp = ref new TextBox();
		if (setting[L"sellect_prob_cnt"] == L"")
			setting[L"sellect_prob_cnt"] = L"5";
		tmp->Text = ref new String(setting[L"sellect_prob_cnt"].c_str());
		tmp->TextChanged += ref new TextChangedEventHandler([=](Object^ sender, TextChangedEventArgs^ e) {
			setting[L"sellect_prob_cnt"] = tmp->Text->Data();
			SavingSetting();
		});
		s->Children->Append(tp);
		s->Children->Append(tmp);
		st->Children->Append(s);
	}
	else if (str == "關閉測驗選項") {
		st->Children->RemoveAtEnd();
		ListView_ItemClick(ref new String(L"回設定主頁"), nullptr);
	}
	else if (str == "啟動選項") {
		set_list->Items->Clear();
		set_list->Items->Append("啟動後進入首頁");
		set_list->Items->Append("啟動後進入單字搜尋頁面");
		set_list->Items->Append("回設定主頁");
	}
	else if (str == "啟動後進入首頁") {
		setting[L"home_page"] = L"home";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "啟動後進入單字搜尋頁面") {
		setting[L"home_page"] = L"search";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "網路選項") {
		set_list->Items->Clear();
		set_list->Items->Append("開啟網路音標");
		set_list->Items->Append("關閉網路音標");
		set_list->Items->Append("開啟網路發音");
		set_list->Items->Append("關閉網路發音");
		set_list->Items->Append("開啟網路圖片");
		set_list->Items->Append("關閉網路圖片");
		set_list->Items->Append("開啟網路例句");
		set_list->Items->Append("關閉網路例句");
		set_list->Items->Append("回設定主頁");
	}
	else if (str == "開啟網路音標") {
		setting[L"network_kk"] = L"true";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "關閉網路音標") {
		setting[L"network_kk"] = L"false";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "開啟網路發音") {
		setting[L"network_sound"] = L"true";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "關閉網路發音") {
		setting[L"network_sound"] = L"false";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "開啟網路圖片") {
		setting[L"network_picture"] = L"true";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "關閉網路圖片") {
		setting[L"network_picture"] = L"false";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "開啟網路例句") {
		setting[L"network_sent"] = L"true";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else if (str == "關閉網路例句") {
		setting[L"network_sent"] = L"false";
		SavingSetting();
		ShowMsg(L"設定成功");
	}
	else {
		throw("Option 404 Error");
	}
	//if()
	/*Frame->Navigate(
		TypeName(BasicSubPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	*/
}

void CutTheWords::Views::SettingPage::host_name_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	setting[L"website"] = host_name->Text->Data();
	wstring out;
	for (auto &x : setting)
		out += x.f + L"," + x.s + L"\n";
	StrToFile(out, L"setting.txt");
}
