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
	if (!IsTrial) 
		ad->Visibility= Windows::UI::Xaml::Visibility::Collapsed;
}

bool note_bool;
Windows::Foundation::EventRegistrationToken SingleVocPage_EventRegistrationToken;
void SingleVocPage::OnNavigatedFrom(NavigationEventArgs^ e) {
	Window::Current->CoreWindow->KeyDown -= SingleVocPage_EventRegistrationToken;
	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;
	Page::OnNavigatedFrom(e);
	}
void SingleVocPage::Init(wstring data,bool first) {
	tivoc->Text = ref new String(data.c_str());
	tivoc->IsTextSelectionEnabled = true;
	wds = GetExp(words[data]);
	Explanation = ref new String(wds.f.c_str());
	expst->Content = ExpStack(wds.f,20);
	if (first) {
		if(setting[L"network_sound"]!=L"false")
		get(setting[L"sound_url"] + data, [=](wstring s) {
			auto be = s.find(setting[L"sound_url2"]);
			if (be == std::wstring::npos) { /*ShowMsg(L"解析錯誤!(0x00000001)");*/  return; }
			s = s.substr(be);
			auto ed = s.find(setting[L"sound_type"]);
			if (ed == std::wstring::npos) { /*ShowMsg(L"解析錯誤!(0x00000002)");*/ return; }
			s = s.substr(0, ed + setting[L"sound_type"].length());
			while (1) {
				auto pos = s.substr(1).find(setting[L"sound_url2"]);
				if (pos == wstring::npos)break;
				s = s.substr(pos + 1);
			}
			media->Source = ref new Uri(ref new String(s.c_str()));
			if (setting[L"auto_play"] == L"On")
				media->AutoPlay = 1;
			play_but->Visibility = Windows::UI::Xaml::Visibility::Visible;

		}, [=] {});
		if (setting[L"network_picture"] != L"false")
		get(L"https://www.bing.com/images/search?q=" + data, [=](wstring s) {
			pics->Children->Clear();
			for (int i = 0; i < 4; i++) {
				auto be = s.find(L".mm.bing.net/");
				if (be == std::wstring::npos || be - 25 < 0) { /*ShowMsg(L"解析錯誤!(0x00000001)"); */ return; }
				s = s.substr(be - 25);
				be = s.find(L"http");
				if (be == std::wstring::npos) {/* ShowMsg(L"解析錯誤!(0x00000003)");  */return; }
				s = s.substr(be);
				auto ed = s.find(L"\"");
				if (ed == std::wstring::npos) {/* ShowMsg(L"解析錯誤!(0x00000002)");*/ return; }
				auto tmp = ref new Image;
				tmp->Source = ref new Media::Imaging::BitmapImage(ref new Uri(ref new String(s.substr(0, ed).c_str())));
				s = s.substr(ed);
				tmp->Margin = Thickness(10, 0, 0, 0);
				tmp->Height = 150;
				pics->Children->Append(tmp);
				//s = s.substr(ed - 1);
			}
		}, [=] {});
		if (setting[L"network_kk"] != L"false")
			get(L"http://tw.dictionary.search.yahoo.com/search?p=" + (wstring)data, [=](wstring web) {
			int len = int(web.length());
			auto beg = web.find(L">KK[");
			if (beg == wstring::npos)return;
			web = web.substr(beg);
			//ShowMsg(web);
			beg = web.find(L"[");
			if (beg == wstring::npos)return;
			web = web.substr(beg);
			beg = web.find(L"]");
			if (beg == wstring::npos)return;
			web = web.substr(0, beg + 1);
			kk->Text = ref new String(web.c_str());
			kk->IsTextSelectionEnabled = true;
		}, [] {});
		if (setting[L"network_sent"] != L"false") {
			StackPanel^ sentst = ref new StackPanel();
			sentst->Margin = Thickness(20, 0, 10, 20);
			sent->Content = sentst;
			get(L"http://cn.bing.com/dict/search?mkt=zh-cn&q=" + (wstring)data, [=](wstring web) {
				wstring str, tmp;
				while (true) {
					auto beg = web.find(L"<div class=\"sen_en\">");
					if (beg == wstring::npos)break;
					web = web.substr(beg);
					//ShowMsg(web);
					beg = web.find(L"</div>");
					if (beg == wstring::npos)break;
					tmp = web.substr(0, beg);
					bool inhtml = 0;
					for (auto &x : tmp)
						if (x == '<')inhtml = 1;
						else if (x == '>')inhtml = 0;
						else if (!inhtml) {
							if (x == '[')str += '(';
							else if (x == '\n')str += ' ';
							else if (x == ']')str += ')';
							else str += x;
						}
						str += L"\n";
						beg = web.find(L"<div class=\"sen_cn\">");
						if (beg == wstring::npos)break;
						web = web.substr(beg);
						//ShowMsg(web);
						beg = web.find(L"</div>");
						if (beg == wstring::npos)break;
						tmp = web.substr(0, beg);
						inhtml = 0;
						bool qut = 0;
						wstring cn = L"";
						for (auto &x : tmp) {
							if (x == '<')inhtml = 1;
							else if (x == '>')inhtml = 0;
							else if (!inhtml) {
								if (x == '[')cn += '(';
								else if (x == '\n')cn += ' ';
								else if (x == ']')cn += ')';
								//else if (x == '“')str += L"「";
								//else if (x == '”')str += L"」";
								else cn += x;
							}
						}
						str += cn;
						str += L"\n";
				}
				str = s2t(str);
				str = html_decode(str);
				str_replace(str, L"“", L"「");
				str_replace(str, L"”", L"」");
				/*str_replace(str, L"&nbsp;", L" ");
				str_replace(str, L"&lt;", L"<");
				str_replace(str, L"&gt;", L">");
				str_replace(str, L"&amp;", L"&");
				str_replace(str, L"&quot;", L"\"");
				str_replace(str, L"&apos;", L"'");
				*/

				StackPanel^ sentst = ref new StackPanel();
				sentst->Margin = Thickness(20, 0, 10, 20);
				wstringstream stream(str);
				wstring tmp2;
				while (getline(stream, tmp)) {
					getline(stream, tmp2);
					wstringstream stream2(tmp);
					wstring tmp3;
					auto stp = ref new StackPanel();
					stp->Orientation = Orientation::Horizontal;
					while (stream2 >> tmp3) {
						auto txt = ref new TextBlock();
						txt->Text = ref new String(trim(tmp3).c_str());
						txt->FontSize = 18;
						txt->Margin = Thickness(5, 15, 0, 0);
						txt->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
						txt->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
						txt->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped3);
						stp->Children->Append(txt);
					}
					sentst->Children->Append(stp);
					stp = ref new StackPanel();
					stp->Orientation = Orientation::Horizontal;
					auto txt = ref new TextBlock();
					txt->Text = ref new String(trim(tmp2).c_str());
					txt->FontSize = 16;
					txt->Margin = Thickness(10, 0, 0, 0);
					txt->Foreground= dynamic_cast<Brush^>(Application::Current->Resources->Lookup("ApplicationSecondaryForegroundThemeBrush"));
					//Application.Current.Resources["SystemAccentColor"]
					txt->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
					txt->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
					txt->IsTextSelectionEnabled = true;
					stp->Children->Append(txt);
					sentst->Children->Append(stp);
				}
				sent->Content = sentst;
				//ShowMsg(str);
				//kk->Text = ref new String(web.c_str());
			}, [] {});
		}
	}
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
	wstring _voc = data,_exp=GetExpSimple(words[_voc]);
	create_task([=] {
		vector<wstring>ve;
		for (auto x : words)
			if (x.first != _voc&&GetExpSimple(x.s) == _exp)
				ve.push_back(x.f);
		return ve;
	}).then([this](vector<wstring>ve) {
		Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this,ve]()
		{
			alias_list->Items->Clear();
			for(auto &x:ve)
				alias_list->Items->Append(ref new String(x.c_str()));
		}));
	});
	if (first) {
		VocList->Items->Clear();
		VocList->IsItemClickEnabled = false;
		VocList->Items->Append("讀取中...");
		create_task([=] {
			vector<wstring> ve;
			if (data.length() == 1 || data.find(' ') != wstring::npos)
				return ve;
			wstring data2;
			for (wchar_t c : data) {
				if (c >= 'a'&&c <= L'z')
					data2 += wstring(L"[") + c + wchar_t(c - 'a' + 'A') + wstring(L"]");
				else if (c >= 'A'&&c <= L'Z')
					data2 += wstring(L"[") + c + wchar_t(c - 'A' + 'a') + wstring(L"]");
				else
					data2 += c;
			}
			wstring reg_string;
			int len = (int)data.length();
			int len2 = (int)data2.length();
			if (len > 2 && (data.back() == 'e' || data.back() == 'E'))
				reg_string = L".*" + data2.substr(0, len2 - 4) + L".*";
			else if (len > 2 && (data.back() == 'y' || data.back() == 'Y'))
				reg_string = L".*" + data2.substr(0, len2 - 4) + L"[iIyY].*";
			else
				reg_string = L".*" + data2 + L".*";
			wregex reg(reg_string);
			int cnt = 0;
			for (auto &x : words) {
				if (!regex_match(x.f, reg) || x.f == data)continue;
				auto s2 = Show2(x.f);
				for (auto &y : s2) {
					if (y.front() == '-' || y.back() == '-')continue;
					if (!_wcsicmp(WordRotToExp(y).s.c_str(), data.c_str())) {
						ve.push_back(x.f);
						break;
					}
				}
			}
			return ve;
		}).then([this](vector<wstring>ve) {
			Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, ve]()
			{
				VocList->Items->Clear();
				VocList->IsItemClickEnabled = true;
				for (auto &x : ve) {
					auto stp = ref new StackPanel();
					stp->Orientation = Orientation::Horizontal;
					auto tmp = ref new TextBlock();
					tmp->Text = ref new String(x.c_str());
					stp->Children->Append(tmp);
					tmp = ref new TextBlock();
					wstring _exp = GetExpSimple(words[x]);
					_exp = trim(_exp);
					tmp->Text = ref new String(_exp.c_str());
					tmp->Margin = Thickness(20, 0, 0, 0);
					stp->Children->Append(tmp);
					VocList->Items->Append(stp);
				}
			}));
		});
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
}
void SingleVocPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	SingleVocPage_EventRegistrationToken = (Window::Current->CoreWindow->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(this, &CutTheWords::Views::SingleVocPage::OnKeyDown));
	play_but->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	EditPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	DelPanel->Visibility::set(Windows::UI::Xaml::Visibility::Collapsed);
	EditPanelVis = 0;
	DelPanelVis = 0;
	auto param = dynamic_cast<String^>(e->Parameter);
	param=ref new String(trim(param->Data()).c_str());
	if (param != nullptr)
	{
		wstring pa(param->Data());
		//transform(pa.begin(), pa.end(), pa.begin(), tolower);
		/*
		for (auto &x : pa)
			if(x>='A'||x<='Z')
				x |= 32;
		*/
		if(words.find(pa)!=words.end()&&words[pa]!=L""){
			pa = words.find(pa)->first;
			param = ref new String(pa.c_str());
			Vocabulary = param;
			Init(pa,true);
		}
		else {
			param = ref new String(pa.c_str());
			Vocabulary = param;
			ShowLoading();
			get(L"http://cn.bing.com/dict/search?mkt=zh-cn&q=" + (wstring)pa, [=](wstring web) {
				wstring npa = pa;
				auto pb = web.find(L"<h1><strong>");
				if (pb != wstring::npos) {
					auto tit = web.substr(pb+12);
					auto pe = tit.find(L"<");
					if (pe != wstring::npos) {
						npa = tit.substr(0, pe);
						if (_wcsicmp(npa.c_str(), pa.c_str()) != 0)npa = pa;
						Vocabulary = ref new String(npa.c_str());
					}
				}

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
					if (ed == std::wstring::npos || ed + 31 >= web.size()) { break; }
					disc += L"[" + web.substr(0, ed) + L"]";
					web = web.substr(ed + 31);
					if (ed == std::wstring::npos) { break; }
					ed = web.find(L"</span></");
					if (ed == std::wstring::npos) { break; }
					wstring tmp = web.substr(0, ed), str;
					bool inhtml = 0;
					for (auto &x : tmp)
						if (x == '<')inhtml = 1;
						else if (x == '>')inhtml = 0;
						else if (!inhtml) {
							if (x == '[')str += '(';
							else if (x == '\n')str += ' ';
							else if (x == ']')str += ')';
							else str += x;
						}
					disc += str + L" ";
				}
				while (1) {
					auto be = web.find(L"<span class=\"pos web\">");
					if (be == std::wstring::npos || be + 22 >= web.size()) { break; }
					web = web.substr(be + 22);
					auto ed = web.find(L"</span><span class=\"def\"><");
					if (ed == std::wstring::npos || ed + 25 >= web.size()) { break; }
					disc += L"[" + web.substr(0, ed) + L"]";
					web = web.substr(ed + 25);
					if (ed == std::wstring::npos) { break; }
					ed = web.find(L"</span></");
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
				HideLoading();
				nt = html_decode(nt);
				disc = html_decode(disc);
				disc = s2t(disc2);
				disc = html_decode(disc);
				words[npa] = disc;
				if (nt != L"")
					note[npa] = s2t(nt), AppendStrToFile(npa + L"," + nt + L"\n", L"note_user.txt");
				AppendStrToFile(npa + L"," + disc + L"\n", L"words_user.txt");
				if (voc_root == nullptr)return;
				Init(npa,true);
			}, [=]{HideLoading(); ShowMsg(L"網路錯誤!");});

		}
	}
	else
	{
		Vocabulary = "#####";
	}

	//SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;

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
		block2->Text = ref new String(GetExpSimpleOrg(words[s]).c_str());
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
		wds.first = b.c_str();
		words[a] = MakeExp(wds);
		b = GetExpSimple(words[a]);
		Explanation = ref new String(b.c_str());
		expst->Content = ExpStack(b,20);
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
		wstring _voc = a, _exp = GetExpSimple(words[_voc]);
		alias_list->Items->Clear();
		for (auto x : words)
			if (x.first != _voc&&GetExpSimple(x.s) == _exp)
				alias_list->Items->Append(ref new String(x.first.c_str()));
		EditButton_Click(sender, e);
	}
	else if (str == "取消")
		EditButton_Click(sender, e);
}
void SingleVocPage::DelPanelListView_ItemClick(Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e) {
	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str == "確定") {
		wstring s(Vocabulary->Data());
		words.erase(s);
		if (ok_words.find(s) != ok_words.end())
			ok_words.erase(s);
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

void SingleVocPage::OnSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e) {
	ComboBox^ senderComboBox = dynamic_cast<ComboBox^>(sender);
	for (int i = 0;i<(int)voc_croot->Children->Size;i++)
		if (voc_croot->Children->GetAt(i) == senderComboBox) {
			if ((int)wds.s.size() < i + 1)wds.s.resize(i + 1);
			wds.s[i] = senderComboBox->SelectedIndex;
			wstring a(Vocabulary->Data());
			words[a] = MakeExp(make_pair(GetExpSimpleOrg(words[a]),wds.s));
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
				s = WordRotToExp(s).s;
				if (words.find(s) != words.end())
					Frame->Navigate(
						TypeName(SingleVocPage::typeid),
						ref new String(s.c_str()),
						ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
			}
			else {
				Frame->Navigate(
					TypeName(SingleRootPage::typeid),
					ref new String(s.c_str()),
					ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
			}
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
		((TextBlock^)(((StackPanel^)(e->ClickedItem))->Children->GetAt(0)))->Text,
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
void CutTheWords::Views::SingleVocPage::OnTapped3(Platform::Object ^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^ e)
{
	TextBlock^ tmp = dynamic_cast<TextBlock^>(sender);
	wstring s = tmp->Text->Data();
	if (s.empty())return;
	s.erase(0, s.find_first_of(L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"));
	if (s.empty())return;
	s.erase(s.find_last_of(L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM")+1);
	if (s.empty())return;
	Frame->Navigate(
		TypeName(SingleVocPage::typeid),
		ref new String(s.c_str()),
		ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	//ShowMsg(tmp->Text->Data());
	//dynamic_cast<TextBox^>(note_view->Content)->Focus(Windows::UI::Xaml::FocusState::Pointer);
}
void CutTheWords::Views::SingleVocPage::OnLostFocus(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
	TextBox^ tmp = dynamic_cast<TextBox^>(note_view->Content);
	if (tmp == nullptr)return;
	wstring in = tmp->Text->Data(),out,s= Vocabulary->Data();
	for (auto x : in)
		if (x == '/')out += L"／";
		else if (x == ',')out += L"，";
		else if (x == '$')out += L"＄";
		else if (x == '*')out += L"＊";
		else out += x;
	if(out!=note[s])
		note[s] = out,AppendStrToFile(s+L","+out + L"\n", L"note_user.txt");
	else if(out==L"" && note.find(s)!=note.end())
		note.erase(s), AppendStrToFile(L"$" + s +  L"\n", L"note_user.txt");
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

void CutTheWords::Views::SingleVocPage::F5Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ShowLoading();
	wstring pa = Vocabulary->Data();
	get(L"http://cn.bing.com/dict/search?mkt=zh-cn&q=" + (wstring)pa, [=](wstring web) {
		wstring npa = pa;
		auto pb = web.find(L"<h1><strong>");
		if (pb != wstring::npos) {
			auto tit = web.substr(pb + 12);
			auto pe = tit.find(L"<");
			if (pe != wstring::npos) {
				npa = tit.substr(0, pe);
				if (_wcsicmp(npa.c_str(), pa.c_str()) != 0)npa = pa;
				if (npa != pa) {
					AppendStrToFile(L"$" + pa + L"\n", L"note_user.txt");
					AppendStrToFile(L"$" + pa + L"\n", L"words_user.txt");
					note.erase(pa);
					words.erase(pa);
				}
				Vocabulary = ref new String(npa.c_str());
			}
		}
		wstring disc, nt;
		auto betip = web.find(L"<div class=\"in_tip\">");
		if (betip != std::wstring::npos) {
			web = web.substr(betip + 20);
			auto edtip = web.find(L"</div>");
			if (edtip != std::wstring::npos) {
				wstring od = web.substr(0, edtip);
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
			if (ed == std::wstring::npos || ed + 31 >= web.size()) { break; }
			disc += L"[" + web.substr(0, ed) + L"]";
			web = web.substr(ed + 31);
			if (ed == std::wstring::npos) { break; }
			ed = web.find(L"</span></");
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
			auto ed = web.find(L"</span><span class=\"def\"><");
			if (ed == std::wstring::npos || ed + 25 >= web.size()) { break; }
			disc += L"[" + web.substr(0, ed) + L"]";
			web = web.substr(ed + 25);
			if (ed == std::wstring::npos) { break; }
			ed = web.find(L"</span></");
			if (ed == std::wstring::npos) { break; }
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

			if (disc2 == L"") { HideLoading(); ShowMsg(L"查無此字"); return; }
			HideLoading();
			nt = html_decode(nt);
			disc = html_decode(disc);
			disc = s2t(disc2);
			disc = html_decode(disc);
			words[npa] = disc;
			if (nt != L"")
				note[npa] = s2t(nt), AppendStrToFile(npa + L"," + nt + L"\n", L"note_user.txt");
			AppendStrToFile(npa + L"," + disc + L"\n", L"words_user.txt");
			if (voc_root == nullptr)return;
			Init(npa);
			return;
			wds = GetExp(words[npa]);
			Explanation = ref new String(wds.f.c_str());
			expst->Content = ExpStack(wds.f,20);
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
	}, [=] {HideLoading(); ShowMsg(L"網路錯誤!"); });

}


void CutTheWords::Views::SingleVocPage::alias_list_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{

	auto str = dynamic_cast<String^>(e->ClickedItem);
	if (str != nullptr) {
		Frame->Navigate(
			TypeName(SingleVocPage::typeid),
			str,
			ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
}


void CutTheWords::Views::SingleVocPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	StackPanel^ sentst = ref new StackPanel();
	sentst->Margin = Thickness(20, 0, 10, 20);
	sent->Content = sentst;
	get(L"http://cn.bing.com/dict/search?mkt=zh-cn&q=" + (wstring)tivoc->Text->Data(), [=](wstring web) {
		wstring str, tmp;
		while (true) {
			auto beg = web.find(L"<div class=\"sen_en\">");
			if (beg == wstring::npos)break;
			web = web.substr(beg);
			//ShowMsg(web);
			beg = web.find(L"</div>");
			if (beg == wstring::npos)break;
			tmp = web.substr(0, beg);
			bool inhtml = 0;
			for (auto &x : tmp)
				if (x == '<')inhtml = 1;
				else if (x == '>')inhtml = 0;
				else if (!inhtml) {
					if (x == '[')str += '(';
					else if (x == '\n')str += ' ';
					else if (x == ']')str += ')';
					else str += x;
				}
				str += L"\n";
				beg = web.find(L"<div class=\"sen_cn\">");
				if (beg == wstring::npos)break;
				web = web.substr(beg);
				//ShowMsg(web);
				beg = web.find(L"</div>");
				if (beg == wstring::npos)break;
				tmp = web.substr(0, beg);
				inhtml = 0;
				bool qut = 0;
				wstring cn = L"";
				for (auto &x : tmp) {
					if (x == '<')inhtml = 1;
					else if (x == '>')inhtml = 0;
					else if (!inhtml) {
						if (x == '[')cn += '(';
						else if (x == '\n')cn += ' ';
						else if (x == ']')cn += ')';
						//else if (x == '“')str += L"「";
						//else if (x == '”')str += L"」";
						else cn += x;
					}
				}
				//str += s2t(cn);
				str += L"\n";
		}
		str = s2t(str);
		str = html_decode(str);
		str_replace(str, L"“", L"「");
		str_replace(str, L"”", L"」");
		/*str_replace(str, L"&nbsp;", L" ");
		str_replace(str, L"&lt;", L"<");
		str_replace(str, L"&gt;", L">");
		str_replace(str, L"&amp;", L"&");
		str_replace(str, L"&quot;", L"\"");
		str_replace(str, L"&apos;", L"'");*/


		StackPanel^ sentst = ref new StackPanel();
		sentst->Margin = Thickness(20, 0, 10, 20);
		wstringstream stream(str);
		wstring tmp2;
		while (getline(stream, tmp)) {
			getline(stream, tmp2);
			wstringstream stream2(tmp);
			wstring tmp3;
			auto stp = ref new StackPanel();
			stp->Orientation = Orientation::Horizontal;
			while (stream2 >> tmp3) {
				auto txt = ref new TextBlock();
				txt->Text = ref new String(trim(tmp3).c_str());
				txt->FontSize = 18;
				txt->Margin = Thickness(5, 15, 0, 0);
				txt->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
				txt->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
				txt->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::SingleVocPage::OnTapped3);
				stp->Children->Append(txt);
			}
			sentst->Children->Append(stp);
			stp = ref new StackPanel();
			stp->Orientation = Orientation::Horizontal;
			auto txt = ref new TextBlock();
			txt->Text = ref new String(trim(tmp2).c_str());
			txt->FontSize = 18;
			txt->Margin = Thickness(10, 0, 0, 0);
			txt->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
			txt->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
			txt->IsTextSelectionEnabled = true;
			stp->Children->Append(txt);
			sentst->Children->Append(stp);
		}
		sent->Content = sentst; 
	}, [] {});
}


void CutTheWords::Views::SingleVocPage::OnKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^e)
{
	if (e->VirtualKey == Windows::System::VirtualKey::Back && !DelPanelVis && !EditPanelVis && !note_bool) {
		if (Frame != nullptr && Frame->CanGoBack)
			Frame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		else
			Frame->Navigate(
				TypeName(SearchVocPage::typeid),
				Vocabulary,
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::Enter && !DelPanelVis && !EditPanelVis&&note_bool) {
		dynamic_cast<TextBox^>(note_view->Content)->IsEnabled = false;
		dynamic_cast<TextBox^>(note_view->Content)->IsEnabled = true;
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::F5 && !DelPanelVis && !EditPanelVis && !note_bool) {
		F5Button_Click(nullptr, nullptr);
	}
}
