//
// SingleRootPage.xaml.h
// Declaration of the SingleRootPage class
//

#pragma once

using namespace Platform;

#include "SingleRootPage.g.h"
#include "Voc.h"
namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class SingleRootPage sealed
		{
		public:
			SingleRootPage();
			property String^ Vocabulary
			{
				String^ get();
				void set(String^);
			}
		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

		private:
			String^ _voc;
			wstring _exp;
			bool EditPanelVis = 0;
			bool DelPanelVis = 0;
			void EditButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void DeleteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void VocListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void PageKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		};
	}
}
