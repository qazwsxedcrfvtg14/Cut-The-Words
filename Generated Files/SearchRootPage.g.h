﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class TextBox;
                ref class ScrollViewer;
                ref class ListView;
                ref class TextBlock;
            }
        }
    }
}

namespace CutTheWords
{
    namespace Views
    {
        [::Windows::Foundation::Metadata::WebHostHidden]
        partial ref class SearchRootPage : public ::Windows::UI::Xaml::Controls::Page, 
            public ::Windows::UI::Xaml::Markup::IComponentConnector,
            public ::Windows::UI::Xaml::Markup::IComponentConnector2
        {
        public:
            void InitializeComponent();
            virtual void Connect(int connectionId, ::Platform::Object^ target);
            virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
        
        private:
            bool _contentLoaded;
        
            private: ::Windows::UI::Xaml::Controls::TextBox^ input_voc;
            private: ::Windows::UI::Xaml::Controls::ScrollViewer^ scro;
            private: ::Windows::UI::Xaml::Controls::ListView^ VocList;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ title;
        };
    }
}

