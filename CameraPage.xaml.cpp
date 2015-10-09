//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "pch.h"
#include "CameraPage.xaml.h"
#include "SingleVocPage.xaml.h"

double wstring_height (wstring s) {
	bool up=0, dw=0;
	for (auto &x : s)
		if (x == 'b')up = 1;
		else if (x == 'd')up = 1;
		else if (x == 'f')up = 1;
		else if (x == 'h')up = 1;
		else if (x == 'i')up = 1;
		else if (x == 'k')up = 1;
		else if (x == 'l')up = 1;
		//else if (x == 't')up = 1;
		else if (x == 'g')dw = 1;
		else if (x == 'j')up = 1,dw = 1;
		else if (x == 'p')dw = 1;
		else if (x == 'q')dw = 1;
		else if (x == 'y')dw = 1;
		else if (x == '.');
		else if (x == ',');
		else if (x == '-');
		else if (x == '_');
		else if (x<'a'&&x>'z')up = 1;
	if (!up&&!dw)
		return 1.2;
	else if (dw&&up)
		return 0.8;
	else
		return 1;
}

using namespace CutTheWords::Views;

using namespace Platform;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media;
using namespace Windows::Media::MediaProperties;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

using namespace std;

bool Camera_init = 0;
// Creates a task that completes with a void result.
concurrency::task<void> create_empty_task()
{
	return concurrency::create_task([] {});
}
CameraPage::CameraPage()
{
	InitializeComponent();


	// Useful to know when to initialize/clean up the camera.
	//Application::Current->Suspending += ref new Windows::UI::Xaml::SuspendingEventHandler(this, &CameraPage::Application_Suspending); ;
	//Application::Current->Resuming += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &CameraPage::Application_Resuming);;
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// Ckecks if English language is avaiable for OCR on device and starts camera preview.
/// </summary>
/// <param name="e"></param>

Object^ CameraPage_Navigate_Obj1;
Object^ CameraPage_Navigate_Obj2;
//vector<CutTheWords::WordOverlay^> CameraPage_Navigate_wordBoxes;
void CameraPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	//orientationChangedEventToken = displayInformation->OrientationChanged += ref new Windows::Foundation::TypedEventHandler<Windows::Graphics::Display::DisplayInformation ^, Platform::Object ^>(this, &SDKTemplate::CameraPage::DisplayInformation_OrientationChanged);

	ocrLanguage = ref new Windows::Globalization::Language("en");
	//auto nav = dynamic_cast<Media::ImageSource^>(CameraPage_Navigate_Obj1);
	auto nav = nullptr;
	if (nav != nullptr) {
		PreviewImage->Source = nav;
		ImageGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
		PreviewControl->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		ExtractButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		slider->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		CameraButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
		auto nav2 = dynamic_cast<Grid^>(CameraPage_Navigate_Obj2);
		if (nav2 != nullptr) {
			//wordBoxes = CameraPage_Navigate_wordBoxes;
			TextOverlay->RenderTransform = nav2->RenderTransform;
			vector<UIElement^>child;
			for (auto &x : nav2->Children)
				child.push_back(x);
			nav2->Children->Clear();
			for (auto &x : child) {
				x->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::CameraPage::OnTapped);
				TextOverlay->Children->Append(x);
			}
		}

	}
	else {
		/*if (!OcrEngine::IsLanguageSupported(ocrLanguage))
		{
			ShowMsg((ocrLanguage->DisplayName + " language ia not supported.")->Data());
			//rootPage->NotifyUser(ocrLanguage->DisplayName + " language ia not supported.", NotifyType::ErrorMessage);

			return;
		}*/

		create_task(StartCameraAsync()).then([this](task<void> prevTask)
		{
			Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
			{
				// Update buttons visibility.
				ExtractButton->Visibility = isInitialized ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				slider->Visibility = isInitialized ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				CameraButton->Visibility = isInitialized ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
				if (isInitialized&&mediaCapture->VideoDeviceController->ZoomControl->Supported) {
					auto tmp = mediaCapture->VideoDeviceController->ZoomControl;
					slider->Maximum = tmp->Max;
					slider->Minimum = tmp->Min;
					float stp = tmp->Step;
					if (stp < 0.01)stp = ((int)(0.01 / stp))*stp;
					slider->SmallChange = stp;
					slider->StepFrequency = stp;
					slider->TickFrequency = 1;
					slider->Value = mediaCapture->VideoDeviceController->ZoomControl->Value;

				}
			}));
		});
	}
}

/// <summary>
/// Invoked immediately before the Page is unloaded and is no longer the current source of a parent Frame.
/// Stops camera if initialized.
/// </summary>
/// <param name="e"></param>
void CameraPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	if (ImageGrid->Visibility == Windows::UI::Xaml::Visibility::Visible) {
		CameraPage_Navigate_Obj1 = PreviewImage->Source;
		if (CameraButton->Visibility == Windows::UI::Xaml::Visibility::Visible)
			CameraPage_Navigate_Obj2 = TextOverlay;
			//CameraPage_Navigate_wordBoxes = wordBoxes;
		else
			CameraPage_Navigate_Obj2 = nullptr;
	}
	else
		CameraPage_Navigate_Obj1 = nullptr;

	displayInformation->OrientationChanged -= orientationChangedEventToken;

	create_task(CleanupCameraAsync());
	Page::OnNavigatedFrom(e);
}

/// <summary>
/// Occures on app suspending. Stops camera if initialized.
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CameraPage::Application_Suspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e)
{
	
	// Handle global application events only if this page is active.
	/*if (Frame->CurrentSourcePageType.Name == Windows::UI::Xaml::Interop::TypeName(MainPage::typeid).Name)
	{
		auto deferral = e->SuspendingOperation->GetDeferral();

		create_task(CleanupCameraAsync()).then([this, deferral]()
		{
			displayInformation->OrientationChanged -= orientationChangedEventToken;

			deferral->Complete();
		});;
	}*/
}

/// <summary>
/// Occures on app resuming. Initializes camera if available.
/// </summary>
/// <param name="sender"></param>
/// <param name="o"></param>
void CameraPage::Application_Resuming(Platform::Object^ sender, Platform::Object^ o)
{
	// Handle global application events only if this page is active
	/*if (Frame->CurrentSourcePageType.Name == Windows::UI::Xaml::Interop::TypeName(MainPage::typeid).Name)
	{
		orientationChangedEventToken = displayInformation->OrientationChanged += ref new Windows::Foundation::TypedEventHandler<Windows::Graphics::Display::DisplayInformation ^, Platform::Object ^>(this, &SDKTemplate::CameraPage::DisplayInformation_OrientationChanged);

		create_task(StartCameraAsync());
	}*/
}

/// <summary>
/// Occures when display orientation changes.
/// Sets camera rotation preview.
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void CameraPage::DisplayInformation_OrientationChanged(Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args)
{
	if (isPreviewing)
	{
		create_task(SetPreviewRotationAsync());
	}
}

/// <summary>
/// This is event handler for 'Extract' button.
/// Captures image from camera ,recognizes text and displays it.
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CameraPage::ExtractButton_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	// Get information about the preview.
	auto previewProperties = safe_cast<VideoEncodingProperties^>(mediaCapture->VideoDeviceController->GetMediaStreamProperties(MediaStreamType::VideoPreview));
	int videoFrameWidth = (int)previewProperties->Width;
	int videoFrameHeight = (int)previewProperties->Height;

	// In portrait modes, the width and height must be swapped for the VideoFrame to have the correct aspect ratio and avoid letterboxing / black bars.
	if (!externalCamera && (displayInformation->CurrentOrientation == DisplayOrientations::Portrait || displayInformation->CurrentOrientation == DisplayOrientations::PortraitFlipped))
	{
		videoFrameWidth = (int)previewProperties->Height;
		videoFrameHeight = (int)previewProperties->Width;
	}

	// Create the video frame to request a SoftwareBitmap preview frame.
	auto videoFrame = ref new VideoFrame(BitmapPixelFormat::Bgra8, videoFrameWidth, videoFrameHeight);

	OcrEngine^ ocrEngine = OcrEngine::TryCreateFromLanguage(ref new Windows::Globalization::Language("en"));

	// Capture the preview frame.
	auto bitmap = make_shared<SoftwareBitmap^>(nullptr);
	create_task(mediaCapture->GetPreviewFrameAsync(videoFrame)).then([this, bitmap, ocrEngine](VideoFrame^ currentFrame)
	{
		// Collect the resulting frame.
		*bitmap = currentFrame->SoftwareBitmap;

		auto imgSource = ref new WriteableBitmap((*bitmap)->PixelWidth, (*bitmap)->PixelHeight);
		(*bitmap)->CopyToBuffer(imgSource->PixelBuffer);
		PreviewImage->Source = imgSource;
		
		PreviewControl->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		ImageGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
		return ocrEngine->RecognizeAsync(*bitmap);
	}).then([this, bitmap, ocrEngine](OcrResult^ ocrResult)
	{
		// Used for text overlay.
		// Prepare scale transform for words since image is not displayed in original format.
		auto scaleTrasform = ref new ScaleTransform();
		scaleTrasform->CenterX = 0;
		scaleTrasform->CenterY = 0;
		scaleTrasform->ScaleX = PreviewControl->ActualWidth / (*bitmap)->PixelWidth;
		//ShowMsg(IntToStr(scaleTrasform->ScaleX*1000));
		scaleTrasform->ScaleY = PreviewControl->ActualHeight / (*bitmap)->PixelHeight;

		if (ocrResult->TextAngle != nullptr)
		{
			// If text is detected under some angle in this sample scenario we want to
			// overlay word boxes over original image, so we rotate overlay boxes.
			auto rotateTransform = ref new RotateTransform();
			rotateTransform->Angle = ocrResult->TextAngle->Value;
			rotateTransform->CenterX = PreviewImage->ActualWidth / 2;
			rotateTransform->CenterY = PreviewImage->ActualHeight / 2;
			TextOverlay->RenderTransform = rotateTransform;

			isAngleDetected = true;
		}

		// Iterate over recognized lines of text.
		for (auto line : ocrResult->Lines)
		{
			// Iterate over words in line.
			for (auto word : line->Words)
			{
				// Define the TextBlock.
				auto wordTextBlock = ref new TextBlock();
				wordTextBlock->Text = word->Text;
				wordTextBlock->Style = (Windows::UI::Xaml::Style^) this->Resources->Lookup("ExtractedWordTextStyle");

				WordOverlay^ wordBoxOverlay = ref new WordOverlay(word);

				// Keep references to word boxes.
				wordBoxes.push_back(wordBoxOverlay);

				// Define position, background, etc.
				auto overlay = ref new Border();
				overlay->Style = (Windows::UI::Xaml::Style^) this->Resources->Lookup("HighlightedWordBoxHorizontalLine");

				// Bind word boxes to UI.
				overlay->SetBinding(FrameworkElement::MarginProperty, wordBoxOverlay->CreateWordPositionBinding());
				overlay->SetBinding(FrameworkElement::WidthProperty, wordBoxOverlay->CreateWordWidthBinding());
				overlay->SetBinding(FrameworkElement::HeightProperty, wordBoxOverlay->CreateWordHeightBinding());
				//wordTextBlock->FontSize = max((int)(word->BoundingRect.Height*PreviewControl->ActualHeight / (*bitmap)->PixelHeight), 1);
				int heig =(int) (word->BoundingRect.Height* (PreviewControl->ActualHeight * wstring_height(wordTextBlock->Text->Data()) /(*bitmap)->PixelHeight) );
				wordTextBlock->FontSize = max(heig, 1);
				overlay->Child = wordTextBlock;
				overlay->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &CutTheWords::Views::CameraPage::OnTapped);
				// Put the filled textblock in the results grid.
				TextOverlay->Children->Append(overlay);
			}
		}

		UpdateWordBoxTransform();

		ExtractButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		slider->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		CameraButton->Visibility = Windows::UI::Xaml::Visibility::Visible;

		Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, ocrEngine]()
		{
			//ShowMsg(("Image processed using " + ocrEngine->RecognizerLanguage->DisplayName + " language.")->Data());
			//rootPage->NotifyUser("Image processed using " + ocrEngine->RecognizerLanguage->DisplayName + " language.", NotifyType::StatusMessage);
		}));
	});
}

/// <summary>
/// This is event handler for 'Camera' button.
/// Clears previous OCR results and starts camera.
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CameraPage::CameraButton_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	create_task(StartCameraAsync()).then([this](task<void> prevTask)
	{
		Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
		{
			if (mediaCapture->VideoDeviceController->ZoomControl->Supported) {
				auto tmp = mediaCapture->VideoDeviceController->ZoomControl;
				slider->Maximum = tmp->Max;
				slider->Minimum = tmp->Min;
				float stp = tmp->Step;
				if (stp < 0.01)stp = ((int)(0.01 / stp))*stp;
				slider->SmallChange = stp;
				slider->StepFrequency = stp;
				slider->TickFrequency = 1;
				slider->Value = mediaCapture->VideoDeviceController->ZoomControl->Value;

			}
		}));
	});
}

/// <summary>
///  Update word box transform to match current UI size.
/// </summary>
void CameraPage::UpdateWordBoxTransform()
{
	if (PreviewImage->Source != nullptr)
	{
		WriteableBitmap^ bitmap = safe_cast<WriteableBitmap^>(PreviewImage->Source);

		// Used for text overlay.
		// Prepare scale transform for words since image is not displayed in original size.
		ScaleTransform^ scaleTrasform = ref new ScaleTransform();
		scaleTrasform->CenterX = 0;
		scaleTrasform->CenterY = 0;
		scaleTrasform->ScaleX = PreviewImage->ActualWidth / bitmap->PixelWidth;
		scaleTrasform->ScaleY = PreviewImage->ActualHeight / bitmap->PixelHeight;

		for (auto& box : wordBoxes)
		{
			box->Transform(scaleTrasform);
		}
	}
}

/// <summary>
/// Occures when displayed image size changes.
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void CameraPage::PreviewImage_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	UpdateWordBoxTransform();

	// Update image rotation center.
	if (isAngleDetected)
	{
		const auto& rotate = static_cast<RotateTransform^>(TextOverlay->RenderTransform);

		rotate->CenterX = PreviewImage->ActualWidth / 2;
		rotate->CenterY = PreviewImage->ActualHeight / 2;
	}
}

#pragma region MediaCapture

/// <summary>
/// Starts the camera. Initializes resources and starts preview.
/// </summary>
task<void> CameraPage::StartCameraAsync()
{
	auto cameraTask = create_empty_task();

	if (!isInitialized)
	{
		cameraTask = cameraTask.then([this]()
		{
			return InitializeCameraAsync();
		});
	}

	cameraTask.then([this](task<void> initTask)
	{
		try
		{
			initTask.get();
		}
		catch (Exception^ e)
		{
			// Use the dispatcher because this method is sometimes called from non-UI threads.
			auto msg = make_shared<String^>(e->Message);
			Dispatcher->RunAsync(
				Windows::UI::Core::CoreDispatcherPriority::High,
				ref new Windows::UI::Core::DispatchedHandler([this, msg]()
			{
				ShowMsg((*msg)->Data());
				//rootPage->NotifyUser(*msg, NotifyType::ErrorMessage);
			}));
		}

		if (isInitialized)
		{
			// Use the dispatcher because this method is sometimes called from non-UI threads.
			Dispatcher->RunAsync(
				Windows::UI::Core::CoreDispatcherPriority::High,
				ref new Windows::UI::Core::DispatchedHandler([this]()
			{
				TextOverlay->Children->Clear();
				wordBoxes.clear();

				PreviewImage->Source = nullptr;

				PreviewControl->Visibility = Windows::UI::Xaml::Visibility::Visible;
				ImageGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

				ExtractButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
				slider->Visibility = Windows::UI::Xaml::Visibility::Visible;
				CameraButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
				//rootPage->NotifyUser("Camera started.", NotifyType::StatusMessage);
			}));
		}
	});

	return cameraTask;
}

/// <summary>
/// Initializes the MediaCapture, registers events, gets camera device information for mirroring and rotating, and starts preview.
/// </summary>
task<void> CameraPage::InitializeCameraAsync()
{
	if (mediaCapture.Get() != nullptr)
	{
		return create_empty_task();
	}
	// Attempt to get the back camera if one is available, but use any camera device if not.
	return create_task(FindCameraDeviceByPanelAsync(Windows::Devices::Enumeration::Panel::Back))
		.then([this](DeviceInformation^ cameraDevice)
	{
		if (cameraDevice == nullptr)
		{
			throw ref new Platform::Exception(E_ABORT, L"No camera device.");
		}

		// Figure out where the camera is located.
		if (cameraDevice->EnclosureLocation == nullptr || cameraDevice->EnclosureLocation->Panel == Windows::Devices::Enumeration::Panel::Unknown)
		{
			// No information on the location of the camera, assume it's an external camera, not integrated on the device.
			externalCamera = true;
		}
		else
		{
			// Camera is fixed on the device.
			externalCamera = false;

			// Only mirror the preview if the camera is on the front panel.
			mirroringPreview = (cameraDevice->EnclosureLocation->Panel == Windows::Devices::Enumeration::Panel::Front);
		}

		auto settings = ref new MediaCaptureInitializationSettings();
		settings->VideoDeviceId = cameraDevice->Id;

		// Create MediaCapture and its settings.
		mediaCapture = ref new MediaCapture();

		//mediaCaptureFailedEventToken = mediaCapture->Failed += ref new Windows::Media::Capture::MediaCaptureFailedEventHandler(this, &SDKTemplate::CameraPage::MediaCapture_Failed);

		// Initialize MediaCapture.
		return mediaCapture->InitializeAsync(settings);
	}).then([this]()
	{
		if (mediaCapture->VideoDeviceController->ZoomControl->Supported) {
			//mediaCapture->VideoDeviceController->
			//auto x = mediaCapture->VideoDeviceController->Zoom->Capabilities->Max;
			auto tmp = mediaCapture->VideoDeviceController->ZoomControl;
			mediaCapture->VideoDeviceController->ZoomControl->Value = tmp->Min + ((int)((tmp->Max - tmp->Min) / tmp->Step / 3))*tmp->Step;
			//mediaCapture->VideoDeviceController->Zoom->TrySetValue(mediaCapture->VideoDeviceController->Zoom->Capabilities->Max);
		}


		/*
		auto focusSettings = ref new Windows::Media::Devices::FocusSettings();
		focusSettings->AutoFocusRange = Windows::Media::Devices::AutoFocusRange::FullRange;
		focusSettings->Mode = Windows::Media::Devices::FocusMode::Auto;
		focusSettings->WaitForFocus = true;
		focusSettings->DisableDriverFallback = false;
		mediaCapture->VideoDeviceController->FocusConarol->Configure(focusSettings);*/
		isInitialized = true;
		Camera_init = true;
		// If initialization succeeded, start the preview.
		return StartPreviewAsync();
	});
	
}

/// <summary>
/// Starts the preview and adjusts it for for rotation and mirroring after making a request to keep the screen on and unlocks the UI.
/// </summary>
task<void> CameraPage::StartPreviewAsync()
{
	// Prevent the device from sleeping while the preview is running.
	displayRequest->RequestActive();

	return create_task([this]()
	{
		// Return task to make sure PreviewControl->Source is set before StartPreviewAsync.
		return Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
		{
			// Set the preview source in the UI and mirror it if necessary.
			PreviewControl->Source = mediaCapture.Get();
			PreviewControl->FlowDirection = mirroringPreview ? ::FlowDirection::RightToLeft : ::FlowDirection::LeftToRight;
		}));
	}).then([this]()
	{
		// Start the preview.
		return mediaCapture->StartPreviewAsync();
	}).then([this]()
	{
		isPreviewing = true;

		// Initialize the preview to the current orientation.
		return SetPreviewRotationAsync();
	});
}

/// <summary>
/// Gets the current orientation of the UI in relation to the device and applies a corrective rotation to the preview.
/// </summary>
task<void> CameraPage::SetPreviewRotationAsync()
{
	// Only need to update the orientation if the camera is mounted on the device.
	if (externalCamera)
	{
		return create_empty_task();
	}

	// Calculate which way and how far to rotate the preview.
	return create_task(CalculatePreviewRotation()).then([this]()
	{
		// Set preview rotation in the preview source.
		mediaCapture->SetPreviewRotation(sourceRotation);

		// Rotation metadata to apply to the preview stream (MF_MT_VIDEO_ROTATION).
		// Reference: http://msdn.microsoft.com/en-us/library/windows/apps/xaml/hh868174.aspx
		static const GUID RotationKey = { 0xC380465D, 0x2271, 0x428C,{ 0x9B, 0x83, 0xEC, 0xEA, 0x3B, 0x4A, 0x85, 0xC1 } };

		// Add rotation metadata to the preview stream to make sure the aspect ratio / dimensions match when rendering and getting preview frames.
		const auto& props = mediaCapture->VideoDeviceController->GetMediaStreamProperties(MediaStreamType::VideoPreview);
		props->Properties->Insert(RotationKey, rotationDegrees);

		return mediaCapture->SetEncodingPropertiesAsync(MediaStreamType::VideoPreview, props, nullptr);
	});;
}

/// <summary>
/// Stops the preview and deactivates a display request, to allow the screen to go into power saving modes, and locks the UI
/// </summary>
/// <returns></returns>
task<void> CameraPage::StopPreviewAsync()
{
	isPreviewing = false;
	return create_task(mediaCapture->StopPreviewAsync()).then([this]()
	{
		// Use the dispatcher because this method is sometimes called from non-UI threads
		return Dispatcher->RunAsync(
			Windows::UI::Core::CoreDispatcherPriority::High,
			ref new Windows::UI::Core::DispatchedHandler([this]()
		{
			PreviewControl->Source = nullptr;

			// Allow the device to sleep now that the preview is stopped
			displayRequest->RequestRelease();
		}));
	}).then([this](task<void> t)
	{
		try
		{
			t.get();
		}
		catch (Exception^ e)
		{
			auto msg = make_shared<String^>(e->Message);
			Dispatcher->RunAsync(
				Windows::UI::Core::CoreDispatcherPriority::High,
				ref new Windows::UI::Core::DispatchedHandler([this, msg]()
			{
				ShowMsg(("Exception stopping preview. " + (*msg))->Data());
				//rootPage->NotifyUser("Exception stopping preview. " + (*msg), NotifyType::ErrorMessage);
			}));
		}
	});
}

/// <summary>
/// Cleans up the camera resources (after stopping the preview if necessary) and unregisters from MediaCapture events.
/// </summary>
task<void> CameraPage::CleanupCameraAsync()
{
	auto cleanUpTask = create_empty_task();

	if (isInitialized)
	{
		if (isPreviewing)
		{
			// The call to stop the preview is included here for completeness, but can be
			// safely removed if a call to MediaCapture.Dispose() is being made later,
			// as the preview will be automatically stopped at that point
			cleanUpTask = cleanUpTask.then([this]()
			{
				return StopPreviewAsync();
			});
		}
	}

	cleanUpTask = cleanUpTask.then([this]()
	{
		isInitialized = false;
		Camera_init = false;
		if (mediaCapture.Get() != nullptr)
		{
			mediaCapture->Failed -= mediaCaptureFailedEventToken;
			delete(mediaCapture.Get());
			mediaCapture = nullptr;
		}
	});

	return cleanUpTask;
}

/// <summary>
/// Queries the available video capture devices to try and find one mounted on the desired panel.
/// </summary>
/// <param name="desiredPanel">The panel on the device that the desired camera is mounted on.</param>
/// <returns>A DeviceInformation instance with a reference to the camera mounted on the desired panel if available,
///          any other camera if not, or null if no camera is available.</returns>
task<DeviceInformation^> CameraPage::FindCameraDeviceByPanelAsync(Windows::Devices::Enumeration::Panel desiredPanel)
{
	// Get available devices for capturing pictures.
	return create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
		.then([this, desiredPanel](DeviceInformationCollection^ allVideoDevices) -> DeviceInformation^
	{
		if (allVideoDevices == nullptr || allVideoDevices->Size == 0)
		{
			return nullptr;
		}

		// Get the desired camera by panel.
		for (const auto& device : allVideoDevices)
		{
			if (device->EnclosureLocation != nullptr && device->EnclosureLocation->Panel == desiredPanel)
			{
				return device;
			}
		}

		// If there is no device mounted on the desired panel, return the first device found.
		return allVideoDevices->First()->Current;
	});
}

/// <summary>
/// Reads the current orientation of the app and calculates the VideoRotation necessary to ensure the preview is rendered in the correct orientation.
/// </summary>
task<void> CameraPage::CalculatePreviewRotation()
{
	return create_task(Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
	{
		// Note that in some cases, the rotation direction needs to be inverted if the preview is being mirrored.
		switch (displayInformation->CurrentOrientation)
		{
		case DisplayOrientations::Portrait:
			if (mirroringPreview)
			{
				rotationDegrees = 270;
				sourceRotation = VideoRotation::Clockwise270Degrees;
			}
			else
			{
				rotationDegrees = 90;
				sourceRotation = VideoRotation::Clockwise90Degrees;
			}
			break;

		case DisplayOrientations::LandscapeFlipped:
			// No need to invert this rotation, as rotating 180 degrees is the same either way.
			rotationDegrees = 180;
			sourceRotation = VideoRotation::Clockwise180Degrees;
			break;

		case DisplayOrientations::PortraitFlipped:
			if (mirroringPreview)
			{
				rotationDegrees = 90;
				sourceRotation = VideoRotation::Clockwise90Degrees;
			}
			else
			{
				rotationDegrees = 270;
				sourceRotation = VideoRotation::Clockwise270Degrees;
			}
			break;

		case DisplayOrientations::Landscape:
		default:
			rotationDegrees = 0;
			sourceRotation = VideoRotation::None;
			break;
		}
	})));
}

void CameraPage::MediaCapture_Failed(MediaCapture^ sender, MediaCaptureFailedEventArgs^ errorEventArgs)
{
	auto msg = make_shared<String^>(errorEventArgs->Message);

	create_task(CleanupCameraAsync()).then([this, msg]
	{
		Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, msg]()
		{
			ExtractButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			slider->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			CameraButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
			ShowMsg(( (*msg))->Data());
			//rootPage->NotifyUser(*msg, NotifyType::ErrorMessage);
		}));
	});
}

#pragma endregion MediaCapture


void CutTheWords::Views::CameraPage::OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	auto tmp=dynamic_cast<Border^>(sender);
	if (tmp != nullptr) {
		auto tmp2 = dynamic_cast<TextBlock^>(tmp->Child);
		if (tmp2 != nullptr) {
			wstring s = tmp2->Text->Data();
			if (s == L"")return;
			for (int i = 0;s[i];i++)
				if ((s[i] | 32) >= 'a' && (s[i] | 32) <= 'z') {
					s = s.substr(i);
					break;
				}
			for (int i = (int)s.length() - 1;~i;i--)
				if ((s[i] | 32) >= 'a' && (s[i] | 32) <= 'z') {
					s = s.substr(0, i + 1);
					break;
				}
			if ((s[0] | 32) < 'a' || (s[0] | 32) > 'z')
				return;
			Frame->Navigate(
				TypeName(SingleVocPage::typeid),
				ref new String(s.c_str()),
				ref new Windows::UI::Xaml::Media::Animation::DrillInNavigationTransitionInfo());
		}
	}
	//throw ref new Platform::NotImplementedException();
}

//float Camera_Final_Zoom = 1;
//bool Camera_Loading = 0;
void CutTheWords::Views::CameraPage::slider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	
	if (isInitialized&&mediaCapture->VideoDeviceController->ZoomControl->Supported) {
		//Camera_Final_Zoom = (float)slider->Value;
		//create_task([this]() {
			mediaCapture->VideoDeviceController->ZoomControl->Value = (float)slider->Value;
		//});
	}
}
