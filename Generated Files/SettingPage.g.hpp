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

#include "SettingPage.xaml.h"

void ::CutTheWords::Views::SettingPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///SettingPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::CutTheWords::Views::SettingPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->set_list = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->set_list))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SettingPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SettingPage::ListView_ItemClick);
            }
            break;
        case 2:
            {
                this->host_name = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->host_name))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::CutTheWords::Views::SettingPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&SettingPage::host_name_TextChanged);
            }
            break;
        case 3:
            {
                this->title = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::CutTheWords::Views::SettingPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


