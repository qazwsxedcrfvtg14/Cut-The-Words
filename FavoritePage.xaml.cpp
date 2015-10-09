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
		VocList->Items->Append(ref new String(x.f.c_str()));
		//if (cnt++ == 50)break;
	}
	Page::OnNavigatedTo(e);
}
void FavoritePage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
