/**************************************************************************************

 Based on ImportScene sample in FBXSDK2005.12.
 Copyright © 2001 - 2006 Autodesk, Inc. and/or its licensors.

 Modified by Rune Myrland.

**************************************************************************************/

#include <fbxsdk.h>

#include "Common.h"
#include <fbxfilesdk_nsuse.h>

// Local function prototypes.
void DisplayContent(KFbxScene* pScene);
void DisplayContent(KFbxNode* pNode);
void DisplayMetaData(KFbxScene* pScene);

void DisplayMyCurve(KFCurve *pCurve);
void DisplayMyChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode);

FILE* OUTFILE;

int main(int argc, char** argv)
{
	KFbxSdkManager* lSdkManager = NULL;
	KFbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);

	// Load the scene.

	// The example can take a FBX file as an argument.
	if(argc > 2)
	{
		lResult = LoadScene(lSdkManager, lScene, argv[1]);

		if(lResult) {
			OUTFILE = fopen(argv[2], "wt");
			if(OUTFILE == 0) {
				fprintf(stderr, "Could not open output file: %s\n", argv[2]);
				lResult = false;
			}
		}
		
	}
	else
	{
		lResult = false;
		fprintf(stderr, "Usage: fbx2areaThings <FBX file name> <output filename>\n");
	}

	if(lResult == false)
	{
		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(lSdkManager);
		fprintf(stderr, "An error occured while loading the scene...\n");
		return 0;
	}
	else 
	{

		fprintf(OUTFILE, "XB01\n");
		KString filename(argv[1]);
		filename.FindAndReplace(".fbx", "", filename.GetLen() - 4);
		
		fprintf(OUTFILE, "N %s\n", filename.GetBuffer());
		DisplayContent(lScene);
		fprintf(OUTFILE, "\nQ\n");
	}

	// Destroy all objects created by the FBX SDK.
	DestroySdkObjects(lSdkManager);
	
	return 0;
}


void DisplayContent(KFbxScene* pScene)
{
	int i;
	KFbxNode* lNode = pScene->GetRootNode();

	if(lNode)
	{
		for(i = 0; i < lNode->GetChildCount(); i++)
		{
			DisplayContent(lNode->GetChild(i));
		}
	}
}


void DisplayContent(KFbxNode* pNode)
{
	KFbxNodeAttribute::EAttributeType lAttributeType;
	int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
		fprintf(OUTFILE, "NULL Node Attribute\n\n");
    }
	else
    {
		KString name(pNode->GetName());

		int from = 0;
		while(name.Find("::", from + 1) > -1) {
			from = name.Find("::", from + 1);
		}

		int to = from;
		while(name.Find("__", to + 1) > -1) {
			to = name.Find("__", to + 1);
		}
		if(to == from) {
			return;
		}

		fprintf(OUTFILE, "\nA %s\n", name.Mid(from + 2, to - from - 2).GetBuffer());

		//DisplayUserProperties(pNode);

		// Display default animation
		KFbxTakeNode* lDefaultTakeNode = pNode->GetDefaultTakeNode();

		if(lDefaultTakeNode) {
			DisplayMyChannels(pNode, lDefaultTakeNode);
		}

		fprintf(OUTFILE, "/\n");
	}


    for(i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}


void DisplayMyCurve(KFCurve *pCurve)
{
    KString lOutputString;

    lOutputString = " ";
    lOutputString += static_cast<float> (pCurve->GetValue());
    fprintf(OUTFILE, lOutputString);
}


void DisplayMyChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode)
{
	KFCurve* lCurve = NULL;

	fprintf(OUTFILE, "# transform\nT ");
	lCurve = pTakeNode->GetTranslationX();
	DisplayMyCurve(lCurve);
	
	lCurve = pTakeNode->GetTranslationY();
	DisplayMyCurve(lCurve);

	lCurve = pTakeNode->GetTranslationZ();
	DisplayMyCurve(lCurve);

	fprintf(OUTFILE, "\n# rotate\nR ");
	lCurve = pTakeNode->GetEulerRotationY();
	DisplayMyCurve(lCurve);

	lCurve = pTakeNode->GetEulerRotationX();
	DisplayMyCurve(lCurve);

	lCurve = pTakeNode->GetEulerRotationZ();
	DisplayMyCurve(lCurve);
 
	fprintf(OUTFILE, "\n# scale\nS ");
	lCurve = pTakeNode->GetScaleX();
	DisplayMyCurve(lCurve);

	/*
	lCurve = pTakeNode->GetScaleY();
	DisplayMyCurve(lCurve);

	lCurve = pTakeNode->GetScaleZ();
	DisplayMyCurve(lCurve);
	*/

	fprintf(OUTFILE, "\n");
}
