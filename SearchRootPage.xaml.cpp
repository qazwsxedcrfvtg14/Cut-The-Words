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
	for (auto x : ve) 
		VocList->Items->Append(ref new String(x.c_str()));
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
	Frame->Navigate(
		TypeName(SingleRootPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}


void SearchRootPage::UpdateVocList(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	wstring q = input_voc->Text->Data();
	vector<wstring> ve;
	match_rot(q + L"*", ve);
	VocList->Items->Clear();
	for (auto x : ve)
		VocList->Items->Append(ref new String(x.c_str()));
	if (ve.size())
		scroll_load_not_finish = 1;
	else
		scroll_load_not_finish = 0;
}
void SearchRootPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter&&VocList->Items->Size) {
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			VocList->Items->GetAt(0),
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}
void SearchRootPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
	return;
	if (scro->VerticalOffset >= scro->ScrollableHeight - 100 && scroll_load_not_finish) {

		wstring q = input_voc->Text->Data();
		vector<wstring> ve;
		match_rot(q + L"*", ve, ((String^)VocList->Items->GetAt(VocList->Items->Size - 1))->Data());
		for (auto x : ve)
			VocList->Items->Append(ref new String(x.c_str()));
		if (ve.size())
			scroll_load_not_finish = 1;
		else
			scroll_load_not_finish = 0;
	}
	//auto x = ;
	//scro->Height
	//ShowMsg(IntToStr(scro->VerticalOffset) + L" " + IntToStr(scro->ScrollableHeight) + L" " + IntToStr(scro->ActualHeight));
}