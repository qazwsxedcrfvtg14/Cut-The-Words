//
// AppShell.xaml.cpp
// Implementation of the AppShell class
//

#include "pch.h"
#include "AddVocPage.xaml.h"
#include "BasicPage.xaml.h"
#include "BasicSubPage.xaml.h"
#include "CommandBarPage.xaml.h"
#include "DrillInPage.xaml.h"
#include "SearchRootPage.xaml.h"
#include "SearchVocPage.xaml.h"
#include "FindVocPage.xaml.h"
#include "SettingPage.xaml.h"
#include "NavMenuItem.h"
#include "NavMenuListView.h"
#include "FavoritePage.xaml.h"
#include "TestPage.xaml.h"
#include "HistoryPage.xaml.h"
#include "ExamPage.xaml.h"
#include "CameraPage.xaml.h"
#include "PicPage.xaml.h"
#include "AppShell.xaml.h"
#include "MAPage.xaml.h"
#include "MALoginPage.xaml.h"
#include "MindMapPage.xaml.h"

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

using namespace CutTheWords::Controls;

namespace CutTheWords
{
	AppShell^ AppShell::_current;

	/// <summary>
	/// Initializes a new instance of the AppShell, sets the static 'Current' reference,
	/// adds callbacks for Back requests and changes in the SplitView's DisplayMode, and
	/// provide the nav menu list with the data to display.
	/// </summary>
	AppShell::AppShell()
	{
		InitializeComponent();

		Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &AppShell::OnLoaded);

		RootSplitView->RegisterPropertyChangedCallback(
			SplitView::DisplayModeProperty,
			ref new DependencyPropertyChangedCallback(this, &AppShell::RootSplitViewDisplayModeChangedCallback));

		SystemNavigationManager::GetForCurrentView()->BackRequested +=
			ref new EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &AppShell::SystemNavigationManager_BackRequested);

		// Declare the top level nav items
		navlist = ref new Vector<NavMenuItem^>();
		navlist->Append(
			ref new NavMenuItem(
				"家",
				Symbol::Home,
				TypeName(Views::BasicPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"單字查詢",
				Symbol::List,
				TypeName(Views::SearchVocPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"部首查詢",
				Symbol::Library,
				TypeName(Views::SearchRootPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"單字搜尋",
				Symbol::Find,
				TypeName(Views::FindVocPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"新增資料",
				Symbol::Edit,
				TypeName(Views::AddVocPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"我的最愛",
				Symbol::Favorite,
				TypeName(Views::FavoritePage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"單字測驗",
				Symbol::Tag,
				TypeName(Views::TestPage::typeid)));
		/*navlist->Append(
			ref new NavMenuItem(
				"歷程記錄",
				Symbol::Clock,
				TypeName(Views::HistoryPage::typeid)));*/
		navlist->Append(
			ref new NavMenuItem(
				"圖片掃描",
				Symbol::Pictures,
				TypeName(Views::PicPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"相機掃描",
				Symbol::Camera,
				TypeName(Views::CameraPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"心智圖",
				Symbol::Link,
				TypeName(Views::MindMapPage::typeid)));
		/*navlist->Append(
			ref new NavMenuItem(
				"設定",
				Symbol::Setting,
				TypeName(Views::SettingPage::typeid)));*/
		if (setting[L"website"] == L"BMA") {
			navlist->Append(
				ref new NavMenuItem(
					"MAHome",
					Symbol::Setting,
					TypeName(Views::MALoginPage::typeid)));
			navlist->Append(
				ref new NavMenuItem(
					"MA",
					Symbol::Setting,
					TypeName(Views::MAPage::typeid)));
		}
		NavMenuList->ItemsSource = navlist;
	}

	Frame^ AppShell::AppFrame::get()
	{
		return frame;
	}

	Frame^ AppShell::AppTopFrame::get()
	{
		return top_frame;
	}
	void AppShell::OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs ^e)
	{
		AppShell::_current = this;

		TogglePaneButton->Focus(Windows::UI::Xaml::FocusState::Programmatic);
	}

	/// <summary>
	/// Default keyboard focus movement for any unhandled keyboarding
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void AppShell::AppShell_KeyDown(Object^ sender, KeyRoutedEventArgs^ e)
	{
		FocusNavigationDirection direction = FocusNavigationDirection::None;
		switch (e->Key)
		{
		case Windows::System::VirtualKey::Left:
		case Windows::System::VirtualKey::GamepadDPadLeft:
		case Windows::System::VirtualKey::GamepadLeftThumbstickLeft:
		case Windows::System::VirtualKey::NavigationLeft:
			direction = FocusNavigationDirection::Left;
			break;
		case Windows::System::VirtualKey::Right:
		case Windows::System::VirtualKey::GamepadDPadRight:
		case Windows::System::VirtualKey::GamepadLeftThumbstickRight:
		case Windows::System::VirtualKey::NavigationRight:
			direction = FocusNavigationDirection::Right;
			break;

		case Windows::System::VirtualKey::Up:
		case Windows::System::VirtualKey::GamepadDPadUp:
		case Windows::System::VirtualKey::GamepadLeftThumbstickUp:
		case Windows::System::VirtualKey::NavigationUp:
			direction = FocusNavigationDirection::Up;
			break;

		case Windows::System::VirtualKey::Down:
		case Windows::System::VirtualKey::GamepadDPadDown:
		case Windows::System::VirtualKey::GamepadLeftThumbstickDown:
		case Windows::System::VirtualKey::NavigationDown:
			direction = FocusNavigationDirection::Down;
			break;
		}

		if (direction != FocusNavigationDirection::None)
		{
			Control^ control = dynamic_cast<Control^>(FocusManager::FindNextFocusableElement(direction));
			if (control != nullptr)
			{
				control->Focus(Windows::UI::Xaml::FocusState::Programmatic);
			}
		}
	}

	void AppShell::SystemNavigationManager_BackRequested(Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ e)
	{
		if (!e->Handled && AppFrame->CanGoBack)
		{
			e->Handled = true;
			AppFrame->GoBack();
		}
	}

	/// <summary>
	/// Navigate to the Page for the selected <paramref name="listViewItem"/>.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="listViewItem"></param>
	void AppShell::NavMenuList_ItemInvoked(Object^ sender, ListViewItem^ listViewItem)
	{
		auto item = (NavMenuItem^)((NavMenuListView^)(sender))->ItemFromContainer(listViewItem);

		if (item != nullptr)
		{
			if (item->DestPage.Name != AppFrame->CurrentSourcePageType.Name)
			{
				/*auto frm = ref new Windows::UI::Xaml::Controls::Frame();
				frm->Margin = Thickness(0, 4, 0, 0);
				frm->Navigating += ref new NavigatingCancelEventHandler(this, &AppShell::OnNavigatingToPage);
				frm->Navigated += ref new NavigatedEventHandler(this, &AppShell::OnNavigatedToPage);
				frm->Navigate(item->DestPage, item->Arguments);
				grame_grid->Children->Append(frm);*/
				AppFrame->Navigate(item->DestPage, item->Arguments);
				//AppFrame->Navigate(TypeName(Views::BasicPage::typeid), item->Arguments);
			}
		}
	}

	void AppShell::OnNavigatingToPage(Object^ sender, NavigatingCancelEventArgs^ e)
	{
		if (e->NavigationMode == NavigationMode::Back)
		{
			NavMenuItem^ item = nullptr;
			for (auto p : navlist)
			{
				if (p->DestPage.Name == e->SourcePageType.Name)
				{
					item = p;
					break;
				}
			}

			if (item == nullptr && AppFrame->BackStackDepth > 0)
			{
				// In cases where a page drills into sub-pages then we'll highlight the most recent
				// navigation menu item that appears in the BackStack
				for (auto entry : AppFrame->BackStack)
				{
					Object^ p = nullptr;
					for (auto p : navlist)
					{
						if (p->DestPage.Name == entry->SourcePageType.Name)
						{
							item = p;
							break;
						}
					}
				}
			}

			auto container = (ListViewItem^)NavMenuList->ContainerFromItem(item);

			// While updating the selection state of the item prevent it from taking keyboard focus.  If a
			// user is invoking the back button via the keyboard causing the selected nav menu item to change
			// then focus will remain on the back button.
			if (container != nullptr) container->IsTabStop = false;
			NavMenuList->SetSelectedItem(container);
			if (container != nullptr) container->IsTabStop = true;
		}
	}


	void AppShell::OnNavigatedToPage(Object^ sender, NavigationEventArgs^ e)
	{
		// After a successful navigation set keyboard focus to the loaded page
		if (e->Content != nullptr && dynamic_cast<Page^>(e->Content) != nullptr)
		{
			auto control = (Page^)e->Content;
			control->Loaded += ref new RoutedEventHandler(this, &AppShell::Page_Loaded);
		}

		// Show the Back button
		SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility =
			Windows::UI::Core::AppViewBackButtonVisibility::Visible;
	}

	void AppShell::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
	{
		((Page^)sender)->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		CheckTogglePaneButtonSizeChanged();
	}

	/// <summary>
	/// Public method to allow pages to open SplitView's pane.
	/// Used for custom app shortcuts like navigating left from page's left-most item
	/// </summary>
	void AppShell::OpenNavePane()
	{
		TogglePaneButton->IsChecked = true;
		NavPaneDivider->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}

	// <summary>
	/// Hide divider when nav pane is closed.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="args"></param>
	void AppShell::RootSplitView_PaneClosed(SplitView^ sender, Object^ args)
	{
		NavPaneDivider->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}

	/// <summary>
	/// Callback when the SplitView's Pane is toggled opened.
	/// Restores divider's visibility and ensures that margins around the floating hamburger are correctly set.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void AppShell::TogglePaneButton_Checked(Object^ sender, RoutedEventArgs^ e)
	{
		NavPaneDivider->Visibility = Windows::UI::Xaml::Visibility::Visible;
		CheckTogglePaneButtonSizeChanged();
	}

	/// <summary>
	/// Callback when the SplitView's Pane is toggled closed.  When the Pane is not visible
	/// then the floating hamburger may be occluding other content in the app unless it is aware.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void AppShell::TogglePaneButton_Unchecked(Object^ sender, RoutedEventArgs^ e)
	{
		CheckTogglePaneButtonSizeChanged();
	}

	/// <summary>
	/// Ensure that we update the reported size of the TogglePaneButton when the SplitView's
	/// DisplayMode changes.
	/// </summary>
	void AppShell::RootSplitViewDisplayModeChangedCallback(DependencyObject^ sender, DependencyProperty^ dp)
	{
		CheckTogglePaneButtonSizeChanged();
	}

	/// <summary>
	/// Check for the conditions where the navigation pane does not occupy the space under the floating
	/// hamburger button and trigger the event.
	/// </summary>
	void AppShell::CheckTogglePaneButtonSizeChanged()
	{
		if (RootSplitView->DisplayMode == SplitViewDisplayMode::Inline ||
			RootSplitView->DisplayMode == SplitViewDisplayMode::Overlay)
		{
			auto transform = TogglePaneButton->TransformToVisual(this);
			auto rect = transform->TransformBounds(Rect(0, 0, (float)TogglePaneButton->ActualWidth, (float)TogglePaneButton->ActualHeight));
			_togglePaneButtonRect = rect;
		}
		else
		{
			_togglePaneButtonRect = Rect();
		}

		TogglePaneButtonRectChanged(this, TogglePaneButtonRect);
	}

	/// <summary>
	/// Enable accessibility on each nav menu item by setting the AutomationProperties.Name on each container
	/// using the associated Label of each item.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="args"></param>
	void AppShell::NavMenuItemContainerContentChanging(ListViewBase^ sender, ContainerContentChangingEventArgs^ args)
	{
		if (!args->InRecycleQueue && args->Item != nullptr && dynamic_cast<NavMenuItem^>(args->Item) != nullptr)
		{
			args->ItemContainer->SetValue(Windows::UI::Xaml::Automation::AutomationProperties::NameProperty, ((NavMenuItem^)args->Item)->Label);
		}
		else
		{
			args->ItemContainer->ClearValue(Windows::UI::Xaml::Automation::AutomationProperties::NameProperty);
		}
	}
}


void CutTheWords::AppShell::SettingsNavPaneButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{


	TogglePaneButton->IsChecked = false;
	AppFrame->Navigate(
		TypeName(Views::SettingPage::typeid),
		nullptr);
}


void CutTheWords::AppShell::FeedbackNavPaneButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	auto success = AWait(Windows::System::Launcher::LaunchUriAsync(ref new Uri("ms-windows-store://pdp/?productid=9nblggh4wfmb")));

}
