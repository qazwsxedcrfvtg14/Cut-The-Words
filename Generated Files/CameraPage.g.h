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
                ref class Slider;
                ref class Button;
                ref class CaptureElement;
                ref class Grid;
                ref class Image;
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
        partial ref class CameraPage : public ::Windows::UI::Xaml::Controls::Page, 
            public ::Windows::UI::Xaml::Markup::IComponentConnector,
            public ::Windows::UI::Xaml::Markup::IComponentConnector2
        {
        public:
            void InitializeComponent();
            virtual void Connect(int connectionId, ::Platform::Object^ target);
            virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
        
        private:
            bool _contentLoaded;
        
            private: ::Windows::UI::Xaml::Controls::Slider^ slider;
            private: ::Windows::UI::Xaml::Controls::Button^ ExtractButton;
            private: ::Windows::UI::Xaml::Controls::Button^ CameraButton;
            private: ::Windows::UI::Xaml::Controls::CaptureElement^ PreviewControl;
            private: ::Windows::UI::Xaml::Controls::Grid^ ImageGrid;
            private: ::Windows::UI::Xaml::Controls::Image^ PreviewImage;
            private: ::Windows::UI::Xaml::Controls::Grid^ TextOverlay;
            private: ::Windows::UI::Xaml::Controls::TextBlock^ title;
        };
    }
}

