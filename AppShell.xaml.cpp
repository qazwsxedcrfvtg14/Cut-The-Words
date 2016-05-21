﻿//
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

		SystemNavigationManager::GetForCurrentView()->BackRequested +=
			ref new EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &AppShell::SystemNavigationManager_BackRequested);
		// If on a phone device that has hardware buttons then we hide the app's back button.
		if (Windows::Foundation::Metadata::ApiInformation::IsTypePresent("Windows.Phone.UI.Input.HardwareButtons"))
		{
			BackButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
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
				Symbol::Link,
				TypeName(Views::TestPage::typeid)));
		navlist->Append(
			ref new NavMenuItem(
				"歷程記錄",
				Symbol::Clock,
				TypeName(Views::HistoryPage::typeid)));
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
				"設定",
				Symbol::Setting,
				TypeName(Views::SettingPage::typeid)));
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
		bool handled = e->Handled;
		BackRequested(&handled);
		e->Handled = handled;
	}

	void AppShell::BackButton_Click(Object^ sender, RoutedEventArgs^ e)
	{
		bool ignored = false;
		BackRequested(&ignored);
	}

	void AppShell::BackRequested(bool* handled)
	{
		// Get a hold of the current frame so that we can inspect the app back stack.

		if (AppFrame == nullptr)
			return;

		// Check to see if this is the top-most page on the app back stack.
		if (AppFrame->CanGoBack && !(*handled))
		{
			// If not, set the event to handled and go back to the previous page in the app.
			*handled = true;
			AppFrame->GoBack(ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		}
	}

	/// <summary>
	/// Navigate to the Page for the selected <paramref name="listViewItem"/>.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="listViewItem"></param>
	void AppShell::NavMenuList_ItemInvoked(Object^ sender, ListViewItem^ listViewItem)
	{

		auto nmlv = dynamic_cast<NavMenuListView^>(sender);
		if (nmlv != nullptr) {
			auto item = dynamic_cast<NavMenuItem^>(nmlv->ItemFromContainer(listViewItem));
			//auto item = (NavMenuItem^)((NavMenuListView^)(sender))->ItemFromContainer(listViewItem);
			if (item != nullptr)
			{
				if (item->DestPage.Name != AppFrame->CurrentSourcePageType.Name)
				{
					if (item->Arguments != nullptr)
						AppFrame->Navigate(item->DestPage, item->Arguments);
					else
						AppFrame->Navigate(item->DestPage);
				}
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
	}

	void AppShell::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
	{
		((Page^)sender)->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		CheckTogglePaneButtonSizeChanged();
	}

	/// <summary>
	/// Callback when the SplitView's Pane is toggled open or close.  When the Pane is not visible
	/// then the floating hamburger may be occluding other content in the app unless it is aware.
	/// </summary>
	/// <param name="sender"></param>
	/// <param name="e"></param>
	void AppShell::TogglePaneButton_Checked(Object^ sender, RoutedEventArgs^ e)
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
