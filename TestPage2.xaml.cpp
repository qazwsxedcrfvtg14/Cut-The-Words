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

TestPage2::TestPage2()
{
	InitializeComponent();
}

void TestPage2::ExpStack(wstring s, StackPanel^ expst) {
	expst->Children->Clear();
	s = trim(s);
	wstring vol, pre;
	bool iv = 0;
	auto stp = ref new StackPanel();
	stp->Orientation = Orientation::Horizontal;
	for (auto x : s)
		if (!iv&&x == '[') {
			pre = trim(pre);
			if (pre != L"") {
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(trim(pre).c_str());
				//tmp->FontSize = 20;
				tmp->Margin = Thickness(10, 0, 0, 0);
				tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
				tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
				stp->Children->Append(tmp);
				expst->Children->Append(stp);
				stp = ref new StackPanel();
				stp->Orientation = Orientation::Horizontal;
				pre.clear();
			}
			iv = 1;
		}
		else if (iv&&x == ']') {
			if (pre == L"XX") {
				/*
				auto tmp = ref new Image;
				tmp->Source = ref new Media::Imaging::BitmapImage(ref new Uri(L"ms-appx:///Assets/v.jpg"));
				tmp->Margin = Thickness(10, 0, 0, 0);
				tmp->Height = 2;
				expst->Children->Append(tmp);
				*/
			}
			else {
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(trim(pre).c_str());
				//tmp->FontSize = 20;
				tmp->FontWeight = Text::FontWeights::Bold;
				tmp->Margin = Thickness(2, 1, 2, 1);
				tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
				tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
				auto bor = ref new Border();
				bor->MinWidth = 50;
				bor->Margin = Thickness(10, 0, 0, 4);
				bor->BorderBrush = ref new SolidColorBrush(Windows::UI::Colors::Gray);
				bor->BorderThickness = 2;
				bor->Child = tmp;

				stp->Children->Append(bor);
			}
			pre.clear();
			iv = 0;
		}
		else pre += x;
		if (pre != L"") {
			auto tmp = ref new TextBlock();
			tmp->Text = ref new String(trim(pre).c_str());
			//tmp->FontSize = 20;
			tmp->Margin = Thickness(10, 0, 0, 0);
			tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
			tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
			stp->Children->Append(tmp);
			expst->Children->Append(stp);
		}
}
void TestPage2::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto score = dynamic_cast<Array<int>^>(e->Parameter);
	if (score != nullptr) {
		ok = score[0];
		wa = score[1];
		srand(score[2]);
	}
	else 
		ok = wa = 0;
	/*auto param = dynamic_cast<String^>(e->Parameter);
	input_voc->Text = ref new String(param->Data());*/
	vector<wstring>ve;
	for (auto x : favorite)ve.push_back(x.f);
	title->Text = ref new String((L"正確:" + IntToStr(ok) + L" 錯誤:" + IntToStr(wa)).c_str());
	test_stp->Children->Clear();
	rand();
	auto rnd = [](int n)->int {return ((rand() << 16) + rand()) % n; };
	ans = ve[rnd(ve.size())];
	vector<wstring> words_ve;
	for (auto x : words)words_ve.push_back(x.f);
	vector<wstring> prob;
	prob.push_back(ans);
	for (int i = 0; i < 5; i++) {
		for (int j = 0;;j++) {
			int p = rnd(words_ve.size());
			int dis = LevenshteinDistance(words_ve[p], ans);
			if (dis >= 1 && dis <= 4 + j / 1000) {
				prob.push_back(words_ve[p]);
				break;
			}
		}
	}
	random_shuffle(prob.begin(),prob.end());
	
	auto pob = ref new StackPanel();
	pob->Orientation = Orientation::Vertical;
	ExpStack(GetExp(words[ans]).f, pob);
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
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
	Page::OnNavigatedTo(e);
}

void CutTheWords::Views::TestPage2::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;
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
		test_stp->Children->Append(tb);
	}
	else {
		wa++;
		auto tb = ref new TextBlock();
		tb->Text = "錯誤";
		test_stp->Children->Append(tb);
	}
	auto ar = ref new Array<int>(3);
	ar[0] = ok;
	ar[1] = wa;
	ar[2] = rand(); 
	auto delay = ref new DispatcherTimer();
	Windows::Foundation::TimeSpan time;
	time.Duration = 20000000;
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
