//
// BasicPage.xaml.h
// Declaration of the BasicPage class
//

#pragma once

#include "BasicPage.g.h"

namespace CutTheWords
{
    namespace Views
    {
        /// <summary>
        /// An empty page that can be used on its own or navigated to within a Frame.
        /// </summary>
        [Windows::Foundation::Metadata::WebHostHidden]
        public ref class BasicPage sealed
        {
        public:
            BasicPage();
		private:
			int adCount = 0;
			Microsoft::Advertising::WinRT::UI::InterstitialAd^ interstitialAd;
			void RunInterstitialAd(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void OnAdReady(Platform::Object^ sender, Platform::Object^ args);
			void OnAdCancelled(Platform::Object^ sender, Platform::Object^ args);
			void OnAdCompleted(Platform::Object^ sender, Platform::Object^ args);
			void OnErrorOccurred2(Object ^ sender, Microsoft::Advertising::WinRT::UI::AdErrorEventArgs ^ e);
			void OnErrorOccurred(Object ^ sender, Microsoft::Advertising::WinRT::UI::AdErrorEventArgs ^ e);
			void OnAdRefreshed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
			void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		};
    }
}
