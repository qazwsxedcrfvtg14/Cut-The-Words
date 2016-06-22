//
// TestPage1.xaml.cpp
// Implementation of the TestPage1 class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "TestPage1.xaml.h"
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

TestPage1::TestPage1()
{
	InitializeComponent();
}

unordered_map<int, wstring> TestPage1_Navigate_Obj;
unordered_map<int, bool> TestPage1_Lock;
void TestPage1::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto score = dynamic_cast<Array<int>^>(e->Parameter);
	if (score != nullptr) {
		ok = score[0];
		wa = score[1]+1;
		uuid = score[2];
		//srand(score[2]);
	}
	else
		ok = 0,wa = 1;
	title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
	test_stp->Children->Clear();
	auto &now = TestPage1_Navigate_Obj[score[2]];
	if (now == L"" && !favorite.empty()) {
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
		now = ans;
	}
	else {
		ans = now;
	}
	//ShowMsg(favorite[ans]);
	auto block = ref new StackPanel();
	block->Orientation = Orientation::Vertical;
	//auto prob = ref new TextBlock();
	//prob->Text = ref new String(trim(GetExp(words[x.f]).f).c_str());
	auto prob = ref new StackPanel();
	prob->Orientation = Orientation::Vertical;
	if(words.find(ans)!=words.end())
		prob=ExpStack(GetExp(words[ans]).f);
	//prob->FontSize = 10;
	block->Children->Append(prob);
	auto anst = ref new TextBox();
	anst->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &CutTheWords::Views::TestPage1::OnTextChanged);
	block->Children->Append(anst);
	auto tit = ref new TextBlock();
	tit->Text = ref new String(trim(ans).c_str());
	tit->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	tit->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::TestPage1::OnTapped);
	block->Children->Append(tit);
	auto res = ref new TextBlock();
	res->Text = ref new String(L"錯誤");
	res->RightTapped += ref new Windows::UI::Xaml::Input::RightTappedEventHandler(this, &CutTheWords::Views::TestPage1::OnRightTapped);
	block->Children->Append(res);
	block->Margin = Thickness(0, 15, 0, 0);
	test_stp->Children->Append(block);
	//ShowMsg(favorite[ans]);
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
	Page::OnNavigatedTo(e);
}

void CutTheWords::Views::TestPage1::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;
	Page::OnNavigatedFrom(e);
}

void CutTheWords::Views::TestPage1::OnRightTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs ^e)
{

	if (!TestPage1_Lock[uuid]) {
		int x = StrToInt(favorite[ans]);
		favorite[ans] = IntToStr(max(1, x +16));
		AppendStrToFile(ans+L","+favorite[ans] + L"\n", L"favorite.txt");
		TestPage1_Lock[uuid] = true;
	}
	for (unsigned int i = 0; i < test_stp->Children->Size; i++) {
		auto block = (StackPanel^)(test_stp->Children->GetAt(i));
		auto tb = (TextBlock^)(block->Children->GetAt(3)), tb2 = (TextBlock^)sender;
		if (tb2 != tb)continue;
		auto target = (TextBlock^)(block->Children->GetAt(2));
		if (target->Visibility == Windows::UI::Xaml::Visibility::Collapsed)
			target->Visibility = Windows::UI::Xaml::Visibility::Visible;
		else
			target->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		auto x = ref new String();
	}
}


void CutTheWords::Views::TestPage1::OnTextChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::TextChangedEventArgs ^e)
{
	for (unsigned int i = 0; i < test_stp->Children->Size; i++) {
		auto block = (StackPanel^)(test_stp->Children->GetAt(i));
		auto tb = (TextBox^)(block->Children->GetAt(1)), tb2 = (TextBox^)sender;
		if (tb2 != tb)continue;
		if (((TextBlock^)(block->Children->GetAt(3)))->Text == L"正確")ok--;
		if (((TextBlock^)(block->Children->GetAt(3)))->Text == L"錯誤")wa--;
		if (((TextBlock^)(block->Children->GetAt(2)))->Text == tb2->Text) {
			((TextBlock^)(block->Children->GetAt(3)))->Text = ref new String(L"正確");
			((TextBlock^)(block->Children->GetAt(3)))->Foreground = ref new SolidColorBrush(Colors::ForestGreen);
			ok++;
			auto block = (StackPanel^)(test_stp->Children->GetAt(0));
			auto tb = (TextBox^)(block->Children->GetAt(1));
			if (((TextBlock^)(block->Children->GetAt(2)))->Text == tb->Text && !TestPage1_Lock[uuid]) {
				int x = StrToInt(favorite[ans]);
				favorite[ans] = IntToStr(max(1, x / 2));
				AppendStrToFile(ans + L"," + favorite[ans] + L"\n", L"favorite.txt");
				TestPage1_Lock[uuid] = true;
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
			auto timerDelegate = [fr, delay, ar](Object^ e, Object^ ags) {
				delay->Stop();
				fr->Navigate(
					TypeName(TestPage1::typeid),
					ar,
					ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
			};
			delay->Tick += ref new EventHandler<Object^>(timerDelegate);
			delay->Start();
		}
		else {
			((TextBlock^)(block->Children->GetAt(3)))->Text = ref new String(L"錯誤");
			wa++;
		}

		title->Text = ref new String((L"正確：" + IntToStr(ok) + L" 錯誤：" + IntToStr(wa)).c_str());
	}
}


void CutTheWords::Views::TestPage1::OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		((TextBlock^)sender)->Text,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}


void CutTheWords::Views::TestPage1::AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	auto ar = ref new Array<int>(3);
	ar[0] = ok;
	ar[1] = wa;
	ar[2] = RandomInt();
	Frame->Navigate(
		TypeName(TestPage1::typeid),
		ar,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}
void TestPage1::OnErrorOccurred(Object^ sender, AdErrorEventArgs^ e) {}

void TestPage1::OnAdRefreshed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args) {}