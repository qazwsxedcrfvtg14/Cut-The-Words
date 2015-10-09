//
// FavoritePage.xaml.h
// Declaration of the FavoritePage class
//

#pragma once

using namespace Platform;

#include "FavoritePage.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class FavoritePage sealed
		{
		public:
			FavoritePage();
		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		private:
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
		};
	}
}
