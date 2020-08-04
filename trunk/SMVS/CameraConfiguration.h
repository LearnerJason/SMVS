#pragma once

#include <pylon/ConfigurationEventHandler.h>
#include "SystemManager.h"
#include "Logger.h"

extern CSystemManager gSystemMgr;
extern CLogger        gLogger;

namespace Pylon
{
    class CInstantCamera;
}

class CameraConfigEventHandler : public Pylon::CConfigurationEventHandler
{
public: 
	CameraConfigEventHandler()
	{
	}
// 	void OnOpened( Pylon::CInstantCamera& camera) 
// 	{
// 		try
// 		{
//             // Allow all the names in the namespace GenApi to be used without qualification.
//             using namespace GenApi;
// 
//             // Get the camera control object.
//             INodeMap &control = camera.GetNodeMap();
// 
//             // Get the parameters for setting the image area of interest (Image AOI).
//             const CIntegerPtr width = control.GetNode("Width");
//             const CIntegerPtr height = control.GetNode("Height");
//             const CIntegerPtr offsetX = control.GetNode("OffsetX");
//             const CIntegerPtr offsetY = control.GetNode("OffsetY");
// 
//             // Maximize the Image AOI.
//             if (IsWritable(offsetX))
//             {
//                 offsetX->SetValue(offsetX->GetMin());
//             }
//             if (IsWritable(offsetY))
//             {
//                 offsetY->SetValue(offsetY->GetMin());
//             }
//             width->SetValue(width->GetMax());
//             height->SetValue(height->GetMax());  

// 			// Get the Packet Size for GigE cameras
// 			const CIntegerPtr packetSizeGigE= control.GetNode("GevSCPSPacketSize");
// 			// Set the Packet Size to 1500 by default.
// 			// If your network adapter and/or switch supported Jumbo Frames,
// 			// you may consider using higher values for Packet Size, e.g. 3000, 6000, 9000.
// 			// By rule of thumb, the bigger the Packet Size- the better the overall performance.
//             if (IsWritable(packetSizeGigE))
//             {
// 				packetSizeGigE->SetValue(3000);
// 			}
// 
//         }
//         catch (GenICam::GenericException& e)
//         {
//             throw RUNTIME_EXCEPTION( "Could not apply configuration. GenICam::GenericException caught in OnOpened method msg=%hs", e.what());
//         }
//     }

	void OnGrabError(CInstantCamera& camera, const char* errorMessage)
	{
		// Do not handle errors caused by device removal.
		if (camera.IsCameraDeviceRemoved())
		{
			string message = "An error occurred during grabbing = ";
			message+= errorMessage;
			USES_CONVERSION;
			MessageBox(NULL, A2CT(message.c_str()), _T("CBaslerCamera::OnGrabError"), MB_OK | MB_ICONEXCLAMATION);
		}
	}

	void OnCameraDeviceRemoved( CInstantCamera& camera)
	{
		// The WM_DEVICE_REMOVED message will call CLiveViewPylonDlg::OnDeviceRemoved,
		// informing the GUI thread, that the GUI can now be updated.
		// Stop the grab; close the pylon device; destroy the pylon device.
		// DestroyDevice does not throw C++ exceptions.

		auto deviceInfo = camera.GetDeviceInfo();
		auto cameraname = deviceInfo.GetFriendlyName();
		wstring wstrCameraName = utils::Utf8ToUnicode(cameraname.c_str());
		std::wostringstream oss;
		oss << L"Connection to camera  " << wstrCameraName << L"  lost !";
		wstring wstrMsg = oss.str();	
		gSystemMgr.OnCameraConnectionLostEvent(wstrCameraName);
		gLogger.QueueUpLog(wstrMsg);
		MessageBox(NULL, wstrMsg.c_str(), _T("Camera Error"), MB_OK | MB_ICONEXCLAMATION);

		camera.DestroyDevice();
		
	}
private:
	int m_nChIndex;
};

