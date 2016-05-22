//
// TestPage.xaml.cpp
// Implementation of the TestPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "TestPage.xaml.h"
#include "TestPage2.xaml.h"
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


void TestPage::ExpStack(wstring s, StackPanel^ expst) {
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

unordered_map<int,Object^> TestPage_Navigate_Obj;
unordered_map<int, int> TestPage_Navigate_ok, TestPage_Navigate_wa;
void TestPage::Init() {
	vector<wstring>ve;
	for (auto x : favorite)ve.push_back(x.f);
	random_shuffle(ve.begin(), ve.end());
	ok = 0;
	wa = ve.size();
	title->Text = ref new String((L"正確:" + IntToStr(ok) + L" 錯誤:" + IntToStr(wa)).c_str());
	test_stp->Children->Clear();
	for (auto x : ve) {
		auto block = ref new StackPanel();
		block->Orientation = Orientation::Vertical;
		//auto prob = ref new TextBlock();
		//prob->Text = ref new String(trim(GetExp(words[x.f]).f).c_str());
		auto prob = ref new StackPanel();
		prob->Orientation = Orientation::Vertical;
		ExpStack(GetExp(words[x]).f, prob);
		//prob->FontSize = 10;
		block->Children->Append(prob);
		auto ans = ref new TextBox();
		ans->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &CutTheWords::Views::TestPage::OnTextChanged);
		block->Children->Append(ans);
		auto tit = ref new TextBlock();
		tit->Text = ref new String(trim(x).c_str());
		tit->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		tit->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::TestPage::OnTapped);
		block->Children->Append(tit);
		auto res = ref new TextBlock();
		res->Text = ref new String(L"錯誤");
		res->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage::OnRightTapped);
		block->Children->Append(res);
		block->Margin = Thickness(0, 15, 0, 0);
		test_stp->Children->Append(block);
	}
}

TestPage::TestPage()
{
	InitializeComponent();
	auto nvo = dynamic_cast<StackPanel^>(TestPage_Navigate_Obj[GetCurrentID()]);
	if (nvo != nullptr) {
		while (nvo->Children->Size) {
			auto tmp = (StackPanel^)nvo->Children->GetAt(0);
			nvo->Children->RemoveAt(0);
			((TextBox^)tmp->Children->GetAt(1))->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &CutTheWords::Views::TestPage::OnTextChanged);
			tmp->Children->GetAt(2)->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::TestPage::OnTapped);
			tmp->Children->GetAt(3)->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage::OnRightTapped);
			test_stp->Children->Append(tmp);
		}
		ok = TestPage_Navigate_ok[GetCurrentID()];
		wa = TestPage_Navigate_wa[GetCurrentID()];
		title->Text = ref new String((L"正確:" + IntToStr(ok) + L" 錯誤:" + IntToStr(wa)).c_str());
	}
	else {
		Init();
	}
}
void TestPage::OnNavigatedTo(NavigationEventArgs^ e)
{	
	Page::OnNavigatedTo(e);
}
void TestPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	TestPage_Navigate_Obj[GetCurrentID()] = test_stp;
	TestPage_Navigate_ok[GetCurrentID()] = ok;
	TestPage_Navigate_wa[GetCurrentID()] = wa;
	Page::OnNavigatedFrom(e);
}
void TestPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}



void CutTheWords::Views::TestPage::OnTextChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::TextChangedEventArgs ^e)
{
	for (unsigned int i = 0; i < test_stp->Children->Size; i++) {
		auto block = (StackPanel^)(test_stp->Children->GetAt(i));
		auto tb = (TextBox^)(block->Children->GetAt(1)),tb2= (TextBox^)sender;
		if (tb2 != tb)continue;
		if (((TextBlock^)(block->Children->GetAt(3)))->Text == L"正確")ok--;
		if (((TextBlock^)(block->Children->GetAt(3)))->Text == L"錯誤")wa--;
		if (((TextBlock^)(block->Children->GetAt(2)))->Text == tb2->Text) {
			((TextBlock^)(block->Children->GetAt(3)))->Text = ref new String(L"正確");
			ok++;
		}
		else {
			((TextBlock^)(block->Children->GetAt(3)))->Text = ref new String(L"錯誤");
			wa++;
		}

		title->Text = ref new String((L"正確:" + IntToStr(ok) + L" 錯誤:" + IntToStr(wa)).c_str());
		//if(block->)
	}
}


void CutTheWords::Views::TestPage::OnRightTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^e)
{
	for (unsigned int i = 0; i < test_stp->Children->Size; i++) {
		auto block = (StackPanel^)(test_stp->Children->GetAt(i));
		auto tb = (TextBlock^)(block->Children->GetAt(3)), tb2 = (TextBlock^)sender;
		if (tb2 != tb)continue;
		auto target = (TextBlock^)(block->Children->GetAt(2));
		if (target->Visibility == Windows::UI::Xaml::Visibility::Collapsed)
			target->Visibility = Windows::UI::Xaml::Visibility::Visible;
		else
			target->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		auto x=ref new String();
	}
}


void CutTheWords::Views::TestPage::fav_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto ar=ref new Array<int>(3);
	ar[0] = 0;
	ar[1] = 0;
	ar[2] = rand();
	Frame->Navigate(
		TypeName(TestPage2::typeid),
		ar,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());

}


void CutTheWords::Views::TestPage::AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Init();
}


void CutTheWords::Views::TestPage::OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)sender)->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
