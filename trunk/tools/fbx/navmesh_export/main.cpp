#include <fbxsdk.h>
#include "common.hpp"
#include "DisplayMesh.hpp"
#include "SaveNavMesh.hpp"

void SaveContent(KFbxScene* scene, float size, float snap);
void SaveContent(KFbxNode* node, float size, float snap);
void DisplayContent(KFbxScene* scene);
void DisplayContent(KFbxNode* node);
void TriangulateContent(KFbxScene* scene, KFbxGeometryConverter* converter);
void TriangulateContent(KFbxNode* node, KFbxGeometryConverter* converter);

int main(int argc, char** argv) {
	KFbxSdkManager* sdkManager = NULL;
	KFbxScene* scene = NULL;
	KFbxGeometryConverter* converter;
	bool result;

	// Load the scene.
	// Take a FBX file as an argument.
	if(argc <= 3) {
		printf("\n\nUsage: ImportScene <size> <snap> <FBX file name>\n\n");
	}

	// Prepare the FBX SDK.
	InitializeSdkObjects(sdkManager, scene);

	for(int i = 3; i < argc; ++i) {
		printf("\n\nFile: %s", argv[i]);
		result = LoadScene(sdkManager, scene, argv[i]);

		if(result == false) {
			printf("\n\nAn error occured while loading file %s", argv[i]);
			DestroySdkObjects(sdkManager);
			return 0;
		}

		// Triangulate
		converter = sdkManager->CreateKFbxGeometryConverter();
		TriangulateContent(scene, converter);
		sdkManager->DestroyKFbxGeometryConverter(converter);

		// Save
		float size = atoi(argv[1]);
		float snap = atof(argv[2]);
		SaveContent(scene, size, snap);
	}

	DestroySdkObjects(sdkManager);
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


void SaveContent(KFbxScene* scene, float size, float snap) {
	int i;
	KFbxNode* node = scene->GetRootNode();

	if(node) {
		for(i = 0; i < node->GetChildCount(); i++) {
			SaveContent(node->GetChild(i), size, snap);
		}
	}
}


void SaveContent(KFbxNode* node, float size, float snap) {
	KFbxNodeAttribute::EAttributeType attributeType;
	int i;

    if(node->GetNodeAttribute() == NULL)
    {
		printf("NULL Node Attribute\n\n");
    }
	else
    {
		puts(node->GetName());
		attributeType = (node->GetNodeAttribute()->GetAttributeType());
		switch (attributeType) {
			case KFbxNodeAttribute::eMESH:
				SaveNavMesh(node, size, snap);
				SaveOgre(node, size, snap);
				break;
	    }
	}
}
