//
// FindVocPage.xaml.cpp
// Implementation of the FindVocPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "FindVocPage.xaml.h"
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
FindVocPage::FindVocPage()
{
	InitializeComponent();
}

void FindVocPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{

	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)(((StackPanel^)(e->ClickedItem))->Children->GetAt(0)))->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}

bool match_running;

Object^ SearchRootPage_Navigate_Obj1;
Object^ SearchRootPage_Navigate_Obj2;
void FindVocPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto svp = dynamic_cast<String^>(SearchRootPage_Navigate_Obj1);
	if (svp != nullptr) {
		input_voc->Text = svp;
		input_voc->SelectAll();
		target = input_voc->Text->Data();
	}
	//auto svp2= dynamic_cast<ListView^>(SearchRootPage_Navigate_Obj2);
	//if (svp2 != nullptr) {
		//for (int i = 0;i < (int)(svp2->Items->Size);i++){
			//VocList->Items->Append(svp2->Items->GetAt(i));
		//}
	//}
	match_running = 0;
	scroll_load_not_finish = 0;
	Page::OnNavigatedTo(e);
}
void FindVocPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	SearchRootPage_Navigate_Obj1 = input_voc->Text;
	//SearchRootPage_Navigate_Obj2 = VocList;
	Page::OnNavigatedFrom(e);
}
void FindVocPage::UpdateVocList(Platform::Object^ sender, ItemClickEventArgs^ e)
{

}
void FindVocPage::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!match_running) {
		match_running = 1;
		ShowLoading();
		wstring q = input_voc->Text->Data();
		//vector<wstring> ve;
		//match(q, ve);
		create_task([q] {
			vector<wstring> ve;
			match(q, ve);
			return ve;
		}).then([this](vector<wstring>ve) {
			VocList->Items->Clear();
			for (auto x : ve) {
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
			}
			if (ve.size())
				scroll_load_not_finish = 1;
			else
				scroll_load_not_finish = 0;
			//ShowMsg(L"Ji");
			match_running = 0;
			HideLoading();
		}, task_continuation_context::use_current());
	}
}

void FindVocPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter)
		button_Click(sender,e);
}

void FindVocPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
	auto scro = (ScrollViewer^)sender;
	if (scro->VerticalOffset >= scro->ScrollableHeight - 200 && scroll_load_not_finish) {
		if (!match_running) {
			match_running = 1;
			wstring q = input_voc->Text->Data(), qq = ((String^)(((TextBlock^)(((StackPanel^)(VocList->Items->GetAt(VocList->Items->Size - 1)))->Children->GetAt(0)))->Text))->Data();
			//vector<wstring> ve;
			//match(q, ve);
			auto tmp = VocList->Items;
			create_task([q,qq] {
				vector<wstring> ve;
				match(q, ve,qq);
				return ve;
			}).then([this](vector<wstring>ve) {
				for (auto x : ve) {
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
				}
				if (ve.size())
					scroll_load_not_finish = 1;
				else
					scroll_load_not_finish = 0;
				//ShowMsg(L"Ji");
				match_running = 0;
			}, task_continuation_context::use_current());
		}
	}
}