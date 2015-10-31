//
// BasicPage.xaml.cpp
// Implementation of the BasicPage class
//

#include "pch.h"
#include "BasicPage.xaml.h"
#include "Voc.h"
using namespace CutTheWords::Views;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

BasicPage::BasicPage()
{
    InitializeComponent();
	host_name->Text = ref new String(setting[L"website"].c_str());
}


void CutTheWords::Views::BasicPage::host_name_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	setting[L"website"] = host_name->Text->Data();
	wstring out;
	for (auto &x : setting)
		out += x.f + L"," + x.s + L"\n";
	StrToFile(out, L"setting.txt");
}
