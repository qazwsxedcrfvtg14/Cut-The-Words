//
// MAPage.xaml.cpp
// Implementation of the MAPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "MAPage.xaml.h"
#include "Voc.h"

using namespace CutTheWords::Views;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
//using namespace Jtol;
//map<string, string> ok_words, nil;
//map<string, string> words, prefix, suffix, root;
MAPage::MAPage()
{
	InitializeComponent();
}
Json_Node solo_data;
void MAPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	kalisin(L"/Game/HomeShow", L"", 1, [=](wstring s) {
		auto home = Read_Json(s).child[L"user"];
		stp->Children->Clear();
		
		auto fun = [=](wstring s) {
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(s.c_str());
			//tmp->FontSize = 20;
			tmp->Margin = 5;
			stp->Children->Append(tmp);
		};
		fun(L"UserID: " + home.child[L"userid"].data);
		fun(L"Name: " + home.child[L"name"].data);
		fun(L"Level: " + home.child[L"level"].data + L"(NextExp:" + home.child[L"next_lv_exp"].data+L")");
		fun(L"AP: " + home.child[L"ap"].data + L"/" + home.child[L"ap_max"].data + L"(" + home.child[L"ap_next_sec"].data + L".." + home.child[L"ap_heal_sec"].data + L")");
		fun(L"BP: " + home.child[L"bp"].data + L"/" + home.child[L"bp_max"].data + L"(" + home.child[L"bp_next_sec"].data + L".." + home.child[L"bp_heal_sec"].data + L")");
		fun(L"Card: " + home.child[L"card_num"].data + L"/" + home.child[L"card_max"].data);
		fun(L"Gold: " + home.child[L"gold"].data);
		//fun(L"Friend Point: " + home.child[L"fp"].data);
		kalisin(L"/Game/TeamBattleSoloShow", L"", 1, [=](wstring s) {
			auto fun = [=](wstring s) {
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(s.c_str());
				//tmp->FontSize = 20;
				tmp->Margin = 5;
				stp2->Children->Append(tmp);
			};
		wstring out;
		solo_data = Read_Json(s);
		function<void(Json_Node&, int)> f = [&](Json_Node& now, int l) {
			if (now.type == 0)out += now.data;
			else if (now.type == 1) {
				if (now.child[L"appear_end"].data == L"-1")return;
				out += L"{\n";
				for (auto &x : now.child) {
					for (int i = 0;i <= l;i++)
						out += L"  ";
					out += x.f + L": ";
					f(x.s, l + 1);
					out += L"\n";
				}
				for (int i = 0;i < l;i++)
					out += L"  ";
				out += L"}\n";
			}
			else if (now.type == 2) {
				out += L"[\n";
				for (auto &x : now.ary) {
					for (int i = 0;i <= l;i++)
						out += L"  ";
					f(x,l + 1);
					out += L"\n";
				}
				for (int i = 0;i < l;i++)
					out += L"  ";
				out += L"]\n";
			}
		};
		f(solo_data.child[L"event_groups"], 0);
		fun(out);
		});
	});
	Page::OnNavigatedTo(e);
}
void MAPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	Page::OnNavigatedFrom(e);
}

void MAPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
}
void MAPage::UpdateVocList(Platform::Object^ sender, ItemClickEventArgs^ e)
{
}
void MAPage::TextBoxKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
}
void MAPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
}

void CutTheWords::Views::MAPage::button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto uid0 = solo_data.child[L"arthurs"].ary[0].child[L"partners"].ary[0].child[L"userid"].data;
	auto uid1 = solo_data.child[L"arthurs"].ary[1].child[L"partners"].ary[0].child[L"userid"].data;
	auto uid2 = solo_data.child[L"arthurs"].ary[2].child[L"partners"].ary[0].child[L"userid"].data;
	kalisin(L"/Game/TeamBattleSoloStart", L"{\"bossid\":" + wstring(tb->Text->Data()) + L",\"deck_arthur_type\":4,\"deck_arthur_type_idx\":0,\"partner_deck_selects\":[{\"userid\":" + uid0 + L",\"arthur_type\":1,\"deck_idx\":0},{\"userid\":" + uid1 + L",\"arthur_type\":2,\"deck_idx\":0},{\"userid\":" + uid2 + L",\"arthur_type\":3,\"deck_idx\":0}]}", 1, [](wstring s) {ShowMsg(L"OK");});
}
void CutTheWords::Views::MAPage::button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	kalisin(L"/Game/TeamBattleSoloEnd", L"{\"progress\":1,\"is_clear\":1,\"input_cmd\":[\"\"],\"enemy_dead_bit\":[7]}", 1, [](wstring s) {ShowMsg(L"OK");});
}
void CutTheWords::Views::MAPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	kalisin(L"/Game/ExploreStart", L"{\"arthur_type\":4,\"deck_idx\":0}", 1, [](wstring s) {ShowMsg(L"Ok");});
}
void CutTheWords::Views::MAPage::Button_Click_3(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	kalisin(L"/Game/ExploreEnd", L"", 1, [](wstring s) {ShowMsg(L"Ok");});
}
