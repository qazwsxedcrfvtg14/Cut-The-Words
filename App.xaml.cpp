//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "AppShell.xaml.h"
#include "BasicPage.xaml.h"
#include "SearchVocPage.xaml.h"
#include "LandingPage.xaml.h"
#include "LoadingPage.xaml.h"
#include "NullPage.xaml.h"
#include "Voc.h"




using namespace CutTheWords;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>

App::App()
{
	int data_version = 18;
	srand((unsigned int)time(0));
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"setting.txt")) == nullptr)
		DumpAppFile(L"setting.txt");
	get_doc(L"setting.txt", setting);
	if (setting[L"theme"] == L"light")
		Application::RequestedTheme = ApplicationTheme::Light;
	if (setting[L"theme"] == L"dark")
		Application::RequestedTheme = ApplicationTheme::Dark;
	if (setting[L"website"] == L"")
		//setting[L"website"] = L"http://localhost/words";
		setting[L"website"] = L"http://joe59491.azurewebsites.net";
	if (setting[L"sound_url"] == L"")
		setting[L"sound_url"] = L"http://cn.bing.com/dict/search?mkt=zh-cn&q=";
	if (setting[L"sound_url2"] == L"")
		setting[L"sound_url2"] = L"https://dictionary.blob.core.chinacloudapi.cn/media/audio/tom";
	if (setting[L"sound_type"] == L"")
		setting[L"sound_type"] = L".mp3";
	if (setting[L"data_version"] == L"")
		setting[L"data_version"] = L"0";
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"words.txt")) == nullptr || StrToInt(setting[L"data_version"]) < data_version)
		DumpAppFile(L"words.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"prefix.txt")) == nullptr || StrToInt(setting[L"data_version"]) < data_version)
		DumpAppFile(L"prefix.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"suffix.txt")) == nullptr || StrToInt(setting[L"data_version"]) < data_version)
		DumpAppFile(L"suffix.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"root.txt")) == nullptr || StrToInt(setting[L"data_version"]) < data_version)
		DumpAppFile(L"root.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"note.txt")) == nullptr || StrToInt(setting[L"data_version"]) < data_version)
		DumpAppFile(L"note.txt");
	if (StrToInt(setting[L"data_version"]) < data_version)
		setting[L"data_version"] = IntToStr(data_version);
	SavingSetting();
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"words_user.txt")) == nullptr)
		StrToFile(L"", L"words_user.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"prefix_user.txt")) == nullptr)
		StrToFile(L"", L"prefix_user.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"suffix_user.txt")) == nullptr)
		StrToFile(L"", L"suffix_user.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"root_user.txt")) == nullptr)
		StrToFile(L"", L"root_user.txt");
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"note_user.txt")) == nullptr)
		StrToFile(L"", L"note_user.txt");
	get_doc(L"words.txt", words, ok_words);
	get_doc(L"prefix.txt", prefix);
	get_doc(L"suffix.txt", suffix);
	get_doc(L"root.txt", root);
	if (AWait(ApplicationData::Current->LocalFolder->TryGetItemAsync(L"favorite.txt")) == nullptr)
		DumpAppFile(L"favorite.txt");
	get_doc(L"favorite.txt", favorite);
	get_doc(L"note.txt", note);

	/*create_task([=] {
		auto nrtp = make_unique<map<wstring, set<wstring>>>();
		for (auto &x : words) {
			auto ve = Show2(x.f);
			for (auto &y : ve) {
				if (y.front() != '-'&&y.back() != '-')
					y= WordRotToExp(y).s;
				if(y!=x.f)
					(*nrtp)[y].insert(x.f);
			}
		}
		swap(nrtp, rtp);
	});*/
	InitializeComponent();
	/*
	auto database_sync = ref new DispatcherTimer();
	Windows::Foundation::TimeSpan time;
	time.Duration = 300000000;
	database_sync->Interval = time;
	auto timerDelegate = [this](Object^ e, Object^ ags) {
		StrToFile(dump_doc(setting), L"setting.txt");
		StrToFile(dump_doc(favorite), L"favorite.txt");
		//StrToFile(dump_doc(words,ok_words), L"words_user.txt");
		//StrToFile(dump_doc(prefix), L"prefix_user.txt");
		//StrToFile(dump_doc(suffix), L"suffix_user.txt");
		//StrToFile(dump_doc(root), L"root_user.txt");
		//StrToFile(dump_doc(note), L"note_user.txt");
	};
	database_sync->Tick += ref new EventHandler<Object^>(timerDelegate);
	database_sync->Start();*/
	//InitializeLicense();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
	if (e->PrelaunchActivated)
	{
		return;
	}
#if _DEBUG
    // Show graphics profiling information while debugging.
    if (IsDebuggerPresent())
    {
        // Display the current frame rate counters
         //DebugSettings->EnableFrameRateCounter = true;
    }
#endif
    auto shell = dynamic_cast<AppShell^>(Window::Current->Content);
    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (shell == nullptr)
    {
        // Create a AppShell to act as the navigation context and navigate to the first page
        shell = ref new AppShell();

        shell->AppFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete

        }
		
		
    }
	else if(false){
		CoreApplicationView^ newView = CoreApplication::CreateNewView();
		int newViewId ;
		AWait(newView->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([&newViewId]()
		{
			auto theView = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
			newViewId = theView->Id;
			//Frame^ frame = ref new Frame();
			auto shell=ref new AppShell(); 
			if (setting[L"home_page"] == L"search")
				shell->AppFrame->Navigate(TypeName(Views::SearchVocPage::typeid));
			else
				shell->AppFrame->Navigate(TypeName(Views::BasicPage::typeid));
			Window::Current->Content = shell;
			// You have to activate the window in order to show it later.
			Window::Current->Activate();
			//ShowMsg(L"Please install the new version of this app!");
			//auto success = AWait(Windows::System::Launcher::LaunchUriAsync(ref new Uri("ms-windows-store://pdp/?ProductId=9nblggh432kk")));
		})));
		create_task(Windows::UI::ViewManagement::ApplicationViewSwitcher::TryShowAsStandaloneAsync(newViewId)).then([](bool b) {
			
		});
		//
	}
	//
    // Place our app shell in the current Window
    Window::Current->Content = shell;
	

    if (shell->AppFrame->Content == nullptr)
    {
        // When the navigation stack isn't restored navigate to the first page,
        // suppressing the initial entrance animation and configuring the new 
        // page by passing required information as a navigation parameter
		//shell->AppFrame->Navigate(TypeName(Views::LandingPage::typeid), e->Arguments, ref new Windows::UI::Xaml::Media::Animation::SuppressNavigationTransitionInfo());
		if(setting[L"home_page"] == L"search")
			shell->AppFrame->Navigate(TypeName(Views::SearchVocPage::typeid), e->Arguments, ref new Windows::UI::Xaml::Media::Animation::SuppressNavigationTransitionInfo());
		else
			shell->AppFrame->Navigate(TypeName(Views::BasicPage::typeid), e->Arguments, ref new Windows::UI::Xaml::Media::Animation::SuppressNavigationTransitionInfo());
	}
	
    // Ensure the current window is active
    Window::Current->Activate();
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void)sender;   // Unused parameter
	(void)e;    // Unused parameter

    //TODO: Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}