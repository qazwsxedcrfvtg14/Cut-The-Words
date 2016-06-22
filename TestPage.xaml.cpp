//
// TestPage.xaml.cpp
// Implementation of the TestPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "TestPage.xaml.h"
#include "TestPage1.xaml.h"
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



void TestPage::Init() {
	vector<wstring>ve;
	for (auto x : favorite)ve.push_back(x.f);
	random_shuffle(ve.begin(), ve.end());
	ok = 0;
	wa = (int)ve.size();
	title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
	test_stp->Children->Clear();
	while (!event_token_queue.empty())event_token_queue.pop();
	for (auto x : ve) {
		auto block = ref new StackPanel();
		block->Orientation = Orientation::Vertical;
		//auto prob = ref new TextBlock();
		//prob->Text = ref new String(trim(GetExp(words[x.f]).f).c_str());
		auto prob = ref new StackPanel();
		prob->Orientation = Orientation::Vertical;
		prob = ExpStack(GetExp(words[x]).f);
		//prob->FontSize = 10;
		block->Children->Append(prob);
		auto ans = ref new TextBox();
		event_token_queue.push(ans->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &CutTheWords::Views::TestPage::OnTextChanged));
		block->Children->Append(ans);
		auto tit = ref new TextBlock();
		tit->Text = ref new String(trim(x).c_str());
		tit->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		event_token_queue.push(tit->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::TestPage::OnTapped));
		block->Children->Append(tit);
		auto res = ref new TextBlock();
		res->Text = ref new String(L"錯誤");
		event_token_queue.push(res->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage::OnRightTapped));
		block->Children->Append(res);
		block->Margin = Thickness(0, 15, 0, 0);
		test_stp->Children->Append(block);
	}
}

TestPage::TestPage()
{
	InitializeComponent();
}
unordered_map<int, Object^> TestPage_Navigate_Obj;
unordered_map<int, int> TestPage_Navigate_ok, TestPage_Navigate_wa;
unordered_map<int, double> TestPage_Navigate_offset;
void TestPage::OnNavigatedTo(NavigationEventArgs^ e)
{	
	auto nvo = dynamic_cast<StackPanel^>(TestPage_Navigate_Obj[GetCurrentID()]);
	if (nvo != nullptr) {
		ok = TestPage_Navigate_ok[GetCurrentID()];
		wa = TestPage_Navigate_wa[GetCurrentID()];
		if (ok == 0 && wa == 0)
			Init();
		else {
			deque<StackPanel^>ve;
			while ((int)(nvo->Children->Size))
				ve.push_front((StackPanel^)nvo->Children->GetAt(nvo->Children->Size - 1)),
				nvo->Children->RemoveAtEnd();
			for (auto &x : ve) {
				event_token_queue.push(((TextBox^)x->Children->GetAt(1))->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &CutTheWords::Views::TestPage::OnTextChanged));
				event_token_queue.push(x->Children->GetAt(2)->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::TestPage::OnTapped));
				event_token_queue.push(x->Children->GetAt(3)->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage::OnRightTapped));
				test_stp->Children->Append(x);
			}
			title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
		}
	}
	else {
		Init();
	}
	scro->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CutTheWords::Views::TestPage::OnLoaded);
	Page::OnNavigatedTo(e);
}
void TestPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	for (int i = 0; i < (int)test_stp->Children->Size;i++) {
		auto x = (StackPanel^)test_stp->Children->GetAt(i);
		((TextBox^)x->Children->GetAt(1))->TextChanged -= event_token_queue.front();
		event_token_queue.pop();
		x->Children->GetAt(2)->Tapped -= event_token_queue.front();
		event_token_queue.pop();
		x->Children->GetAt(3)->RightTapped -= event_token_queue.front();
		event_token_queue.pop();
	}
	TestPage_Navigate_Obj[GetCurrentID()] = test_stp;
	TestPage_Navigate_ok[GetCurrentID()] = ok;
	TestPage_Navigate_wa[GetCurrentID()] = wa;
	TestPage_Navigate_offset[GetCurrentID()] = scro->VerticalOffset;
	Page::OnNavigatedFrom(e);
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
			((TextBlock^)(block->Children->GetAt(3)))->Foreground = ref new SolidColorBrush(Colors::ForestGreen);
			ok++;
		}
		else {
			((TextBlock^)(block->Children->GetAt(3)))->Text = ref new String(L"錯誤");
			((TextBlock^)(block->Children->GetAt(3)))->Foreground = ref new SolidColorBrush(Colors::OrangeRed);
			wa++;
		}

		title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
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
	ar[2] = RandomInt();
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


void CutTheWords::Views::TestPage::OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
	scro->ChangeView(nullptr, TestPage_Navigate_offset[GetCurrentID()], nullptr);
}


void CutTheWords::Views::TestPage::AppBarButton_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	auto ar = ref new Array<int>(3);
	ar[0] = 0;
	ar[1] = 0;
	ar[2] = RandomInt();
	Frame->Navigate(
		TypeName(TestPage1::typeid),
		ar,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
