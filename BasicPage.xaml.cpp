//
// BasicPage.xaml.cpp
// Implementation of the BasicPage class
//

#include "pch.h"
#include "BasicPage.xaml.h"
#include "Voc.h"
using namespace CutTheWords::Views;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Advertising::WinRT::UI;

BasicPage::BasicPage()
{
    InitializeComponent();
	interstitialAd = ref new InterstitialAd();
	// Attach event handlers
	interstitialAd->ErrorOccurred += ref new EventHandler<AdErrorEventArgs^>(this, &BasicPage::OnErrorOccurred2);
	interstitialAd->AdReady += ref new EventHandler<Object^>(this, &BasicPage::OnAdReady);
	interstitialAd->Cancelled += ref new EventHandler<Object^>(this, &BasicPage::OnAdCancelled);
	interstitialAd->Completed += ref new EventHandler<Object^>(this, &BasicPage::OnAdCompleted);
	if (!IsTrial) {
		stp->Children->RemoveAtEnd();
		stp->Children->RemoveAtEnd();
		stp->Children->RemoveAtEnd();
		stp->Children->RemoveAtEnd();
	}
}

// This is an event handler for the ad control. It's invoked when the ad is refreshed.
void BasicPage::OnAdRefreshed(Object^ sender, RoutedEventArgs^ e)
{
	// We increment the ad count so that the message changes at every refresh.
	adCount++;
	//rootPage->NotifyUser("Advertisement #" + adCount.ToString(), NotifyType::StatusMessage);
}

void BasicPage::OnErrorOccurred(Object^ sender, AdErrorEventArgs^ e)
{
	//ShowMsg(wstring(L"An error occurred. : ") + e->ErrorMessage->Data());
}
void BasicPage::RunInterstitialAd(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Request an ad. When the ad is ready to show, the AdReady event will fire.
	// The application id and ad unit are passed in here.
	// The application id and ad unit id can be obtained from Dev Center.
	// See "Monetize with Ads" at https://msdn.microsoft.com/en-us/library/windows/apps/mt170658.aspx
	interstitialAd->RequestAd(AdType::Video, L"5c959e61-ce5a-4232-ab9a-d5b409749c32", L"316538");
	//interstitialAd->RequestAd(AdType::Video, L"d25517cb-12d4-4699-8bdc-52040c712cab", L"11389925");
}

// This is an event handler for the interstitial ad. It is triggered when the interstitial ad information has been downloaded and is ready to show.
void BasicPage::OnAdReady(Object^ sender, Object^ e)
{
	// The ad is ready to show; show it.
	interstitialAd->Show();
}

// This is an event handler for the interstitial ad. It is triggered when the interstitial ad is cancelled.
void BasicPage::OnAdCancelled(Object^ sender, Object^ e)
{
	//rootPage->NotifyUser("Ad cancelled", NotifyType::StatusMessage);
	ShowMsg(L"Ad cancelled");
}

// This is an event handler for the interstitial ad. It is triggered when the interstitial ad has completed playback.
void BasicPage::OnAdCompleted(Object^ sender, Object^ e)
{
	//rootPage->NotifyUser("Ad completed", NotifyType::StatusMessage);
}

// This is an error handler for the interstitial ad.
void BasicPage::OnErrorOccurred2(Object^ sender, AdErrorEventArgs^ e)
{
	//rootPage->NotifyUser("An error occurred. " + e->ErrorCode.ToString() + ": " + e->ErrorMessage, NotifyType::ErrorMessage);
	ShowMsg(wstring(L"An error occurred. ") + e->ErrorCode.ToString()->Data() + wstring(L": ") + e->ErrorMessage->Data());
}

void CutTheWords::Views::BasicPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ShowMsg(
		L"單字解釋的格式：\n"
		L"  [] 中填寫詞性且須寫在開頭，當遇到下組[]時會自動分隔\n"
		L"  整段解釋內均不可出現\"/\"字元\n"
		L"  可使用\"=XXX\"來代表此單字跟XXX相等\n"
		L"\n"
		L"部首解釋的格式：\n"
		L"  請以\"/\"字元分段，其餘部分隨意填寫\n"
		L"  可使用\"=OOO\"來代表此部首跟OOO相等\n"
	);
}
