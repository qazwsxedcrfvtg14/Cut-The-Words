//
// SearchVocPage.xaml.cpp
// Implementation of the SearchVocPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "SearchVocPage.xaml.h"
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
//using namespace Jtol;
//map<string, string> ok_words, nil;
//map<string, string> words, prefix, suffix, root;
SearchVocPage::SearchVocPage()
{
	InitializeComponent();
}

Object^ SearchVocPage_Navigate_Obj;
void SearchVocPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	/*auto param = dynamic_cast<String^>(e->Parameter);
	input_voc->Text = ref new String(param->Data());*/
	running_target = 0;
	auto svp = dynamic_cast<String^>(SearchVocPage_Navigate_Obj);
	if (svp != nullptr) {
		input_voc->Text = svp;
		input_voc->SelectAll();
	}
	wstring q = input_voc->Text->Data();
	vector<wstring> ve;
	match(q + L"*", ve);
	VocList->Items->Clear();
	int cnt = 0;
	for (auto x : ve) {
		auto stp = ref new StackPanel();
		stp->Orientation = Orientation::Horizontal;
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(x.c_str());
		stp->Children->Append(tmp);
		tmp = ref new TextBlock();
		int len = (int)x.length();
		wstring _exp= GetExp(words[x]).f;
		_exp = trim(_exp);
		tmp->Text = ref new String(_exp.c_str());
		tmp->Margin = Thickness(20, 0, 0, 0);
		stp->Children->Append(tmp);
		VocList->Items->Append(stp);
	}
	if (ve.size())
		scroll_load_not_finish = 1;
	else
		scroll_load_not_finish = 0;
	Page::OnNavigatedTo(e);
}
void SearchVocPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	SearchVocPage_Navigate_Obj = input_voc->Text;
	Page::OnNavigatedFrom(e);
}

void SearchVocPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)(((StackPanel^)(e->ClickedItem))->Children->GetAt(0)))->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}

void SearchVocPage::UpdateVocList_sync(Platform::Object^ sender, ItemClickEventArgs^ e) {
	if (!running_target) {
		running_target = 1;
		Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
		auto timerDelegate = [tempdispatchertime,this](Object^ e, Object^ ags) {
			UpdateVocList();
			tempdispatchertime->Stop();
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();
	}
}
void SearchVocPage::UpdateVocList()
{
	while (target != input_voc->Text->Data()) {
		target = input_voc->Text->Data();
		wstring q = input_voc->Text->Data();
		vector<wstring> ve;
		match(q + L"*", ve);
		VocList->Items->Clear();
		int cnt = 0;
		for (auto x : ve) {
			auto stp = ref new StackPanel();
			stp->Orientation = Orientation::Horizontal;
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(x.c_str());
			stp->Children->Append(tmp);
			tmp = ref new TextBlock();
			int len = (int)x.length();
			wstring _exp = GetExp(words[x]).f;
			_exp = trim(_exp);
			tmp->Text = ref new String(_exp.c_str());
			tmp->Margin = Thickness(20, 0, 0, 0);
			stp->Children->Append(tmp);
			VocList->Items->Append(stp);
		}
		if (ve.size())
			scroll_load_not_finish = 1;
		else
			scroll_load_not_finish = 0;
	}
	running_target = 0;
}
void SearchVocPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter&&input_voc->Text!="") {
		Frame->Navigate(
			TypeName(SingleVocPage::typeid),
			input_voc->Text,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}
void SearchVocPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
	if(scro->VerticalOffset >= scro->ScrollableHeight -100 && scroll_load_not_finish){
		wstring q = input_voc->Text->Data();
		vector<wstring> ve;
		match(q + L"*", ve, ((TextBlock^)(((StackPanel^)(VocList->Items->GetAt(VocList->Items->Size - 1)))->Children->GetAt(0)))->Text->Data());
		int cnt = 0;
		for (auto x : ve) {
			auto stp = ref new StackPanel();
			stp->Orientation = Orientation::Horizontal;
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(x.c_str());
			stp->Children->Append(tmp);
			tmp = ref new TextBlock();
			int len = (int)x.length();
			wstring _exp = GetExp(words[x]).f;
			_exp = trim(_exp);
			tmp->Text = ref new String(_exp.c_str());
			tmp->Margin = Thickness(20, 0, 0, 0);
			stp->Children->Append(tmp);
			VocList->Items->Append(stp);
		}
		if (ve.size())
			scroll_load_not_finish = 1;
		else
			scroll_load_not_finish = 0;
	}
	//auto x = ;
	//scro->Height
	//ShowMsg(IntToStr(scro->VerticalOffset) + L" " + IntToStr(scro->ScrollableHeight) + L" " + IntToStr(scro->ActualHeight));
}