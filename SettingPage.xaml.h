//
// SettingPage.xaml.h
// SettingPage 類別的宣告
//

#pragma once

using namespace Platform;

#include "SettingPage.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// 可以在本身使用或巡覽至框架內的空白頁面。
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class SettingPage sealed
		{
		public:
			SettingPage();
		private:
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
			void host_name_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		};
	}
}
