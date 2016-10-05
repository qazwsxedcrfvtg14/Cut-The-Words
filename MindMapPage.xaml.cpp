//
// MindMapPage.xaml.cpp
// Implementation of the MindMapPage class
//

#include "pch.h"
#include "MindMapPage.xaml.h"
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
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Advertising::WinRT::UI;
using namespace Microsoft::Graphics::Canvas;
unordered_map<long long int, vector<pair<wstring, pair<double, double>>>> MindMapPage_Navigate_Obj;
struct MindMapPage_Navigate_Struct {
	int tick;
	bool pause;
	double x;
	double y;
	float scale;
	MindMapPage_Navigate_Struct() { tick = -1; }
	MindMapPage_Navigate_Struct(int t, bool p, double xx, double yy, float s) { tick = t; pause = p; x = xx; y = yy; scale = s; }
};
unordered_map<long long int, MindMapPage_Navigate_Struct> MindMapPage_Navigate_Obj2;
void MindMapPage::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	auto window = CoreWindow::GetForCurrentThread();
	window->KeyDown -= key_dwn;
	run = false;
	for (auto &x : nods)
		MindMapPage_Navigate_Obj[GetCurrentID()].push_back(make_pair(x.first,make_pair(x.second.x,x.second.y)));
	MindMapPage_Navigate_Obj2[GetCurrentID()] = MindMapPage_Navigate_Struct(tick,pause,scroll->HorizontalOffset,scroll->VerticalOffset,scroll->ZoomFactor);
}
MindMapPage::MindMapPage()
{
	InitializeComponent();
}
void MindMapPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	auto window = CoreWindow::GetForCurrentThread();
	key_dwn=(window->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(this, &CutTheWords::Views::MindMapPage::OnKeyDown));
	if (favorite.size() == 0) {
		ShowMsg(L"請先在我的最愛中加入單字!");
		return;
	}
	create_task([this] {
		if(rt.empty())
			for (auto &x : words) {
				auto ve = Show2(x.f);
				for (auto &y : ve) {
					if (y.front() != '-'&&y.back() != '-') {
						y = WordRotToExp(y).s;
						if (words.find(y) == words.end())continue;
					}
					if (y != x.f)
						rt[y].insert(x.f),
						rt[x.f].insert(y);
				}
			}
	}).then([this] {
		Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]()
		{
			vector<wstring>qu;
			//qu.push_back(L"computer");
			auto Add = [this](wstring now,double x,double y) {
				if (nods.find(now) != nods.end())return false;
				auto &nw = nods[now];
				nw.x = x;
				nw.y = y;
				auto tmp = ref new Ellipse();
				tmp->Width = radius * 3;
				tmp->Height = radius * 3;
				tmp->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
				tmp->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
				tmp->Margin = Thickness(nw.x - radius*1.5+30, nw.y - radius*1.5 + 30, 0, 0);
				tmp->Fill = ref new SolidColorBrush(Colors::Gray);
				tmp->ManipulationMode = ManipulationModes::TranslateX | ManipulationModes::TranslateY;
				tmp->ManipulationStarted += ref new ManipulationStartedEventHandler([this](Object^ sender, ManipulationStartedRoutedEventArgs^ e) {
					notscroll = true;
					auto tmp = dynamic_cast<Ellipse^>(sender);
					if (tmp == nullptr)return;
					for (auto &x : nods)
						if (x.s.cir == tmp) {
							x.s.fixed = true;
							//x.second.txt->Foreground = ref new SolidColorBrush(Colors::Green);
						}
				});
				tmp->ManipulationDelta += ref new ManipulationDeltaEventHandler([this](Object^ sender, ManipulationDeltaRoutedEventArgs^ e)
				{
					auto tmp = dynamic_cast<Ellipse^>(sender);
					if (tmp == nullptr)return;
					nods_lock.lock();
					for (auto &x : nods)
						if (x.s.cir == tmp) {
							x.s.x += e->Delta.Translation.X / scroll->ZoomFactor,
							x.s.y += e->Delta.Translation.Y / scroll->ZoomFactor;
							break;
						}
					nods_lock.unlock();
					/*if (e->Delta.Translation.X + e->Delta.Translation.Y > 3)
						tick = min(tick, 150);
					if (e->Delta.Translation.X + e->Delta.Translation.Y>5)
						tick = min(tick, 100);
					if (e->Delta.Translation.X + e->Delta.Translation.Y>10)
						tick = min(tick, 50);
					if (e->Delta.Translation.X + e->Delta.Translation.Y>20)
						tick = min(tick, 0);*/
					//nods[mp[tmp->ToString()->Data()]].x += e->Delta.Translation.X;
					//nods[mp[tmp->ToString()->Data()]].y += e->Delta.Translation.Y;
				});
				tmp->ManipulationCompleted += ref new ManipulationCompletedEventHandler([this](Object^ sender, ManipulationCompletedRoutedEventArgs^ e) {
					auto tmp = dynamic_cast<Ellipse^>(sender);
					if (tmp == nullptr)return;
					for (auto &x : nods)
						if (x.s.cir == tmp) {
							x.s.fixed = false;
							break;
						}
					notscroll = false;
				});
				tmp->Tapped += ref new TappedEventHandler([this](Object^ sender, TappedRoutedEventArgs^ e) {
					auto tmp = dynamic_cast<Ellipse^>(sender);
					if (tmp == nullptr)return;
					for (auto &x : nods) 
						if (x.second.cir == tmp)
							x.s.fixed = !x.s.fixed;
						else if (x.s.fixed)
							x.s.fixed = false;
				});
				tmp->RightTapped += ref new RightTappedEventHandler([this](Object^ sender, RightTappedRoutedEventArgs^ e) {
					auto tmp = dynamic_cast<Ellipse^>(sender);
					if (tmp == nullptr)return;
					for (auto &x : nods)
						if (x.s.cir == tmp) {
							wstring now = x.f;
							auto fr = Frame;
							if (now.front() == '-' || now.back() == '-') {
								fr->Navigate(
									TypeName(SingleRootPage::typeid),
									ref new String(now.c_str()),
									ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
							}
							else {
								fr->Navigate(
									TypeName(SingleVocPage::typeid),
									ref new String(now.c_str()),
									ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
							}
						}
				});
				tmp->Opacity = 0;
				nw.cir = tmp;
				return true;
			};
			if (MindMapPage_Navigate_Obj[GetCurrentID()].empty()) {
				tick = 0;
				//for (auto &x : words)
					//Add(x.f, rand() % 3000, rand() % 3000);
				for (auto &x : favorite)
					qu.push_back(x.f), Add(x.f, rand() % 3000, rand() % 3000);
				for (auto &x : favorite) 
					for (auto &y : rt[x.f]) 
						if (nods.find(y) == nods.end())
							qu.push_back(y), Add(y, rand() % 3000, rand() % 3000);
				int tims = 0;
				if (setting[L"mind_map_cnt"] == L"")tims = 500;
				else tims = StrToInt(setting[L"mind_map_cnt"]);
				for (int i = 0; !qu.empty() && i < tims; i++) {
					int tmp = (int)qu.size();
					if(rand()&1)
						swap(qu.back(), qu[rand() % qu.size()]);
					else
						swap(qu.back(), qu[rand() % (qu.size()+1)/2]);
					auto now = qu.back();
					qu.pop_back();
					for (auto &x : rt[now]) {
						if (nods.find(x) == nods.end())
							qu.push_back(x);
					}
					if (!Add(now, rand() % 3000, rand() % 3000)) {
						
					}
				}
			}
			else {
				auto &obj = MindMapPage_Navigate_Obj2[GetCurrentID()];
				pause = obj.pause;
				tick = obj.tick;
				obj.tick = -2;
				//scroll->ChangeView(ref new Box<double>(obj.x), ref new Box<double>(obj.y), ref new Box<float>(obj.scale));
				for (auto &x : MindMapPage_Navigate_Obj[GetCurrentID()]) {
					Add(x.first, x.second.first, x.second.s);
				}
			}
			for (auto &x : nods) {
				for(auto &y : rt[x.f]){
					if (nods.find(y) == nods.end())continue;
					lines.push_back(Lin());
					lines.back().a = x.first;
					lines.back().b = y;
				}
			}
			run = true;
			//for (auto &x : nods)
				//grid->Children->Append(x.s.txt);
			for (auto &x : nods)
				grid->Children->Append(x.s.cir);
			/*tempdispatchertime_upd_sync = ref new DispatcherTimer();
			Windows::Foundation::TimeSpan time;
			time.Duration = 15000;
			tempdispatchertime_upd_sync->Interval = time;
			run = true;
			auto timerDelegate = [this](Object^ e, Object^ ags) {
				nods_lock.lock();
				for (auto &x : nods) 
					x.s.cir->Margin = Thickness(x.s.x - radius*1.5 + 30, x.s.y - radius*1.5 + 30, 0, 0);
				nods_lock.unlock();
			};
			tempdispatchertime_upd_sync->Tick += ref new EventHandler<Object^>(timerDelegate);
			tempdispatchertime_upd_sync->Start();*/
			create_task([this]() {
				while (run) {
					double _t = 0.015;
					int ti = (int)(150 * pow(nods.size(), 0.1));
					if (tick <= ti)_t = 0.015 + (0.05 - 0.015) * (ti - tick) / ti;
					else if(tick<=ti*2) _t = 0.005 + 0.01*(ti*2 - tick) / ti;
					else if(tick<=ti*3) _t = 0.005*(ti*3 - tick) / ti;
					else _t = 0.00;
					//_t = _t / 15 * 100;
					//else _t=0.015*max((200-tick),0)/200;
					//_t = 1 / tick;
					//if (_t < 0.015)_t = 0.015;
					if(!pause)
						tick++;
					/*if (tick == ti * 3) {
						wstring out;
						wchar_t s[1005];
						for (auto &x : nods) {
							out += x.first + L"," + IntToStr(x.second.x) + L" " + IntToStr(x.second.y)+L"\n";
						}
						StrToFile(out, L"mind.txt");
					}*/
					auto dis2 = [](Nod &a, Nod &b) {
						return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
					};
					auto dis2p = [](Nod &a, pair<double,double> &b) {
						return (a.x - b.f)*(a.x - b.f) + (a.y - b.s)*(a.y - b.s);
					};
					/*vector<pair<double, double>>ve1, ve2;
					for (auto &w : nods)
						ve1.push_back(make_pair(w.second.x, w.second.y)),
						ve2.push_back(make_pair(w.second.y, w.second.x));
					sort(ve1.begin(), ve1.end());
					sort(ve2.begin(), ve2.end());*/
							const int dst = 150;
					map<int, map<int, vector<pair<double, double>>>>mp;
					nods_lock.lock();
					for (auto &w : nods)
						mp[(int)(w.second.x / dst)][(int)(w.second.y / dst)].push_back(make_pair(w.second.x, w.second.y));
					nods_lock.unlock();
					const int arx[] = { -3,-3,-3, -2,-2,-2,-2,-2, -1,-1,-1,-1,-1,-1,-1, 0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2,2,2, 3,3,3};
					const int ary[] = { -1,0,1, -2,-1,0,1,2, -3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -2,-1,0,1,2, -1,0,1};
					if(tick!=0&&!pause)
						for (auto &w : nods) {
							if (w.second.fixed)continue;
							double fx = 0, fy = 0;
							auto x = w.second;
							auto now = w.f;
							if (nods.size() > 1) {
								/*auto it = lower_bound(ve1.begin(), ve1.end(), make_pair(x.x - dst, 0.0));
								auto ed = lower_bound(ve1.begin(), ve1.end(), make_pair(x.x + dst, 0.0));
								Nod p;
								for (; it != ed; ++it) {
									p.x = it->first;
									p.y = it->second;
									if (p.x == x.x)continue;
									double di2 = dis2(x, p), ds = sqrt(di2);
									double f = 30000.0*sqrt(nods.size()) / di2;
									fx -= f * (p.x - x.x) / ds;
								}
								it = lower_bound(ve2.begin(), ve2.end(), make_pair(x.y - dst, 0.0));
								ed = lower_bound(ve2.begin(), ve2.end(), make_pair(x.y + dst, 0.0));
								for (; it != ed; ++it) {
									p.x = it->second;
									p.y = it->first;
									if (p.y == x.y)continue;
									double di2 = dis2(x, p), ds = sqrt(di2);
									double f = 30000.0*sqrt(nods.size()) / di2;
									fy -= f * (p.y - x.y) / ds;
								}*//*
								for (int i = int(x.x / dst) - 3; i <= int(x.x / dst) + 3; i++)
									for (int j = int(x.y / dst) - 3; j <= int(x.y / dst) + 3; j++)
										for (auto &y : mp[i][j]) {
											if (y.f==x.x&&y.s == x.y)continue; 
											double di2 = dis2p(x, y), ds = sqrt(di2);
											double f = 30000.0*sqrt(nods.size()) / di2;
											fx -= f * (y.f - x.x) / ds;
											fy -= f * (y.s - x.y) / ds;
										}*/
								int dx = (int)x.x / dst, dy = (int)x.y / dst;
								for (int i = 0; i < 37; i++)
									for (auto &y : mp[dx+arx[i]][dy+ary[i]]) {
										if (y.f == x.x&&y.s == x.y)continue;
										double di2 = dis2p(x, y), ds = sqrt(di2);
										double f = 30000.0*sqrt(nods.size()) / di2;
										fx -= f * (y.f - x.x) / ds;
										fy -= f * (y.s - x.y) / ds;								
									}

							}
							int cnt = 0;
							double fx2 = 0, fy2 = 0;
							for (auto &z : rt[now]) {
								if (nods.find(z) == nods.end())continue;
								nods_lock.lock();
								auto y = nods[z];
								nods_lock.unlock();
								if (x.x == y.x&&x.y == y.y)
									continue;
								double di2 = dis2(x, y), ds = sqrt(di2);
								//double f = min(max((ds - 50) * 5, -300), 300);
								double f;
									f = ds * 10 / pow(nods.size(),0.15);
								/*else if (ds <= 60 + nods.size() / 50)
									f = (ds - 60 + nods.size() / 50) * 8;*/
								fx2 += f * (y.x - x.x) / ds;
								fy2 += f * (y.y - x.y) / ds;
								cnt++;
							}
							//fx2 = fy2 = 0;
							if (cnt == 0)cnt = 1;
							fx = min(max(fx, -1000), 1000) + fx2 / cnt;
							fy = min(max(fy, -1000), 1000) + fy2 / cnt;
							nods_lock.lock();
							w.s.x += fx * _t;
							w.s.y += fy * _t;
							nods_lock.unlock();
						}
					double mx = 999999, my = 999999;
					nods_lock.lock();
					for (auto &x : nods)
						mx = min(mx, x.second.x), my = min(my, x.second.y);
					nods_lock.unlock();
					//if (mx)mx -= 30;
					//if (my)my -= 30;
					nods_lock.lock();
					if (mx < 0)mx = min(0, mx + 15);
					if (mx > 0)mx = max(0, mx - 15);
					if (my < 0)my = min(0, my + 15);
					if (my > 0)my = max(0, my - 15);
					for (auto &x : nods) 
						if(!x.second.fixed)
							x.s.x -= mx, x.s.y -= my;
					nods_lock.unlock();
					//x.s.cir->Margin = Thickness(x.s.x - radius*1.5 + 30, x.s.y - radius*1.5 + 30, 0, 0);
					//x.s.txt->Margin = Thickness(x.s.x + radius , x.s.y - radius - 5, 0, 0);
					/*for (auto &x : lines) {
					auto &tmp = x.li;
					tmp->X1 = nods[x.a].x;
					tmp->Y1 = nods[x.a].y;
					tmp->X2 = nods[x.b].x;
					tmp->Y2 = nods[x.b].y;
					}*/
					Sleep(1);
				}
			});
		}));
	});
}

void CutTheWords::Views::MindMapPage::Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	canvas->RemoveFromVisualTree();
	canvas = nullptr;
	//canvas->Cl
}


void CutTheWords::Views::MindMapPage::canvas_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args)
{
	//if (tick >= 50)
	/*
	nods_lock.lock();
	auto nd = nods;
	nods_lock.unlock();
	for (auto &x : lines) {
		if(nd[x.a].fixed||nd[x.b].fixed)
			args->DrawingSession->DrawLine(nd[x.a].x + 30, nd[x.a].y + 30, nd[x.b].x + 30, nd[x.b].y + 30, ColorHelper::FromArgb(127, 127, 127, 0));
		else
			args->DrawingSession->DrawLine(nd[x.a].x + 30, nd[x.a].y + 30, nd[x.b].x + 30, nd[x.b].y + 30, ColorHelper::FromArgb(127, 255, 0, 0));
	}
	wstring fx = L"";
	for (auto &x : nd)
		if (x.second.fixed)
			fx = x.first;
	//if (tick >= 50)
	for (auto &x : nd) {
		if (x.second.fixed)
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), x.s.x + radius + 30, x.s.y - radius - 8 + 30, ColorHelper::FromArgb(255, 0, 255, 0));
		else if (rt[x.f].find(fx) != rt[x.f].end())
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), x.s.x + radius + 30, x.s.y - radius - 8 + 30, ColorHelper::FromArgb(255, 80, 180, 80));
		else if (favorite.find(x.f) != favorite.end())
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), x.s.x + radius + 30, x.s.y - radius - 8 + 30, ColorHelper::FromArgb(255, 0, 0x78, 0xd7));
		else if (x.first.front()=='-'||x.first.back()=='-')
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), x.s.x + radius + 30, x.s.y - radius - 8 + 30, ColorHelper::FromArgb(255, 20, 120, 120));
		else
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), x.s.x + radius + 30, x.s.y - radius - 8 + 30, Colors::Gray);
	}
	for (auto &x : nd) {
		args->DrawingSession->DrawCircle(x.s.x + 30, x.s.y + 30, radius/2, ColorHelper::FromArgb(127, 127, 127, 127),radius);
		x.s.cir->Margin = Thickness(x.s.x - radius*1.5 + 30, x.s.y - radius*1.5 + 30, 0, 0);
	}
	//for (int i = 0; i < 100; i++)
	//{
		//args->DrawingSession->DrawText("Hello, World!", rand() % 1000, rand() % 1000, Colors::Red);
		//args->DrawingSession->DrawCircle(rand()%1000, rand() % 1000, rand()%20, Colors::Blue);
		//args->DrawingSession->DrawLine(rand()%1000, rand() % 1000, rand() % 1000, rand() % 1000, Colors::Yellow);
	//}*/
}

void CutTheWords::Views::MindMapPage::canvas_Draw_1(Microsoft::Graphics::Canvas::UI::Xaml::ICanvasAnimatedControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedDrawEventArgs^ args)
{
	//args->DrawingSession->DrawText(ref new String(IntToStr(tick).c_str()),  30, 30, ColorHelper::FromArgb(255, 0, 255, 0));
	//args->DrawingSession->DrawText("空白鍵：暫停 F5：活化節點 F6：縮小畫面 F7：放大畫面 右鍵：查看單字詳細內容", 30, 30, Colors::Gray);
	if (!run)return;
	nods_lock.lock();
	auto nd = nods;
	nods_lock.unlock();
	for (auto &x : lines) {
		if (nd[x.a].fixed || nd[x.b].fixed)
			args->DrawingSession->DrawLine((float)nd[x.a].x + 30, (float)nd[x.a].y + 30, (float)nd[x.b].x + 30, (float)nd[x.b].y + 30, ColorHelper::FromArgb(127, 127, 127, 0));
		else
			args->DrawingSession->DrawLine((float)nd[x.a].x + 30, (float)nd[x.a].y + 30, (float)nd[x.b].x + 30, (float)nd[x.b].y + 30, ColorHelper::FromArgb(127, 255, 0, 0));
	}
	wstring fx = L"";
	for (auto &x : nd)
		if (x.second.fixed)
			fx = x.first;
	//if (tick >= 50)
	for (auto &x : nd) {
		if (x.second.fixed)
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), (float)x.s.x + (float)radius + 30, (float)x.s.y - (float)radius - 8 + 30, ColorHelper::FromArgb(255, 0, 230, 30));
		else if (rt[x.f].find(fx) != rt[x.f].end())
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), (float)x.s.x + (float)radius + 30, (float)x.s.y - (float)radius - 8 + 30, ColorHelper::FromArgb(255, 80, 180, 80));
		else if (favorite.find(x.f) != favorite.end())
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), (float)x.s.x + (float)radius + 30, (float)x.s.y - (float)radius - 8 + 30, ColorHelper::FromArgb(255, 0, 0x78, 0xd7));
		else if (x.first.front() == '-' || x.first.back() == '-')
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), (float)x.s.x + (float)radius + 30, (float)x.s.y - (float)radius - 8 + 30, ColorHelper::FromArgb(255, 20, 120, 120));
		else
			args->DrawingSession->DrawText(ref new String(x.f.c_str()), (float)x.s.x + (float)radius + 30, (float)x.s.y - (float)radius - 8 + 30, Colors::Gray);
	}
	for (auto &x : nd) 
		args->DrawingSession->DrawCircle((float)(x.s.x + 30), (float)(x.s.y + 30), (float)(radius / 2), ColorHelper::FromArgb(127, 127, 127, 127), (float)radius);
		
	Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this,nd]()
	{
		for (auto &x : nd) 
			x.s.cir->Margin = Thickness(x.s.x - radius*1.5 + 30, x.s.y - radius*1.5 + 30, 0, 0);
		auto &obj = MindMapPage_Navigate_Obj2[GetCurrentID()];
		if (obj.tick == -2) {
			if (scroll->HorizontalOffset == obj.x&&scroll->VerticalOffset == obj.y && scroll->ZoomFactor == obj.scale)
				obj.tick = -1;
			else
				scroll->ChangeView(ref new Box<double>(obj.x), ref new Box<double>(obj.y), ref new Box<float>(obj.scale));
		}
	}));
}


void CutTheWords::Views::MindMapPage::canvas_CreateResources(Microsoft::Graphics::Canvas::UI::Xaml::CanvasAnimatedControl^ sender, Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs^ args)
{
}


void CutTheWords::Views::MindMapPage::Page_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{

}


void CutTheWords::Views::MindMapPage::OnKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
{
	if (args->VirtualKey == Windows::System::VirtualKey::Space) {
		pause = !pause;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::F5) {
		tick = min(tick, 150);
	}
	if (args->VirtualKey == Windows::System::VirtualKey::F6) {
		scroll->ChangeView(
			ref new Box<double>((scroll->HorizontalOffset- (scroll->ActualWidth / 2)*0.5) / 1.5),
			ref new Box<double>((scroll->VerticalOffset- (scroll->ActualWidth / 2)*0.5) / 1.5),
			ref new Box<float>((float)(scroll->ZoomFactor / 1.5)));
	}
	if (args->VirtualKey == Windows::System::VirtualKey::F7) {
		scroll->ChangeView(
			ref new Box<double>(scroll->HorizontalOffset * 1.5 + (scroll->ActualWidth / 2)*0.5),
			ref new Box<double>(scroll->VerticalOffset * 1.5 + (scroll->ActualHeight / 2)*0.5),
			ref new Box<float>((float)(scroll->ZoomFactor * 1.5)));
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Up) {
		scroll->ChangeView(
			ref new Box<double>(scroll->HorizontalOffset),
			ref new Box<double>(scroll->VerticalOffset - 150),
			ref new Box<float>(scroll->ZoomFactor));
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Down) {
		scroll->ChangeView(
			ref new Box<double>(scroll->HorizontalOffset),
			ref new Box<double>(scroll->VerticalOffset + 150),
			ref new Box<float>(scroll->ZoomFactor));
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Left) {
		scroll->ChangeView(
			ref new Box<double>(scroll->HorizontalOffset - 150),
			ref new Box<double>(scroll->VerticalOffset),
			ref new Box<float>(scroll->ZoomFactor));
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Right) {
		scroll->ChangeView(
			ref new Box<double>(scroll->HorizontalOffset + 150),
			ref new Box<double>(scroll->VerticalOffset),
			ref new Box<float>(scroll->ZoomFactor));
	}
}


void CutTheWords::Views::MindMapPage::canvas_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e)
{
	if (notscroll)return;
	scroll->ChangeView(
		ref new Box<double>(scroll->HorizontalOffset - e->Delta.Translation.X*4),
		ref new Box<double>(scroll->VerticalOffset - e->Delta.Translation.Y*4),
		ref new Box<float>(scroll->ZoomFactor));
	//scroll->ZoomToFactor(scroll->ZoomFactor*e->Delta.Scale);
}
