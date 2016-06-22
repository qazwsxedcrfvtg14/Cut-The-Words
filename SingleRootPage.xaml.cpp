//
// SingleRootPage.xaml.cpp
// Implementation of the SingleRootPage class
//

#include "pch.h"
#include "SingleRootPage.xaml.h"
#include "SearchRootPage.xaml.h"
#include "SingleVocPage.xaml.h"
#include "Voc.h"
using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Advertising::WinRT::UI;

using namespace CutTheWords::Views;

SingleRootPage::SingleRootPage()
{
	InitializeComponent();
}


void SingleRootPage::OnNavigatedFrom(NavigationEventArgs^ e) {
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;

	Page::OnNavigatedFrom(e);
}

void SingleRootPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	EditPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	DelPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	EditPanelVis = 0;
	DelPanelVis = 0;
	auto param = dynamic_cast<String^>(e->Parameter);
	if (param != nullptr)
	{
		//std::wstring msg(param->Data());
		//const wchar_t* format = L"Clicked on %s";
		//wchar_t buffer[75];
		//swprintf_s(buffer, 75, format, msg.c_str());

		//Vocabulary = ref new String(buffer);
		wstring s = param->Data();
		title->Text = param;
		_voc = trim(s);
		_exp = GetRootExp(s);
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		for (auto x : CutExp(_exp)) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 20;
			voc_root->Children->Append(tmp.back());
		}
		create_task([=] {
			vector<wstring>ve;
			if (_voc.front() == '-'&&_voc.back() == '-')
				for (auto x : root)
					if (L"-" + x.first + L"-" != _voc&&GetRootExp(L"-" + x.f + L"-") == _exp)
						ve.push_back(x.f);
			if (_voc.front() == '-'&&_voc.back() != '-')
				for (auto x : suffix)
					if (L"-" + x.first != _voc&&GetRootExp(L"-" + x.f) == _exp)
						ve.push_back(x.f);
			if (_voc.front() != '-'&&_voc.back() == '-')
				for (auto x : prefix)
					if (x.first + L"-" != _voc&&GetRootExp(x.f + L"-") == _exp)
						ve.push_back(x.f);
			return ve;
		}).then([this](vector<wstring>ve) {
			Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, ve]()
			{
				alias_list->Items->Clear();
				for (auto &x : ve)
					alias_list->Items->Append(ref new String(x.c_str()));
			}));
		});

		VocList->Items->Clear();
		VocList->IsItemClickEnabled = false;
		VocList->Items->Append("讀取中...");
		create_task([=] {
			vector<wstring> ve;
			wstring data;
			wstring reg_string;
			int len = (int)_voc.length();
			if (_voc[0] == '-' && _voc.back() == '-')
				data = _voc.substr(1, len - 2);
			else if (_voc[0] != '-' && _voc.back() == '-')
				data = _voc.substr(0, len - 1);
			else if (_voc[0] == '-' && _voc.back() != '-')
				data = _voc.substr(1, len - 1);

			reg_string = L".*" + data + L".*";
			if (data.length() == 1)
			{
				if (_voc[0] == '-')
					reg_string = L".*" + data;
				else
					reg_string = data + L".*";

			}
			wregex reg(reg_string);
			int cnt = 0;
			bool brk = false;
			int cnt2 = 0;
			for (auto &x : words) {
				if (brk)break;
				if (!regex_match(x.f, reg) || x.f == data)continue;
				auto s2 = Show2(x.f);
				for (auto &y : s2) {
					if (y == _voc) {
						ve.push_back(x.f);
						if (++cnt2 == 50)brk = true;
					}
				}
			}
			return ve;
		}).then([this](vector<wstring>ve) {
			Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, ve]()
			{
				VocList->Items->Clear();
				VocList->IsItemClickEnabled = true;
				for (auto &x : ve) {
					auto stp = ref new StackPanel();
					stp->Orientation = Orientation::Horizontal;
					auto tmp = ref new TextBlock();
					tmp->Text = ref new String(x.c_str());
					stp->Children->Append(tmp);
					tmp = ref new TextBlock();
					wstring _exp = GetExpSimple(words[x]);
					_exp = trim(_exp);
					tmp->Text = ref new String(_exp.c_str());
					tmp->Margin = Thickness(20, 0, 0, 0);
					stp->Children->Append(tmp);
					VocList->Items->Append(stp);
				}
			}));
		});

		/*Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 10000;
		tempdispatchertime->Interval = time;
		VocList->Items->Clear();
		VocList->IsItemClickEnabled = false;
		VocList->Items->Append("讀取中...");
		if (rtp != nullptr) {
			VocList->Items->Clear();
			int cnt = 0;
			for (auto &x : (*rtp)[s]) {
				auto stp = ref new StackPanel();
				stp->Orientation = Orientation::Horizontal;
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(x.c_str());
				stp->Children->Append(tmp);
				tmp = ref new TextBlock();
				int len = (int)x.length();
				wstring _exp = GetExpSimple(words[x]);
				_exp = trim(_exp);
				tmp->Text = ref new String(_exp.c_str());
				tmp->Margin = Thickness(20, 0, 0, 0);
				stp->Children->Append(tmp);
				VocList->Items->Append(stp);
				if (++cnt == 50)break;
			}
			VocList->IsItemClickEnabled = true;
		}
		else {
			auto timerDelegate = [=](Object^ e, Object^ ags) {
				if (rtp != nullptr) {
					VocList->Items->Clear();
					int cnt = 0;
					for (auto &x : (*rtp)[s]) {
						auto stp = ref new StackPanel();
						stp->Orientation = Orientation::Horizontal;
						auto tmp = ref new TextBlock();
						tmp->Text = ref new String(x.c_str());
						stp->Children->Append(tmp);
						tmp = ref new TextBlock();
						int len = (int)x.length();
						wstring _exp = GetExpSimple(words[x]);
						_exp = trim(_exp);
						tmp->Text = ref new String(_exp.c_str());
						tmp->Margin = Thickness(20, 0, 0, 0);
						stp->Children->Append(tmp);
						VocList->Items->Append(stp);
						if (++cnt == 50)break;
					}
					VocList->IsItemClickEnabled = true;
					tempdispatchertime->Stop();
				}
			};
			tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
			tempdispatchertime->Start();
		}*/
	}
	else
	{
		title->Text = "#######";
	}
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;

	Page::OnNavigatedTo(e);
}

void SingleRootPage::EditButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	if (EditPanelVis)
		FadOutEditPanel->Begin(), EditPanelVis = 0;
	else {
		if (DelPanelVis)
			FadOutDelPanel->Begin(), DelPanelVis = 0;
		block->Text = ref new String(GetRootExpOrg(_voc).c_str());
		block->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		FadInEditPanel->Begin(), EditPanelVis = 1;
	}
}
void SingleRootPage::DeleteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	if (DelPanelVis)
		FadOutDelPanel->Begin(), DelPanelVis = 0;
	else {
		if (EditPanelVis)
			FadOutEditPanel->Begin(), EditPanelVis = 0;
		FadInDelPanel->Begin(), DelPanelVis = 1;
	}
}
void SingleRootPage::ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {

	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring a(_voc), b(block->Text->Data());
		int len = (int)a.length();
		if (a[0] == '-'&&a[len - 1] == '-') {
			wstring s = a.substr(1, len - 2);
			root[s] = b;
			AppendStrToFile(s + L"," + b + L"\n", L"root_user.txt");
		}
		else if (a[0] == '-') {
			wstring s = a.substr(1);
			suffix[s] = b;
			AppendStrToFile(s + L"," + b + L"\n", L"suffix_user.txt");
		}
		else if (a[len - 1] == '-') {
			wstring s = a.substr(0, len - 1);
			prefix[s] = b;
			AppendStrToFile(s + L"," + b + L"\n", L"prefix_user.txt");
		}
		else
			return;
		
		_exp = GetRootExp(a);
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		for (auto x : CutExp(_exp)) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 20;
			voc_root->Children->Append(tmp.back());
		}
		alias_list->Items->Clear();
		if (_voc.front() == '-'&&_voc.back() == '-')
			for (auto x : root)
				if (L"-" + x.first + L"-" != _voc&&GetRootExp(L"-" + x.f + L"-") == _exp)
					alias_list->Items->Append(ref new String(x.first.c_str()));
		if (_voc.front() == '-'&&_voc.back() != '-')
			for (auto x : suffix)
				if (L"-" + x.first != _voc&&GetRootExp(L"-" + x.f) == _exp)
					alias_list->Items->Append(ref new String(x.first.c_str()));
		if (_voc.front() != '-'&&_voc.back() == '-')
			for (auto x : prefix)
				if (x.first + L"-" != _voc&&GetRootExp(x.f + L"-") == _exp)
					alias_list->Items->Append(ref new String(x.first.c_str()));
		EditButton_Click(sender, e);
	}
	else if (str == "取消")
		EditButton_Click(sender, e);
}
void SingleRootPage::DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring s(_voc);
		wstring a(_voc);
		int len = (int)a.length();
		if (a[0] == '-'&&a[len - 1] == '-') {
			wstring s = a.substr(1, len - 2);
			root.erase(s);
			AppendStrToFile(L"$" + s + L"\n", L"root_user.txt");
		}
		else if (a[0] == '-') {
			wstring s = a.substr(1);
			suffix.erase(s);
			AppendStrToFile(L"$" + s + L"\n", L"suffix_user.txt");
		}
		else if (a[len - 1] == '-') {
			wstring s = a.substr(0, len - 1);
			prefix.erase(s);
			AppendStrToFile(L"$" + s + L"\n", L"prefix_user.txt");
		}
		else
			return;

		DeleteButton_Click(sender, e);
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SearchRootPage::typeid),
				ref new String(_voc.c_str()),
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (str == "取消")
		DeleteButton_Click(sender, e);
}

void SingleRootPage::PageKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Back&&!DelPanelVis&&!EditPanelVis) {
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SingleRootPage::typeid),
				ref new String(_voc.c_str()),
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}

void SingleRootPage::VocListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)(((StackPanel^)(e->ClickedItem))->Children->GetAt(0)))->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}

void SingleRootPage::OnErrorOccurred(Object^ sender, AdErrorEventArgs^ e)
{
	//ShowMsg(wstring(L"An error occurred. : ") + e->ErrorMessage->Data());
}

void SingleRootPage::OnAdRefreshed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args) {
}

void CutTheWords::Views::SingleRootPage::alias_list_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str != nullptr) {
		if (_voc.front() == '-')
			str = L"-" + str;
		if (_voc.back() == '-')
			str = str + L"-";
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			str,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}
