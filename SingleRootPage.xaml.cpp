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

using namespace CutTheWords::Views;

SingleRootPage::SingleRootPage()
{
	InitializeComponent();
}


void SingleRootPage::OnNavigatedFrom(NavigationEventArgs^ e) {
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;

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
		Vocabulary = param;
		int len = (int) s.length();
		if (s[0] == '-'&&s[len - 1] == '-')
			_exp = root[s.substr(1,len-2)];
		else if (s[0] == '-')
			_exp = suffix[s.substr(1)];
		else if (s[len - 1] == '-')
			_exp = prefix[s.substr(0, len - 1)];
		else
			_exp = words[param->Data()];
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		for (auto x : CutExp(_exp)) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 20;
			voc_root->Children->Append(tmp.back());
		}
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		Windows::Foundation::TimeSpan time;
		time.Duration = 10000;
		tempdispatchertime->Interval = time;
		VocList->Items->Clear();
		VocList->IsItemClickEnabled = false;
		VocList->Items->Append("讀取中...");
		if (inited) {
			VocList->Items->Clear();
			int cnt = 0;
			for (auto &x : rt[s]) {
				VocList->Items->Append(ref new String(x.c_str()));
				if (++cnt == 50)break;
			}
			VocList->IsItemClickEnabled = true;
		}
		else {
			auto timerDelegate = [=](Object^ e, Object^ ags) {
				if (inited) {
					VocList->Items->Clear();
					int cnt = 0;
					for (auto &x : rt[s]) {
						VocList->Items->Append(ref new String(x.c_str()));
						if (++cnt == 50)break;
					}
					VocList->IsItemClickEnabled = true;
					tempdispatchertime->Stop();
				}
			};
			tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
			tempdispatchertime->Start();
		}
	}
	else
	{
		Vocabulary = "#######";
	}
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;

	Page::OnNavigatedTo(e);
}

String^ SingleRootPage::Vocabulary::get()
{
	return _voc;
}

void SingleRootPage::Vocabulary::set(String^ value)
{
	_voc = value;
}

void SingleRootPage::EditButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	if (EditPanelVis)
		FadOutEditPanel->Begin(), EditPanelVis = 0;
	else {
		if (DelPanelVis)
			FadOutDelPanel->Begin(), DelPanelVis = 0;
		block->Text = ref new String(_exp.c_str());
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
		wstring a(Vocabulary->Data()), b(block->Text->Data());
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
		
		_exp = b;
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		for (auto x : CutExp(_exp)) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 20;
			voc_root->Children->Append(tmp.back());
		}
		EditButton_Click(sender, e);
	}
	else if (str == "取消")
		EditButton_Click(sender, e);
}
void SingleRootPage::DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring s(Vocabulary->Data());
		wstring a(Vocabulary->Data());
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
				Vocabulary,
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
				Vocabulary,
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}

void SingleRootPage::VocListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
