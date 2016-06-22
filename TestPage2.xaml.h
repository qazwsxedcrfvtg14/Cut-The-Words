//
// TestPage2.xaml.h
// Declaration of the TestPage2 class
//

#pragma once

using namespace Platform;
using namespace Microsoft::Advertising::WinRT::UI;

#include "TestPage2.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class TestPage2 sealed
		{
		public:
			TestPage2();
		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			int ok = 0, wa = 0,uuid;
			wstring ans;
			//void ExpStack(wstring s, StackPanel ^ expst);
			void OnItemClick(Platform::Object ^sender, Windows::UI::Xaml::Controls::ItemClickEventArgs ^e);
			void OnRightTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^e);
			void OnErrorOccurred(Object^ sender, AdErrorEventArgs^ e);
			void OnAdRefreshed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
		};
	}
}
