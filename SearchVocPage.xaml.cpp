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
	for (auto x : ve)
		VocList->Items->Append(ref new String(x.c_str()));
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
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}


void SearchVocPage::UpdateVocList(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	wstring q = input_voc->Text->Data();
	vector<wstring> ve;
	match(q + L"*", ve);
	VocList->Items->Clear();
	int cnt = 0;
	for (auto x : ve) 
		VocList->Items->Append(ref new String(x.c_str()));
	if (ve.size())
		scroll_load_not_finish = 1;
	else
		scroll_load_not_finish = 0;
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
		match(q + L"*", ve,((String^)VocList->Items->GetAt(VocList->Items->Size-1))->Data());
		int cnt = 0;
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