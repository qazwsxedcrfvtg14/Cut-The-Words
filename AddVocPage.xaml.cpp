//
// AddVocPage.xaml.cpp
// AddVocPage 類別的實作
//

#include "pch.h"
#include "AddVocPage.xaml.h"
#include "SingleVocPage.xaml.h"
#include "SingleRootPage.xaml.h"
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

// 空白頁項目範本已記錄在 http://go.microsoft.com/fwlink/?LinkId=234238

AddVocPage::AddVocPage()
{
	InitializeComponent();
}

void AddVocPage::ListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "新增為單字") {
		wstring a(voca->Text->Data()), b(expl->Text->Data());
		words[a] = b;
		wstring out;
		if (ok_words.find(a) != ok_words.end())
			out += L"*" + ok_words[a] + L"," + b + L"\n";
		else
			out += a + L"," + b + L"\n";
		AppendStrToFile(out,L"words_user.txt");
		Frame->Navigate(
			TypeName(SingleVocPage::typeid),
			voca->Text,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (str == "新增為字根") {
		wstring a(voca->Text->Data()), b(expl->Text->Data());
		root[a] = b;
		AppendStrToFile(a + L"," + b + L"\n", L"root_user.txt");
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			"-"+voca->Text+"-",
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (str == "新增為字首") {
		wstring a(voca->Text->Data()), b(expl->Text->Data());
		prefix[a] = b;
		AppendStrToFile(a + L"," + b + L"\n", L"prefix_user.txt");
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			voca->Text + "-",
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (str == "新增為字尾") {
		wstring a(voca->Text->Data()), b(expl->Text->Data());
		suffix[a] = b;
		AppendStrToFile(a + L"," + b + L"\n", L"suffix_user.txt");
		Frame->Navigate(
			TypeName(SingleRootPage::typeid),
			"-" + voca->Text,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else {
		throw("Error");
	}
}
