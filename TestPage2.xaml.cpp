//
// TestPage2.xaml.cpp
// Implementation of the TestPage2 class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "TestPage2.xaml.h"
#include "Voc.h"
#include <random>
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
using namespace Microsoft::Advertising::WinRT::UI;

TestPage2::TestPage2()
{
	InitializeComponent();
}

unordered_map<int, pair<wstring, vector<wstring>>> TestPage2_Navigate_Obj;
unordered_map<int, bool> TestPage2_Lock;
void TestPage2::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto score = dynamic_cast<Array<int>^>(e->Parameter);
	if (score != nullptr) {
		ok = score[0];
		wa = score[1];
		uuid = score[2];
		//srand(score[2]);
	}
	else 
		ok = wa = 0;
	title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
	test_stp->Children->Clear();
	auto &now = TestPage2_Navigate_Obj[score[2]];
	vector<wstring> prob;
	if (now.first == L""&&!favorite.empty()) {
		/*auto param = dynamic_cast<String^>(e->Parameter);
		input_voc->Text = ref new String(param->Data());*/
		int cnt = 0;
		for (auto &x : favorite) {
			if (trim(x.s) == L"") { x.s = L"64"; }
			cnt += StrToInt(x.s);
		}
		int rd = RandomInt(cnt);
		cnt = 0;
		for (auto &x : favorite) {
			cnt += StrToInt(x.s);
			if (cnt > rd) {
				ans = x.f;
				break;
			}
		}
		vector<wstring> words_ve;
		for (auto x : words)words_ve.push_back(x.f);
		prob.push_back(ans);
		for (int i = 0; i < 5; i++) {
			for (int j = 0;; j++) {
				int p =RandomInt((int)words_ve.size());
				int dis = LevenshteinDistance(words_ve[p], ans);
				for (auto w : prob)
					if (w == words_ve[p])
						dis = 0;
				if (dis >= 1 && dis <= 4 + j / 1000) {
					prob.push_back(words_ve[p]);
					break;
				}
			}
		}
		random_shuffle(prob.begin(), prob.end());
		now.first = ans;
		now.second = prob;
	}
	else {
		ans = now.first;
		prob = now.second;
	}
	auto pob = ref new StackPanel();
	pob->Orientation = Orientation::Vertical;
	if(words.find(ans)!=words.end())
		pob=ExpStack(GetExp(words[ans]).f);
	//prob->FontSize = 10;
	test_stp->Children->Append(pob);
	auto lis = ref new ListView();
	lis->IsItemClickEnabled = true;
	lis->ItemClick += ref new Windows::UI::Xaml::Controls::ItemClickEventHandler(this, &CutTheWords::Views::TestPage2::OnItemClick);
	for (auto &x : prob) {
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(x.c_str());
		tmp->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage2::OnRightTapped);
		lis->Items->Append(tmp);
	}
	test_stp->Children->Append(lis);
	//ShowMsg(favorite[ans]);
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
	Page::OnNavigatedTo(e);
}

void CutTheWords::Views::TestPage2::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;
	Page::OnNavigatedFrom(e);
}

void CutTheWords::Views::TestPage2::OnItemClick(Platform::Object ^sender, Windows::UI::Xaml::Controls::ItemClickEventArgs ^e)
{
	auto tmp = dynamic_cast<TextBlock^>(e->ClickedItem);
	((ListView^)test_stp->Children->GetAt(1))->IsItemClickEnabled = false;
	if (tmp->Text == ref new String(ans.c_str())) {
		ok++;
		auto tb = ref new TextBlock();
		tb->Text = "正確";
		tb->Foreground = ref new SolidColorBrush(Colors::ForestGreen);
		test_stp->Children->Append(tb);
		if (!TestPage2_Lock[uuid]) {
			int x = StrToInt(favorite[ans]);
			favorite[ans] = IntToStr((int)max(1, x / 1.2));
			AppendStrToFile(ans + L"," + favorite[ans] + L"\n", L"favorite.txt");
			TestPage2_Lock[uuid] = true;
		}
	}
	else {
		wa++;
		auto tb = ref new TextBlock();
		tb->Text = "錯誤";
		tb->Foreground = ref new SolidColorBrush(Colors::OrangeRed);
		test_stp->Children->Append(tb);
		if (!TestPage2_Lock[uuid]) {
			int x = StrToInt(favorite[ans]);
			favorite[ans] = IntToStr(max(1, x + 32));
			AppendStrToFile(ans + L"," + favorite[ans] + L"\n", L"favorite.txt");
			TestPage2_Lock[uuid] = true;
		}
	}
	auto ar = ref new Array<int>(3);
	ar[0] = ok;
	ar[1] = wa;
	ar[2] = RandomInt(); 
	auto delay = ref new DispatcherTimer();
	Windows::Foundation::TimeSpan time;
	time.Duration = 10000000;
	delay->Interval = time;
	auto fr = Frame;
	auto timerDelegate = [fr,delay,ar](Object^ e, Object^ ags) {
		delay->Stop();
		fr->Navigate(
			TypeName(TestPage2::typeid),
			ar,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	};
	delay->Tick += ref new EventHandler<Object^>(timerDelegate);
	delay->Start();
	
}


void CutTheWords::Views::TestPage2::OnRightTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)sender)->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
void TestPage2::OnErrorOccurred(Object^ sender, AdErrorEventArgs^ e) {}

void TestPage2::OnAdRefreshed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args) {}