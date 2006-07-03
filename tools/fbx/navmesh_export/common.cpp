/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#include <fbxsdk.h>
#include "common.hpp"


void InitializeSdkObjects(KFbxSdkManager*& pSdkManager, KFbxScene*& pScene)
{
    // The first thing to do is to create the FBX SDK manager which is the
	// object allocator for almost all the classes in the SDK.
    // Only one FBX SDK manager can be created. Any subsequent call to
    // KFbxSdkManager::CreateKFbxSdkManager() will return NULL.
    pSdkManager = KFbxSdkManager::CreateKFbxSdkManager();

	if (!pSdkManager)
	{
		printf("Unable to create the FBX SDK manager, the license may be expired\n");
		exit(0);
	}

	// Create the entity that will hold the scene.
    pScene = pSdkManager->CreateKFbxScene();

}

void DestroySdkObjects(KFbxSdkManager* pSdkManager)
{
    // Delete the FBX SDK manager. All the objects that have been allocated
	// using the FBX SDK manager and that haven't been explicitely destroyed
	// are automatically destroyed at the same time.
    delete pSdkManager;
}

bool SaveScene(KFbxSdkManager* pSdkManager, KFbxScene* pScene, const char* pFilename, bool pSaveAsBinary, bool pEmbedMedia, bool pSaveAsVersion5)
{
	int lMajor, lMinor, lRevision;

	// Create an exporter.
    KFbxExporter* lExporter = pSdkManager->CreateKFbxExporter();

    // Initialize the exporter by providing a filename.
	if(lExporter->Initialize(pFilename) == false)
	{
		printf("Call to KFbxExporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lExporter->GetLastErrorString());
		return false;
	}

	KFbxIO::GetCurrentVersion(lMajor, lMinor, lRevision);
	printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n\n", lMajor, lMinor, lRevision);

    // Set the export states. By default, the export states are always set to
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below
	// shows how to change these states.
    lExporter->SetState(KFbxExporter::eEXPORT_MATERIAL, true);
    lExporter->SetState(KFbxExporter::eEXPORT_TEXTURE, true);
	lExporter->SetState(KFbxExporter::eEXPORT_TEXTURE_AS_EMBEDDED, pEmbedMedia);
    lExporter->SetState(KFbxExporter::eEXPORT_LINK, true);
    lExporter->SetState(KFbxExporter::eEXPORT_SHAPE, true);
    lExporter->SetState(KFbxExporter::eEXPORT_GOBO, true);
    lExporter->SetState(KFbxExporter::eEXPORT_ANIMATION, true);
    lExporter->SetState(KFbxExporter::eEXPORT_GLOBAL_SETTINGS, true);


	// Set the file mode to binary
	if (pSaveAsVersion5 == false)
		// use the utility function to save in binary or ascii
		lExporter->SetFileModeBinary(pSaveAsBinary);
	else
	{
		// use the SetFileFormat method to specifiy ascii or binary.
		// This is the typical way to proceed.
		lExporter->SetFileFormat((pSaveAsBinary) ? KFbxExporter::eFBX_50_BINARY : KFbxExporter::eFBX_50_ASCII);
	}

    // Export the scene.
    bool lStatus = lExporter->Export(*pScene);

	// Destroy the exporter.
	pSdkManager->DestroyKFbxExporter(lExporter);

	return lStatus;
}

bool LoadScene(KFbxSdkManager* pSdkManager, KFbxScene* pScene, const char* pFilename)
{
	int lMajor, lMinor, lRevision;
	int i, lTakeCount;
	KString lCurrentTakeName;
	bool lStatus;
	char lPassword[1024];

	// Create an importer.
    KFbxImporter* lImporter = pSdkManager->CreateKFbxImporter();
	lImporter->SetFileFormat(GetFileFormat(pFilename));

    // Initialize the importer by providing a filename.
	if(lImporter->Initialize(pFilename) == false)
	{
		printf("Call to KFbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());

		if (lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_YET ||
			lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_ANYMORE)
		{
			KFbxIO::GetCurrentVersion(lMajor, lMinor, lRevision);
			printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n", lMajor, lMinor, lRevision);

			lImporter->GetFileVersion(lMajor, lMinor, lRevision);
			printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lMajor, lMinor, lRevision);
		}

		return false;
	}

	KFbxIO::GetCurrentVersion(lMajor, lMinor, lRevision);
	printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n", lMajor, lMinor, lRevision);

	bool isFbx = lImporter->GetFileFormat() == KFbxImporter::eFBX_ENCRYPTED ||
			     lImporter->GetFileFormat() == KFbxImporter::eFBX_ASCII ||
				 lImporter->GetFileFormat() == KFbxImporter::eFBX_BINARY;

	if (isFbx)
	{
		lImporter->GetFileVersion(lMajor, lMinor, lRevision);
		printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lMajor, lMinor, lRevision);

		// From this point, it is possible to access take information without
		// the expense of loading the entire file.

		printf("Take Information\n");

		lTakeCount = lImporter->GetTakeCount();

		printf("    Number of takes: %d\n", lTakeCount);
		printf("    Current take: \"%s\"\n", lImporter->GetCurrentTakeName());
		printf("\n");

		for(i = 0; i < lTakeCount; i++)
		{
			KFbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			printf("    Take %d\n", i);
			printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the take should be imported
			// under a different name.
			printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the take should be not
			// be imported.
			printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			printf("\n");
		}

		// Set the import states. By default, the import states are always set to
		// true. The code below shows how to change these states.
		lImporter->SetState(KFbxImporter::eIMPORT_MATERIAL, true);
		lImporter->SetState(KFbxImporter::eIMPORT_TEXTURE, true);
		lImporter->SetState(KFbxImporter::eIMPORT_LINK, true);
		lImporter->SetState(KFbxImporter::eIMPORT_SHAPE, true);
		lImporter->SetState(KFbxImporter::eIMPORT_GOBO, true);
		lImporter->SetState(KFbxImporter::eIMPORT_ANIMATION, true);
		lImporter->SetState(KFbxImporter::eIMPORT_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(*pScene);

	if(lStatus == false && lImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
	{
		printf("Please enter password: ");

		lPassword[0] = '\0';

		scanf("%s", lPassword);

		lImporter->SetPassword(lPassword);

		lStatus = lImporter->Import(*pScene);

		if(lStatus == false && lImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
		{
			printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	pSdkManager->DestroyKFbxImporter(lImporter);

	return lStatus;
}

KFbxImporter::EFileFormat GetFileFormat(const char* pFilename)
{
	KFbxImporter::EFileFormat lFileFormat = KFbxImporter::eFBX_BINARY;
	KString lFileName(pFilename);
	int     lIndex;

	// Find the '.' separating the file extension
	if ((lIndex = lFileName.ReverseFind('.')) > -1)
	{
		// extension found
		KString ext = lFileName.Right(lFileName.GetLen() - lIndex - 1);
		if (ext.CompareNoCase("obj") == 0)
		{
			lFileFormat = KFbxImporter::eALIAS_OBJ;
		}
		else
		if (ext.CompareNoCase("3ds") == 0)
		{
			lFileFormat = KFbxImporter::e3D_STUDIO_3DS;
		}
		else
		if (ext.CompareNoCase("dxf") == 0)
		{
			lFileFormat = KFbxImporter::eAUTOCAD_DXF;
		}
#ifdef ARUBA
		else
		if (ext.CompareNoCase("apf") == 0)
		{
			lFileFormat = KFbxImporter::eALIAS_APF;
		}
#endif
	}

	return lFileFormat;
}



