﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "BasicPage.xaml.h"

void ::CutTheWords::Views::BasicPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///BasicPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::CutTheWords::Views::BasicPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->stp = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 2:
            {
                ::Microsoft::Advertising::WinRT::UI::AdControl^ element2 = safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(__target);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element2))->AdRefreshed += ref new ::Windows::Foundation::EventHandler<::Windows::UI::Xaml::RoutedEventArgs^>(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&BasicPage::OnAdRefreshed);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element2))->ErrorOccurred += ref new ::Windows::Foundation::EventHandler<::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^>(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^))&BasicPage::OnErrorOccurred);
            }
            break;
        case 3:
            {
                ::Microsoft::Advertising::WinRT::UI::AdControl^ element3 = safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(__target);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element3))->AdRefreshed += ref new ::Windows::Foundation::EventHandler<::Windows::UI::Xaml::RoutedEventArgs^>(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&BasicPage::OnAdRefreshed);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element3))->ErrorOccurred += ref new ::Windows::Foundation::EventHandler<::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^>(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^))&BasicPage::OnErrorOccurred);
            }
            break;
        case 4:
            {
                ::Windows::UI::Xaml::Controls::Button^ element4 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element4))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&BasicPage::RunInterstitialAd);
            }
            break;
        case 5:
            {
                ::Windows::UI::Xaml::Controls::Button^ element5 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element5))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::BasicPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&BasicPage::Button_Click);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::CutTheWords::Views::BasicPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


