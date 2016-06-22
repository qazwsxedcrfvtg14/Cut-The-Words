//
// SingleVocPage.xaml.h
// Declaration of the SingleVocPage class
//

#pragma once

using namespace Platform;

#include "SingleVocPage.g.h"
#include "Voc.h"
namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// An empty page that can be used on its own or navigated to within a Frame.
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class SingleVocPage sealed
		{
		public:
			SingleVocPage();
			property String^ Vocabulary
			{
				String^ get();
				void set(String^);
			}
			property String^ Explanation
			{
				String^ get();
				void set(String^);
			}


		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

		private:
			String^ _voc;
			String^ _exp;
			pair<wstring, vector<int>> wds;
			bool EditPanelVis = 0;
			bool DelPanelVis = 0;
			void Init(wstring data,bool first=false);
			void EditButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void DeleteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void FavoriteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void PlayButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void ListView2_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void PageKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
			void comboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
			void OnSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
			void GoRootPage(Platform::Object ^sender);
			void OnDoubleTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs ^e);
			void OnHolding(Platform::Object ^sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs ^e);
			void OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
			void OnTapped2(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
			void VocListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void OnLostFocus(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
			void OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
			//void SingleVocPage::ExpStack();
			void upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e);
			void F5Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void alias_list_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
		};
	}
}
