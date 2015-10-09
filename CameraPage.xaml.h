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

#pragma once

#include "CameraPage.g.h"
//#include "MainPage.xaml.h"
#include "WordOverlay.h"

#include <ppl.h>
#include <Voc.h>

using namespace Concurrency;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Graphics::Display;
using namespace Windows::Media::Capture;
using namespace Windows::System::Display;

namespace CutTheWords
{
	namespace Views
	{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class CameraPage sealed
		{
		public:
			CameraPage();

		protected:
			virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
			virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

		private:

			bool isAngleDetected;

			// Language for OCR.
			Windows::Globalization::Language^ ocrLanguage;

			// Recognized words ovelay boxes.
			std::vector<WordOverlay^> wordBoxes;

			// Receive notifications about rotation of the UI and apply any necessary rotation to the preview stream.
			DisplayInformation^ displayInformation = DisplayInformation::GetForCurrentView();

			// Prevent the screen from sleeping while the camera is running.
			DisplayRequest^ displayRequest = ref new DisplayRequest();

			// MediaCapture and its state variables.
			Platform::Agile<MediaCapture> mediaCapture;
			bool isInitialized = false;
			bool isPreviewing = false;

			// Information about the camera device.
			bool mirroringPreview = false;
			bool externalCamera = false;

			int rotationDegrees;
			VideoRotation sourceRotation;

			// Event handler tokens.
			Windows::Foundation::EventRegistrationToken orientationChangedEventToken;
			Windows::Foundation::EventRegistrationToken mediaCaptureFailedEventToken;

			void UpdateWordBoxTransform();
			task<void> CalculatePreviewRotation();

			task<void> StartCameraAsync();
			task<void> StartPreviewAsync();
			task<void> SetPreviewRotationAsync();
			task<void> InitializeCameraAsync();
			task<void> StopPreviewAsync();
			task<void> CleanupCameraAsync();
			task<DeviceInformation^> FindCameraDeviceByPanelAsync(Windows::Devices::Enumeration::Panel desiredPanel);

			void PreviewImage_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
			void ExtractButton_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
			void CameraButton_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
			void DisplayInformation_OrientationChanged(Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args);
			void MediaCapture_Failed(MediaCapture^ sender, MediaCaptureFailedEventArgs^ errorEventArgs);
			void Application_Suspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
			void Application_Resuming(Platform::Object^ sender, Platform::Object^ o);
			void OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
			void slider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		};
	}
}
