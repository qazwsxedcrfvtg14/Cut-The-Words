//
// TestPage.xaml.h
// Declaration of the TestPage class
//

#pragma once

using namespace Platform;

#include "TestPage.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class TestPage sealed
		{
		public:
			TestPage();
		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			int ok = 0, wa = 0;
			void ExpStack(wstring s, StackPanel ^ expst);
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void OnTextCompositionChanged(Windows::UI::Xaml::Controls::TextBox ^sender, Windows::UI::Xaml::Controls::TextCompositionChangedEventArgs ^args);
			void OnTextChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::TextChangedEventArgs ^e);
			void OnRightTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^e);
			void fav_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		};
	}
}
