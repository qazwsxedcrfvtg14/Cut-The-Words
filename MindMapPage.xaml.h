//
// MindMapPage.xaml.h
// Declaration of the MindMapPage class
//

#pragma once

#include "MindMapPage.g.h"
#include "Voc.h"
namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class MindMapPage sealed
		{
		public:
			MindMapPage();
		protected:
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			class Nod {
				public:
					double x;
					double y;
					bool fixed=false;
					Windows::UI::Xaml::Shapes::Ellipse^ cir;
					//TextBlock^ txt;
			};
			class Lin {
				public:
					wstring a, b;
					//Windows::UI::Xaml::Shapes::Line^ li;
			};
			double radius=8;
			int tick = 1;
			bool run=false;
			bool pause = false;
			mutex nods_lock;
			map<wstring, Nod> nods;
			vector<Lin>lines;
			//map<wstring, set<wstring>> rt;
			map<wstring,wstring>mp;
			DispatcherTimer^ tempdispatchertime_upd_sync;
			bool notscroll = false;
			Windows::Foundation::EventRegistrationToken key_dwn;
			void Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void canvas_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args);
			void canvas_Draw_1(Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedDrawEventArgs^ args);
			void canvas_CreateResources(Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedControl^ sender, Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs^ args);
			void Page_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
			void OnKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);
			void canvas_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e);
		};
	}
}
