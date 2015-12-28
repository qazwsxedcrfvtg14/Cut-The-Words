//
// SearchRootPage.xaml.cpp
// Implementation of the SearchVocPage class
//

#include "pch.h"
#include "SingleRootPage.xaml.h"
#include "SearchRootPage.xaml.h"
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
SearchRootPage::SearchRootPage()
{
	InitializeComponent();
}

Object^ SearchRootPage_Navigate_Obj;
void SearchRootPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto svp = dynamic_cast<String^>(SearchRootPage_Navigate_Obj);
	if (svp != nullptr) {
		input_voc->Text = svp;
		input_voc->SelectAll();
	}
	wstring q = input_voc->Text->Data();
	vector<wstring> ve;
	match_rot(q + L"*", ve);
	VocList->Items->Clear();
	for (auto x : ve) {
		auto stp = ref new StackPanel();
		stp->Orientation = Orientation::Horizontal;
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(x.c_str());
		stp->Children->Append(tmp);
		tmp = ref new TextBlock();
		int len = (int)x.length();
		wstring _exp;
		if (x[0] == '-'&&x[len - 1] == '-')_exp = root[x.substr(1, len - 2)];
		else if (x[0] == '-')_exp = suffix[x.substr(1)];
		else if (x[len - 1] == '-')_exp = prefix[x.substr(0, len - 1)];
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
void SearchRootPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	SearchRootPage_Navigate_Obj = input_voc->Text;
	Page::OnNavigatedFrom(e);
}
void SearchRootPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	(ref new StackPanel)->Children;
	Frame->Navigate(
		TypeName(SingleRootPage::typeid),
		((TextBlock^)(((StackPanel^)(e->ClickedItem))->Children->GetAt(0)))->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}


void SearchRootPage::UpdateVocList(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	wstring q = input_voc->Text->Data();
	vector<wstring> ve;
	match_rot(q + L"*", ve);
	VocList->Items->Clear();
	for (auto x : ve) {
		auto stp = ref new StackPanel();
		stp->Orientation = Orientation::Horizontal;
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(x.c_str());
		stp->Children->Append(tmp);
		tmp = ref new TextBlock();
		int len = (int)x.length();
		wstring _exp;
		if (x[0] == '-'&&x[len - 1] == '-')_exp = root[x.substr(1, len - 2)];
		else if (x[0] == '-')_exp = suffix[x.substr(1)];
		else if (x[len - 1] == '-')_exp = prefix[x.substr(0, len - 1)];
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
void SearchRootPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter&&VocList->Items->Size) {
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			((TextBlock^)(((StackPanel^)(VocList->Items->GetAt(0)))->Children->GetAt(0)))->Text,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}
void SearchRootPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
	return;
	if (scro->VerticalOffset >= scro->ScrollableHeight - 100 && scroll_load_not_finish) {

		wstring q = input_voc->Text->Data();
		vector<wstring> ve;
		match_rot(q + L"*", ve, ((String^)VocList->Items->GetAt(VocList->Items->Size - 1))->Data());
		for (auto x : ve){
			auto stp = ref new StackPanel();
			stp->Orientation = Orientation::Horizontal;
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(x.c_str());
			stp->Children->Append(tmp);
			tmp = ref new TextBlock();
			int len = (int)x.length();
			wstring _exp;
			if (x[0] == '-'&&x[len - 1] == '-')_exp = root[x.substr(1, len - 2)];
			else if (x[0] == '-')_exp = suffix[x.substr(1)];
			else if (x[len - 1] == '-')_exp = prefix[x.substr(0, len - 1)];
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