//
// MALoginPage.xaml.cpp
// Implementation of the MALoginPage class
//

#include "pch.h"
#include "MALoginPage.xaml.h"
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

MALoginPage::MALoginPage()
{
	InitializeComponent();
	//host_name->Text = ref new String(setting[L"website"].c_str());
}



void CutTheWords::Views::MALoginPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	post(L"https://app.login.kairisei-ma.jp/Auth/login.php", (wstring)L"{\"uuid\":\""+uuid->Text->Data()+L"\",\"hash_token\":\""+ hash_token->Text->Data()+L"\",\"clver\":\""+ clver->Text->Data()+L"\",\"os\":"+device_os->Text->Data()+L",\"carrier\":1,\"market\":2,\"lang\":0,\"device\":\"MIT Droid4X-WIN\",\"token\":\""+token->Text->Data()+L"\",\"os_ver\":\"Android OS 4.2.2 / API-17 (JDQ39E / eng.work.20150928.114211)\"}", [=](wstring s) {
		ShowMsg(s);
		user_data = Read_Json(s);
		key_main = user_data.child[L"sess_key"].data;
		kalisin(L"/Game/Connect");
	}, []() {});

	//system(("curl -s -d \"{\\\"uuid\\\":\\\"" + uuid + "\\\",\\\"hash_token\\\":\\\"" + hash_token + "\\\",\\\"clver\\\":\\\"" + clver + "\\\",\\\"os\\\":" + device_os + ",\\\"carrier\\\":1,\\\"market\\\":" + market + ",\\\"lang\\\":0,\\\"device\\\":\\\"MIT Droid4X-WIN\\\",\\\"token\\\":\\\"" + token + "\\\"}\" -A \"Dalvik/1.6.0 (Linux; U; Android 4.2.2; Droid4X-WIN\" https://app.login.kairisei-ma.jp/Auth/login.php > Auth").c_str());
}


