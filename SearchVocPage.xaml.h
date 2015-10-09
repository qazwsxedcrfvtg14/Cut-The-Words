//
// SearchVocPage.xaml.h
// Declaration of the SearchVocPage class
//

#pragma once

using namespace Platform;

#include "SearchVocPage.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class SearchVocPage sealed
		{
		public:
			SearchVocPage();
		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void UpdateVocList(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
			void upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e);
		};
	}
}
