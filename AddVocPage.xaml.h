//
// AddVocPage.xaml.h
// AddVocPage 類別的宣告
//

#pragma once

#include "AddVocPage.g.h"

namespace CutTheWords
{
	namespace Views
	{
		/// <summary>
		/// 可以在本身使用或巡覽至框架內的空白頁面。
		/// </summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class AddVocPage sealed
		{
		public:
			AddVocPage();
		private:
			void ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
		};
	}
}
