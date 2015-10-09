//
// SingleVocPage.xaml.cpp
// Implementation of the SingleVocPage class
//

#include "pch.h"
#include "SingleVocPage.xaml.h"
#include "SingleRootPage.xaml.h"
#include "SearchVocPage.xaml.h"
#include "Voc.h"
using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace CutTheWords::Views;

SingleVocPage::SingleVocPage()
{
	InitializeComponent();
}

bool note_bool;
void SingleVocPage::OnNavigatedFrom(NavigationEventArgs^ e) {
	Symbol::Play;
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;

	Page::OnNavigatedFrom(e);
	}
void SingleVocPage::ExpStack() {
	expst->Children->Clear();
	wstring s(Explanation->Data());
	s = trim(s);
	wstring vol,pre;
	bool iv=0;
	auto stp=ref new StackPanel();
	stp->Orientation = Orientation::Horizontal;
	for (auto x : s)
		if (!iv&&x == '[') {
			pre = trim(pre);
			if (pre != L"") {
				auto tmp = ref new TextBlock();
				tmp->Text = ref new String(trim(pre).c_str());
				tmp->FontSize = 20;
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
			if(pre==L"XX"){
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
				tmp->FontSize = 20;
				tmp->FontWeight = Text::FontWeights::Bold;
				tmp->Margin = Thickness(2, 1,2,1);
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
	if (pre != L""){
		auto tmp = ref new TextBlock();
		tmp->Text = ref new String(trim(pre).c_str());
		tmp->FontSize = 20;
		tmp->Margin = Thickness(10, 0, 0, 0);
		tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
		tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
		stp->Children->Append(tmp);
		expst->Children->Append(stp);
	}
}
void SingleVocPage::Init(wstring data) {
	wds = GetExp(words[data]);
	Explanation = ref new String(wds.f.c_str());
	ExpStack();
	post(L"http://dictionary.reference.com/browse/" + data, L"", [=](wstring s) {
		//ShowMsg(s);
		//ShowMsg(ws);
		//HideLoading();
		auto be = s.find(L"http://static.sfdict.com/staticrep/dictaudio");
		if (be == std::wstring::npos) { /*ShowMsg(L"解析錯誤!(0x00000001)"),*/  return; }
		s = s.substr(be);
		auto ed = s.find(L".mp3");
		if (ed == std::wstring::npos) { /*ShowMsg(L"解析錯誤!(0x00000002)"),*/ return; }
		s = s.substr(0, ed + 4);
		while (1) {
			auto pos = s.substr(1).find(L"http://static.sfdict.com/staticrep/dictaudio");
			if (pos == wstring::npos)break;
			s = s.substr(pos + 1);
			//break;
		}
		media->Source = ref new Uri(ref new String(s.c_str()));
		play_but->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}, [=] {});
	post(L"https://www.bing.com/images/search?q=" + data, L"", [=](wstring s) {
		//ShowMsg(s);
		//ShowMsg(ws);
		//HideLoading();
		pics->Children->Clear();
		for (int i = 0;i < 4;i++){
			auto be = s.find(L".mm.bing.net/");
			if (be == std::wstring::npos&&be - 25 > 0) { /*ShowMsg(L"解析錯誤!(0x00000001)"); */ return; }
			s = s.substr(be - 25);
			be = s.find(L"http");
			if (be == std::wstring::npos) {/* ShowMsg(L"解析錯誤!(0x00000003)");  */return; }
			s = s.substr(be);
			//ShowMsg(IntToStr(s[0]) + L" " + IntToStr(s[1]) + L" " + IntToStr(s[2]) + L" " + IntToStr(s[3]) + L"$" + s);
			auto ed = s.find(L"\"");
			if (ed == std::wstring::npos) {/* ShowMsg(L"解析錯誤!(0x00000002)");*/ return; }
			//ShowMsg(s);
			auto tmp = ref new Image;
			tmp->Source = ref new Media::Imaging::BitmapImage(ref new Uri(ref new String(s.substr(0, ed).c_str())));
			s=s.substr(ed);
			tmp->Margin = Thickness(10, 0, 0, 0);
			tmp->Height = 150;
			pics->Children->Append(tmp);
		}
	}, [=] {});
	auto ve = Show(data);
	vector<ComboBox^> tp;
	vector<TextBlock^> tmp;
	voc_root->Children->Clear();
	voc_croot->Children->Clear();
	for (auto x : ve) {
		tmp.push_back(ref new TextBlock());
		tmp.back()->Text = ref new String(x.s.c_str());
		tmp.back()->Margin = 20;
		tmp.back()->FontSize = 18;
		//tmp.back()->Holding += ref new Windows::UI::Xaml::Input::HoldingEventHandler(this, &CutTheWords::Views::SingleVocPage::OnHolding);
		//tmp.back()->DoubleTapped += ref new Windows::UI::Xaml::Input::DoubleTappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnDoubleTapped);
		tmp.back()->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped);
		voc_root->Children->Append(tmp.back());
	}
	int i = 0;
	for (auto x : ve) {
		tp.push_back(ref new ComboBox());
		vector<wstring> vw = CutExp(x.f);
		for (auto &y : vw)
			tp.back()->Items->Append(ref new String(y.c_str()));
		tp.back()->Margin = 20;
		tp.back()->FontSize = 18;
		if (wds.s.size() == i)wds.s.push_back(0);
		if (wds.s[i] >= (int)tp.back()->Items->Size)wds.s[i] = tp.back()->Items->Size - 1;
		tp.back()->SelectedIndex = wds.s[i];
		tp.back()->SelectionChanged += ref new SelectionChangedEventHandler(this, &SingleVocPage::OnSelectionChanged);
		voc_croot->Children->Append(tp.back());
		i++;
	}
	if (favorite.find(Vocabulary->Data()) != favorite.end())
		fav->Icon = ref new SymbolIcon(Symbol::UnFavorite);
	Windows::UI::Xaml::DispatcherTimer^ tempdispatchertime = ref new DispatcherTimer();
	Windows::Foundation::TimeSpan time;
	time.Duration = 10000;
	tempdispatchertime->Interval = time;
	VocList->Items->Clear();
	VocList->IsItemClickEnabled = false;
	VocList->Items->Append("讀取中...");
	if (inited) {
		VocList->Items->Clear();
		for (auto &x : rt[data])
			VocList->Items->Append(ref new String(x.c_str()));
		VocList->IsItemClickEnabled = true;
	}
	else {
		auto timerDelegate = [=](Object^ e, Object^ ags) {
			if (inited) {
				VocList->Items->Clear();
				for (auto &x : rt[data])
					VocList->Items->Append(ref new String(x.c_str()));
				VocList->IsItemClickEnabled = true;
				tempdispatchertime->Stop();
			}
		};
		tempdispatchertime->Tick += ref new EventHandler<Object^>(timerDelegate);
		tempdispatchertime->Start();
	}
	auto tb = ref new TextBlock();
	if (note.find(Vocabulary->Data()) != note.end())
		tb->Text = ref new String(note[Vocabulary->Data()].c_str());
	else
		tb->Text = "備註(點擊修改)";
	tb->Margin = 20;
	tb->FontSize = 18;
	tb->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped2);
	note_view->Content = tb;
	note_bool = 0;
	//note->Content->Text
}
void SingleVocPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	play_but->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	EditPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	DelPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	EditPanelVis = 0;
	DelPanelVis = 0;
	auto param = dynamic_cast<String^>(e->Parameter);
	param=ref new String(trim(param->Data()).c_str());
	if (param != nullptr)
	{
		//std::wstring msg(param->Data());
		//const wchar_t* format = L"Clicked on %s";
		//wchar_t buffer[75];
		//swprintf_s(buffer, 75, format, msg.c_str());

		//Vocabulary = ref new String(buffer);
		wstring pa(param->Data());
		for (auto &x : pa)
			x |= 32;
		param = ref new String(pa.c_str());
		Vocabulary = param;
		if(words.find(pa)!=words.end()){
			Init(pa);
		}
		else {
			ShowLoading();
			post(L"http://cn.bing.com/dict/search?q=" + (wstring)pa, L"", [=](wstring web) {
				wstring disc,nt;
				auto betip = web.find(L"<div class=\"in_tip\">");
				if (betip != std::wstring::npos) {
					web = web.substr(betip + 20);
					auto edtip = web.find(L"</div>");
					if (edtip != std::wstring::npos){
						wstring od= web.substr(0,edtip);
						for (auto x : od)
							if (x == ',')
								nt += L"，";
							else if (x == '/')
								nt += L"／";
							else
								nt += x;
						}
					}
				while (1) {
					auto be = web.find(L"<span class=\"pos\">");
					if (be == std::wstring::npos || be + 18 >= web.size()) { break; }
					web = web.substr(be + 18);
					auto ed = web.find(L"</span><span class=\"def\"><span>");
					if (ed == std::wstring::npos || be + 31 >= web.size()) { break; }
					disc += L"[" + web.substr(0, ed) + L"]";
					web = web.substr(ed + 31);
					if (ed == std::wstring::npos) { break; }
					ed = web.find(L"</span></span>");
					wstring tmp = web.substr(0, ed), str;
					bool inhtml = 0;
					for (auto &x : tmp)
						if (x == '[')str += '(';
						else if (x == ']')str += ')';
						else if (x == '<')inhtml = 1;
						else if (x == '>')inhtml = 0;
						else if (!inhtml)str += x;
						disc += str + L" ";
				}
				while (1) {
					auto be = web.find(L"<span class=\"pos web\">");
					if (be == std::wstring::npos || be + 22 >= web.size()) { break; }
					web = web.substr(be + 22);
					auto ed = web.find(L"</span><span class=\"def\"><span>");
					if (ed == std::wstring::npos || be + 31 >= web.size()) { break; }
					disc += L"[" + web.substr(0, ed) + L"]";
					web = web.substr(ed + 31);
					if (ed == std::wstring::npos) { break; }
					ed = web.find(L"</span></span>");
					if (ed == std::wstring::npos) { break; }
					wstring tmp = web.substr(0, ed),str;
					bool inhtml = 0;
					for (auto &x : tmp)
						if (x == '[')str += '(';
						else if (x == ']')str += ')';
						else if (x == '<')inhtml = 1;
						else if (x == '>')inhtml = 0;
						else if (!inhtml)str += x;
					disc += str+L" ";
				}
				//ShowMsg(disc);
				//Explanation = ref new String(disc.c_str());
				wstring disc2;
				for (auto x : disc)
					if (x == ',')
						disc2 += L"，";
					else if (x == '/')
						disc2 += L"／";
					else
						disc2 += x;

				if (disc2 == L"") { HideLoading();ShowMsg(L"查無此字");return; }

				/*
				HttpMultipartFormDataContent^ post_data = ref new HttpMultipartFormDataContent();
				post_data->Add(ref new HttpStringContent(ref new String(disc2.c_str())), "text");
				post_data->Add(ref new HttpStringContent("s2twp.json"), "config");
				post_data->Add(ref new HttpStringContent("0"), "precise");
				post(L"http://opencc.byvoid.com/convert", post_data, [=](wstring disc_utf8) {
					HideLoading();
					wstring disc=disc2;
					if (disc_utf8[0] == '[')
						disc = utf8_decode(w2s(disc_utf8));
					words[pa] = disc;
					vocs.insert(w2s(pa));

					if (nt != L"")
						note[pa] = nt, AppendStrToFile(pa + L"," + nt + L"\n", L"note.txt");
					AppendStrToFile(pa + L"," + disc + L"\n", L"words_user.txt");
					if (voc_root == nullptr)return;
					Init(pa);
				}, [=] {
					HideLoading();
					words[pa] = disc2;
					vocs.insert(w2s(pa));
					if (nt != L"")
						note[pa] = nt, AppendStrToFile(pa + L"," + nt + L"\n", L"note.txt");
					AppendStrToFile(pa + L"," + disc2 + L"\n", L"words_user.txt");
					if (voc_root == nullptr)return;
					Init(pa);
				});*/
				HideLoading();
				disc = s2t(disc2);
				words[pa] = disc;
				vocs.insert(w2s(pa));

				if (nt != L"")
					note[pa] = nt, AppendStrToFile(pa + L"," + nt + L"\n", L"note.txt");
				AppendStrToFile(pa + L"," + disc + L"\n", L"words_user.txt");
				if (voc_root == nullptr)return;
				Init(pa);
			}, [=]{HideLoading(); ShowMsg(L"網路錯誤!");});

		}
	}
	else
	{
		Vocabulary = "#####";
	}

	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;

	Page::OnNavigatedTo(e);
}

String^ SingleVocPage::Vocabulary::get()
{
	return _voc;
}

void SingleVocPage::Vocabulary::set(String^ value)
{
	_voc = value;
}

String^ SingleVocPage::Explanation::get()
{
	return _exp;
}

void SingleVocPage::Explanation::set(String^ value)
{
	_exp = value;
}
void SingleVocPage::EditButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	if (EditPanelVis)
		FadOutEditPanel->Begin(), EditPanelVis = 0;
	else {
		wstring s = Vocabulary->Data();
		if (ok_words.find(s) != ok_words.end())
			block->Text = ref new String(ok_words[s].c_str());
		else {
			wstring ss;
			bool nf = 0;
			for (int i = 0;i < (int)voc_root->Children->Size;i++) {
				if (nf) ss += L" ";nf = 1;
				TextBlock^ tmp = dynamic_cast<TextBlock^>(voc_root->Children->GetAt(i));
				ss += tmp->Text->Data();
			}
			block->Text = ref new String(ss.c_str());
			}
		block2->Text = Explanation;
		if (DelPanelVis)
			FadOutDelPanel->Begin(), DelPanelVis = 0;
		FadInEditPanel->Begin(), EditPanelVis = 1;
		block->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnLoaded);
		block->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		block->SelectAll();
	}
}
void SingleVocPage::DeleteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	if (DelPanelVis)
		FadOutDelPanel->Begin(), DelPanelVis = 0;
	else {
		if (EditPanelVis)
			FadOutEditPanel->Begin(), EditPanelVis = 0;
		FadInDelPanel->Begin(), DelPanelVis = 1;
	}
}

void SingleVocPage::FavoriteButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	wstring s(Vocabulary->Data());
	if (favorite.find(s) == favorite.end()) {
		AppendStrToFile(s + L"\n", L"favorite.txt");
		favorite[s];
		fav->Icon= ref new SymbolIcon(Symbol::UnFavorite);
	}
	else {
		AppendStrToFile(L"$" + s + L"\n", L"favorite.txt");
		favorite.erase(s);
		fav->Icon = ref new SymbolIcon(Symbol::Favorite);
	}
}
void SingleVocPage::PlayButton_Click(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	media->Play();
}


//<ListViewItem>確定< / ListViewItem>
//<ListViewItem>取消< / ListViewItem>
void SingleVocPage::ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring a(Vocabulary->Data()), b(block->Text->Data()), c;
		int len = (int)b.length();
		for (int i = 0;i<len;i++)
			if (b[i] != ' ')
				c += b[i];
		if (c != a) {
			ShowMsg(L"格式錯誤");
			return;
		}
		ok_words[a] = b;
		wstring out;
		if (ok_words.find(a) != ok_words.end())
			out += L"*" + ok_words[a] + L"," + words[a] + L"\n";
		else
			out += a + L"," + words[a] + L"\n";
		AppendStrToFile(out, L"words_user.txt");
		auto ve = Show(a);
		vector<ComboBox^> tp;
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		voc_croot->Children->Clear();
		for (auto x : ve) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.s.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 18;
			//tmp.back()->DoubleTapped += ref new Windows::UI::Xaml::Input::DoubleTappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnDoubleTapped);
			tmp.back()->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped);
			voc_root->Children->Append(tmp.back());
		}
		int i = 0;
		for (auto x : ve) {
			tp.push_back(ref new ComboBox());
			vector<wstring> vw = CutExp(x.f);
			for (auto &y : vw)
				tp.back()->Items->Append(ref new String(y.c_str()));
			tp.back()->Margin = 20;
			tp.back()->FontSize = 18;
			if (wds.s.size() == i)wds.s.push_back(0);
			if (wds.s[i] >= (int)tp.back()->Items->Size)wds.s[i] = tp.back()->Items->Size - 1;
			tp.back()->SelectedIndex = wds.s[i];
			tp.back()->SelectionChanged += ref new SelectionChangedEventHandler(this, &SingleVocPage::OnSelectionChanged);
			voc_croot->Children->Append(tp.back());
			i++;
		}
		EditButton_Click(sender, e);
	}
	else if (str == "取消")
		EditButton_Click(sender, e);
}
void SingleVocPage::ListView2_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring a(Vocabulary->Data()), b,c(block2->Text->Data());
		for (int i = 0;c[i];i++)
			if (c[i] == ',')
				b += L"，";
			else
				b += c[i];
		Explanation = ref new String(b.c_str());
		ExpStack();
		//tiexp->Text = ref new String(b.c_str());
		wds.first = b.c_str();
		words[a] = MakeExp(wds);
		wstring out;
		if (ok_words.find(a) != ok_words.end())
			out += L"*" + ok_words[a] + L"," + words[a] + L"\n";
		else
			out += a + L"," + words[a] + L"\n";
		AppendStrToFile(out, L"words_user.txt");
		auto ve = Show(a);
		vector<ComboBox^> tp;
		vector<TextBlock^> tmp;
		voc_root->Children->Clear();
		voc_croot->Children->Clear();
		for (auto x : ve) {
			tmp.push_back(ref new TextBlock());
			tmp.back()->Text = ref new String(x.s.c_str());
			tmp.back()->Margin = 20;
			tmp.back()->FontSize = 18;
			//tmp.back()->DoubleTapped += ref new Windows::UI::Xaml::Input::DoubleTappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnDoubleTapped);
			tmp.back()->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped);
			voc_root->Children->Append(tmp.back());
		}
		int i = 0;
		for (auto x : ve) {
			tp.push_back(ref new ComboBox());
			vector<wstring> vw = CutExp(x.f);
			for (auto &y : vw)
				tp.back()->Items->Append(ref new String(y.c_str()));
			tp.back()->Margin = 20;
			tp.back()->FontSize = 18;
			if (wds.s.size() == i)wds.s.push_back(0);
			if (wds.s[i] >= (int)tp.back()->Items->Size)wds.s[i] = tp.back()->Items->Size - 1;
			tp.back()->SelectedIndex = wds.s[i];
			tp.back()->SelectionChanged += ref new SelectionChangedEventHandler(this, &SingleVocPage::OnSelectionChanged);
			voc_croot->Children->Append(tp.back());
			i++;
		}
		EditButton_Click(sender, e);
	}
	else if (str == "取消")
		EditButton_Click(sender, e);
}
/*
void SingleVocPage::ListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring a(Vocabulary->Data()), b(block->Text->Data());
		words[a] = b;
		vocs.insert(w2s(a));
		wstring out;
		if (ok_words.find(a) != ok_words.end())
			out += L"*" + ok_words[a] + L"," + b + L"\n";
		else
			out += a + L"," + b + L"\n";
		AppendStrToFile(out, L"words_user.txt");
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		Frame->Navigate(
			TypeName(SingleVocPage::typeid),
			Vocabulary,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}*/
void SingleVocPage::DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring s(Vocabulary->Data());
		words.erase(s);
		vocs.erase(w2s(s));
		AppendStrToFile(L"$"+s+L"\n", L"words_user.txt");
		DeleteButton_Click(sender, e);
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SearchVocPage::typeid),
				Vocabulary,
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());

	}
	else if (str == "取消")
		DeleteButton_Click(sender, e);

}

void SingleVocPage::PageKeyDown(Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Back&&!DelPanelVis&&!EditPanelVis&&!note_bool) {
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SearchVocPage::typeid),
				Vocabulary,
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (e->Key == Windows::System::VirtualKey::Enter&&!DelPanelVis&&!EditPanelVis&&note_bool) {
			dynamic_cast<TextBox^>(note_view->Content)->IsEnabled = false;
			dynamic_cast<TextBox^>(note_view->Content)->IsEnabled = true;
		//tivoc->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		//dynamic_cast<TextBox^>(note_view->Content)->Focus(Windows::UI::Xaml::FocusState::Unfocused);
	}/*
	else if(e->Key>= Windows::System::VirtualKey::A&&e->Key <= Windows::System::VirtualKey::Z){
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SearchVocPage::typeid),
				Vocabulary,
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());

	}*/
}
void SingleVocPage::OnSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e) {
	ComboBox^ senderComboBox = dynamic_cast<ComboBox^>(sender);
	for (int i = 0;i<(int)voc_croot->Children->Size;i++)
		if (voc_croot->Children->GetAt(i) == senderComboBox) {
			wds.s[i] = senderComboBox->SelectedIndex;
			wstring a(Vocabulary->Data());
			words[a] = MakeExp(wds);
			wstring out;
			if (ok_words.find(a) != ok_words.end())
				out += L"*" + ok_words[a] + L"," + words[a] + L"\n";
			else
				out += a + L"," + words[a] + L"\n";
			AppendStrToFile(out, L"words_user.txt");
			break;
		}
}
map<wstring, set<wstring>>pr, ro, su;



void CutTheWords::Views::SingleVocPage::GoRootPage(Platform::Object ^sender) {
	TextBlock^ senderTextBlock = dynamic_cast<TextBlock^>(sender);
	for (int i = 0;i<(int)voc_root->Children->Size;i++)
		if (voc_root->Children->GetAt(i) == senderTextBlock) {
			auto s = Show2(Vocabulary->Data())[i];
			if (s.front() != L'-'&&s.back() != L'-') {
				if (words.find(s) != words.end())
					Frame->Navigate(
						TypeName(SingleVocPage::typeid),
						ref new String(s.c_str()),
						ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
			}
			else
				Frame->Navigate(
					TypeName(SingleRootPage::typeid),
					ref new String(s.c_str()),
					ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
			return;
		}
}
void CutTheWords::Views::SingleVocPage::OnDoubleTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs ^e)
{
	GoRootPage(sender);
}
void CutTheWords::Views::SingleVocPage::OnHolding(Platform::Object ^sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs ^e)
{
	GoRootPage(sender);
}
void CutTheWords::Views::SingleVocPage::OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	GoRootPage(sender);
}

void SingleVocPage::VocListView_ItemClick(Platform::Object^ sender, ItemClickEventArgs^ e)
{
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		e->ClickedItem,
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
}


void CutTheWords::Views::SingleVocPage::OnTapped2(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	note_bool = 1;
	auto tb = ref new TextBox();
	if (note.find(Vocabulary->Data()) != note.end())
		tb->Text = ref new String(note[Vocabulary->Data()].c_str());
	else
		tb->Text = "";
	tb->Margin = 20;
	tb->FontSize = 18;
	tb->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnLoaded);
	tb->LostFocus += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnLostFocus);
	note_view->Content = tb;
	//dynamic_cast<TextBox^>(note_view->Content)->Focus(Windows::UI::Xaml::FocusState::Pointer);
}
void CutTheWords::Views::SingleVocPage::OnLostFocus(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
	TextBox^ tmp = dynamic_cast<TextBox^>(note_view->Content);
	wstring in = tmp->Text->Data(),out,s= Vocabulary->Data();
	for (auto x : in)
		if (x == '/')out += L"／";
		else if (x == ',')out += L"，";
		else if (x == '$')out += L"＄";
		else if (x == '*')out += L"＊";
		else out += x;
	if(out!=L"")
		note[s] = out,AppendStrToFile(s+L","+out + L"\n", L"note.txt");
	auto tb = ref new TextBlock();
	if (note.find(s) != note.end())
		tb->Text = ref new String(note[s].c_str());
	else
		tb->Text = "備註(點擊修改)";
	tb->Margin = 20;
	tb->FontSize = 18;
	tb->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped2);
	note_view->Content = tb;
	note_bool = 0;
	}


void CutTheWords::Views::SingleVocPage::OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
	((TextBox^)sender)->Focus(Windows::UI::Xaml::FocusState::Programmatic);
	((TextBox^)sender)->SelectAll();
}


void SingleVocPage::upd(Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e) {
	return;
}