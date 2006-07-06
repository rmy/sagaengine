#include <fbxsdk.h>
#include "common.hpp"
#include "DisplayMesh.hpp"
#include "SaveNavMesh.hpp"

void SaveContent(KFbxScene* scene);
void SaveContent(KFbxNode* node);
void DisplayContent(KFbxScene* scene);
void DisplayContent(KFbxNode* node);
void TriangulateContent(KFbxScene* scene, KFbxGeometryConverter* converter);
void TriangulateContent(KFbxNode* node, KFbxGeometryConverter* converter);

int main(int argc, char** argv) {
	KFbxSdkManager* sdkManager = NULL;
	KFbxScene* scene = NULL;
	KFbxGeometryConverter* converter;
	bool result;

	// Prepare the FBX SDK.
	InitializeSdkObjects(sdkManager, scene);

	// Load the scene.
	// Take a FBX file as an argument.
	if(argc > 1) {
		printf("\n\nFile: %s\n\n", argv[1]);
		result = LoadScene(sdkManager, scene, argv[1]);
	}
	else {
		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(sdkManager);
		result = false;

		printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}

	if(result == false)
	{
		printf("\n\nAn error occured while loading the scene...");
		return 0;
	}

	// Triangulate
	converter = sdkManager->CreateKFbxGeometryConverter();
	TriangulateContent(scene, converter);
	sdkManager->DestroyKFbxGeometryConverter(converter);

	// Dump
	//DisplayContent(scene);
	SaveContent(scene);
}



void TriangulateContent(KFbxScene* scene, KFbxGeometryConverter* converter) {
	int i;
	KFbxNode* node = scene->GetRootNode();

	if(node) {
		for(i = 0; i < node->GetChildCount(); i++) {
			TriangulateContent(node->GetChild(i), converter);
		}
	}
}


void TriangulateContent(KFbxNode* node, KFbxGeometryConverter* converter) {
	KFbxNodeAttribute::EAttributeType attributeType;
	int i;

	attributeType = (node->GetNodeAttribute()->GetAttributeType());

	switch(attributeType) {
	case KFbxNodeAttribute::eNURB:
	case KFbxNodeAttribute::ePATCH:
	case KFbxNodeAttribute::eMESH:
		converter->TriangulateInPlace(node);
		break;
	}

    for(i = 0; i < node->GetChildCount(); i++) {
		TriangulateContent(node->GetChild(i), converter);
	}
}


void DisplayContent(KFbxScene* scene) {
	int i;
	KFbxNode* node = scene->GetRootNode();

	if(node) {
		for(i = 0; i < node->GetChildCount(); i++) {
			DisplayContent(node->GetChild(i));
		}
	}
}


void DisplayContent(KFbxNode* node) {
	KFbxNodeAttribute::EAttributeType attributeType;
	int i;

    if(node->GetNodeAttribute() == NULL)
    {
		printf("NULL Node Attribute\n\n");
    }
	else
    {
		attributeType = (node->GetNodeAttribute()->GetAttributeType());

		switch (attributeType) {
			case KFbxNodeAttribute::eMESH:
				DisplayMesh(node);
				break;
	    }
	}

	//DisplayUserProperties(pNode);
	//DisplayTarget(pNode);
	//DisplayPivotsAndLimits(pNode);
	//DisplayTransformPropagation(pNode);
	//DisplayGeometricTransform(pNode);
	//DisplayDefaultAnimation(pNode);

    for(i = 0; i < node->GetChildCount(); i++)
	{
		DisplayContent(node->GetChild(i));
	}
}


void SaveContent(KFbxScene* scene) {
	int i;
	KFbxNode* node = scene->GetRootNode();

	if(node) {
		for(i = 0; i < node->GetChildCount(); i++) {
			SaveContent(node->GetChild(i));
		}
	}
}


void SaveContent(KFbxNode* node) {
	KFbxNodeAttribute::EAttributeType attributeType;
	int i;

    if(node->GetNodeAttribute() == NULL)
    {
		printf("NULL Node Attribute\n\n");
    }
	else
    {
		attributeType = (node->GetNodeAttribute()->GetAttributeType());

		switch (attributeType) {
			case KFbxNodeAttribute::eMESH:
				SaveNavMesh(node);
				break;
	    }
	}

	//DisplayUserProperties(pNode);
	//DisplayTarget(pNode);
	//DisplayPivotsAndLimits(pNode);
	//DisplayTransformPropagation(pNode);
	//DisplayGeometricTransform(pNode);
	//DisplayDefaultAnimation(pNode);

    for(i = 0; i < node->GetChildCount(); i++)
	{
		DisplayContent(node->GetChild(i));
	}
}
