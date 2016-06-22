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

#include "SingleRootPage.xaml.h"

void ::CutTheWords::Views::SingleRootPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///SingleRootPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::CutTheWords::Views::SingleRootPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                ::Windows::UI::Xaml::Controls::Page^ element1 = safe_cast<::Windows::UI::Xaml::Controls::Page^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Page^>(element1))->KeyDown += ref new ::Windows::UI::Xaml::Input::KeyEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Input::KeyRoutedEventArgs^))&SingleRootPage::PageKeyDown);
            }
            break;
        case 2:
            {
                this->scro = safe_cast<::Windows::UI::Xaml::Controls::ScrollViewer^>(__target);
            }
            break;
        case 3:
            {
                this->FadOutEditPanel = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 4:
            {
                this->FadInEditPanel = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 5:
            {
                this->FadOutDelPanel = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 6:
            {
                this->FadInDelPanel = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 7:
            {
                this->topbar = safe_cast<::Windows::UI::Xaml::Controls::CommandBar^>(__target);
            }
            break;
        case 8:
            {
                this->title = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
            }
            break;
        case 9:
            {
                this->alias_list = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->alias_list))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleRootPage::alias_list_ItemClick);
            }
            break;
        case 10:
            {
                ::Microsoft::Advertising::WinRT::UI::AdControl^ element10 = safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(__target);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element10))->AdRefreshed += ref new ::Windows::Foundation::EventHandler<::Windows::UI::Xaml::RoutedEventArgs^>(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleRootPage::OnAdRefreshed);
                (safe_cast<::Microsoft::Advertising::WinRT::UI::AdControl^>(element10))->ErrorOccurred += ref new ::Windows::Foundation::EventHandler<::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^>(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Microsoft::Advertising::WinRT::UI::AdErrorEventArgs^))&SingleRootPage::OnErrorOccurred);
            }
            break;
        case 11:
            {
                this->VocList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->VocList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleRootPage::VocListView_ItemClick);
            }
            break;
        case 12:
            {
                this->EditPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 13:
            {
                this->DelPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 14:
            {
                this->DelList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->DelList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleRootPage::DelPanelListView_ItemClick);
            }
            break;
        case 15:
            {
                this->block = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 16:
            {
                this->EditList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->EditList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleRootPage::ListView_ItemClick);
            }
            break;
        case 17:
            {
                this->voc_root = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 18:
            {
                ::Windows::UI::Xaml::Controls::AppBarButton^ element18 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element18))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleRootPage::EditButton_Click);
            }
            break;
        case 19:
            {
                ::Windows::UI::Xaml::Controls::AppBarButton^ element19 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element19))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleRootPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleRootPage::DeleteButton_Click);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::CutTheWords::Views::SingleRootPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


