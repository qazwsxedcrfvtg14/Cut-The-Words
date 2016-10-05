//
// FavoritePage.xaml.cpp
// Implementation of the FavoritePage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "FavoritePage.xaml.h"
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

FavoritePage::FavoritePage()
{
	InitializeComponent();
}
void FavoritePage::OnNavigatedTo(NavigationEventArgs^ e)
{
	/*auto param = dynamic_cast<String^>(e->Parameter);
	input_voc->Text = ref new String(param->Data());*/
	VocList->Items->Clear();
	int cnt = 0;
	for (auto x : favorite) {
		if (words.find(x.f) == words.end())continue;
		auto stp = ref new StackPanel();
		stp->Orientation = Orientation::Horizontal;
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(x.f.c_str());
		stp->Children->Append(tmp);
		tmp = ref new TextBlock();
		wstring _exp = GetExpSimple(words[x.f]);
		tmp->Text = ref new String(_exp.c_str());
		tmp->Margin = Thickness(20, 0, 0, 0);
		stp->Children->Append(tmp);
		VocList->Items->Append(stp);
		//if (cnt++ == 50)break;
	}
	Page::OnNavigatedTo(e);
}
void FavoritePage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	StackPanel^ st = dynamic_cast<StackPanel^>(e->ClickedItem);
	if (st == nullptr)return;
	TextBlock^ tb = dynamic_cast<TextBlock^>(st->Children->GetAt(0));
	if (tb == nullptr)return;
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		tb->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
