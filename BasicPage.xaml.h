//
// BasicPage.xaml.h
// Declaration of the BasicPage class
//

#pragma once

#include "BasicPage.g.h"

namespace CutTheWords
{
    namespace Views
    {
        /// <summary>
        /// An empty page that can be used on its own or navigated to within a Frame.
        /// </summary>
        [Windows::Foundation::Metadata::WebHostHidden]
        public ref class BasicPage sealed
        {
        public:
            BasicPage();
		private:
			void host_name_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		};
    }
}
