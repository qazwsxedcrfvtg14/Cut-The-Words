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

unordered_map<int, Object^> SearchRootPage_Navigate_Obj;
void SearchRootPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto svp = dynamic_cast<String^>(SearchRootPage_Navigate_Obj[GetCurrentID()]);
	if (svp != nullptr) {
		input_voc->Text = svp;
		input_voc->SelectAll();
	}
	target = L"#";
	tempdispatchertime_upd_sync = ref new DispatcherTimer();
	Windows::Foundation::TimeSpan time;
	time.Duration = 2000000;
	tempdispatchertime_upd_sync->Interval = time;
	auto timerDelegate = [this](Object^ e, Object^ ags) {
		if (target != input_voc->Text->Data()) {
			target = input_voc->Text->Data();
			wstring q = input_voc->Text->Data();
			vector<wstring> ve;
			match_rot(q + L"*", ve);
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
				wstring _exp=GetRootExp(x);
				_exp = trim(_exp);
				tmp->Text = ref new String(_exp.c_str());
				tmp->Margin = Thickness(20, 0, 0, 0);
				stp->Children->Append(tmp);
				VocList->Items->Append(stp);
				if (++cnt == 30)break;
			}
			if (ve.size())
				scroll_load_not_finish[GetCurrentID()] = 1;
			else
				scroll_load_not_finish[GetCurrentID()] = 0;
		}
		//Wait to do
		//if (scro->VerticalOffset >= scro->ScrollableHeight - 200 && scroll_load_not_finish[GetCurrentID()]);
	};
	tempdispatchertime_upd_sync->Tick += ref new EventHandler<Object^>(timerDelegate);
	tempdispatchertime_upd_sync->Start();
	Page::OnNavigatedTo(e);
}
void SearchRootPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	tempdispatchertime_upd_sync->Stop();
	SearchRootPage_Navigate_Obj[GetCurrentID()] = input_voc->Text;
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
void SearchRootPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter&&VocList->Items->Size) {
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			((TextBlock^)(((StackPanel^)(VocList->Items->GetAt(0)))->Children->GetAt(0)))->Text,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}