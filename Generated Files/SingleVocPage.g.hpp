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

#include "SingleVocPage.xaml.h"

void ::CutTheWords::Views::SingleVocPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///SingleVocPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}


/// <summary>
/// Auto generated class for compiled bindings.
/// </summary>
class CutTheWords::Views::SingleVocPage::SingleVocPage_obj1_Bindings 
    : public ::XamlBindingInfo::XamlBindingsBase<::CutTheWords::Views::SingleVocPage>
{
public:
    SingleVocPage_obj1_Bindings()
    {
    }

    void Connect(int __connectionId, ::Platform::Object^ __target)
    {
        switch(__connectionId)
        {
            case 8:
                this->obj8 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
                break;
        }
    }
private:
    // Fields for each control that has bindings.
    ::Windows::UI::Xaml::Controls::TextBlock^ obj8;

    // Update methods for each path node used in binding steps.
    void Update_(::CutTheWords::Views::SingleVocPage^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | (1 << 0))) != 0)
            {
                this->Update_Vocabulary(obj->Vocabulary, phase);
            }
        }
    }
    void Update_Vocabulary(::Platform::String^ obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED )) != 0)
        {
            ::XamlBindingInfo::XamlBindingSetters::Set_Windows_UI_Xaml_Controls_TextBlock_Text(this->obj8, obj, nullptr);
        }
    }
};

void ::CutTheWords::Views::SingleVocPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                ::Windows::UI::Xaml::Controls::Page^ element1 = safe_cast<::Windows::UI::Xaml::Controls::Page^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Page^>(element1))->KeyDown += ref new ::Windows::UI::Xaml::Input::KeyEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Input::KeyRoutedEventArgs^))&SingleVocPage::PageKeyDown);
            }
            break;
        case 2:
            {
                this->scro = safe_cast<::Windows::UI::Xaml::Controls::ScrollViewer^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ScrollViewer^>(this->scro))->ViewChanged += ref new ::Windows::Foundation::EventHandler<::Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^>(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^))&SingleVocPage::upd);
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
                this->tivoc = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
            }
            break;
        case 9:
            {
                this->play_but = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->play_but))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleVocPage::PlayButton_Click);
            }
            break;
        case 10:
            {
                this->note_view = safe_cast<::Windows::UI::Xaml::Controls::ScrollViewer^>(__target);
            }
            break;
        case 11:
            {
                this->VocList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->VocList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleVocPage::VocListView_ItemClick);
            }
            break;
        case 12:
            {
                this->EditPanel = safe_cast<::Windows::UI::Xaml::Controls::ScrollViewer^>(__target);
            }
            break;
        case 13:
            {
                this->DelPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 14:
            {
                this->media = safe_cast<::Windows::UI::Xaml::Controls::MediaElement^>(__target);
            }
            break;
        case 15:
            {
                this->DelList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->DelList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleVocPage::DelPanelListView_ItemClick);
            }
            break;
        case 16:
            {
                this->block = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 17:
            {
                this->EditList = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->EditList))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleVocPage::ListView_ItemClick);
            }
            break;
        case 18:
            {
                this->block2 = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 19:
            {
                this->EditList2 = safe_cast<::Windows::UI::Xaml::Controls::ListView^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::ListView^>(this->EditList2))->ItemClick += ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::ItemClickEventArgs^))&SingleVocPage::ListView2_ItemClick);
            }
            break;
        case 20:
            {
                this->pics = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 21:
            {
                this->voc_croot = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 22:
            {
                this->voc_root = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 23:
            {
                this->expst = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 24:
            {
                this->fav = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(this->fav))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleVocPage::FavoriteButton_Click);
            }
            break;
        case 25:
            {
                ::Windows::UI::Xaml::Controls::AppBarButton^ element25 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element25))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleVocPage::EditButton_Click);
            }
            break;
        case 26:
            {
                ::Windows::UI::Xaml::Controls::AppBarButton^ element26 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element26))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::CutTheWords::Views::SingleVocPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&SingleVocPage::DeleteButton_Click);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::CutTheWords::Views::SingleVocPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    ::XamlBindingInfo::XamlBindings^ bindings = nullptr;
    switch (__connectionId)
    {
        case 1:
            {
                ::Windows::UI::Xaml::Controls::Page^ element1 = safe_cast<::Windows::UI::Xaml::Controls::Page^>(__target);
                SingleVocPage_obj1_Bindings* objBindings = new SingleVocPage_obj1_Bindings();
                objBindings->SetDataRoot(this);
                bindings = ref new ::XamlBindingInfo::XamlBindings(objBindings);
                this->Bindings = bindings;
                element1->Loading += ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::FrameworkElement^, ::Platform::Object^>(bindings, &::XamlBindingInfo::XamlBindings::Loading);
            }
            break;
    }
    return bindings;
}


