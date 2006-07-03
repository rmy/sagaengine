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
#include "DisplayCommon.hpp"
#include "DisplayMesh.hpp"
#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayControlsPoints(KFbxMesh* pMesh);
void DisplayPolygons(KFbxMesh* pMesh);


void DisplayMesh(KFbxNode* pNode)
{
	KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute ();

	DisplayString("Mesh Name: ", pNode->GetName());
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);
}


void DisplayControlsPoints(KFbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	KFbxVector4* lControlPoints = pMesh->GetControlPoints();

	DisplayString("    Control Points");

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("        Control Point ", i);
		Display3DVector("            Coordinates: ", lControlPoints[i]);

		for (int j = 0; j < pMesh->GetLayerCount(); j++)
		{
			KFbxLayerElementNormal* leNormals = pMesh->GetLayer(j)->GetNormals();
			if (leNormals)
			{
				if (leNormals->GetMappingMode() == KFbxLayerElement::eBY_CONTROL_POINT)
				{
					char header[100];
					sprintf(header, "            Normal Vector (on layer %d): ", j);
					if (leNormals->GetReferenceMode() == KFbxLayerElement::eDIRECT)
						Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
				}
			}
		}
	}

	DisplayString("");
}


void DisplayPolygons(KFbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	KFbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	DisplayString("    Polygons");

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		DisplayInt("        Polygon ", i);
		int l;

		for (l = 0; l < pMesh->GetLayerCount(); l++)
		{
			KFbxLayerElementPolygonGroup* lePolgrp = pMesh->GetLayer(l)->GetPolygonGroups();
			if (lePolgrp)
			{
				switch (lePolgrp->GetMappingMode())
				{
				case KFbxLayerElement::eBY_POLYGON:
					if (lePolgrp->GetReferenceMode() == KFbxLayerElement::eINDEX)
					{
						sprintf(header, "        Assigned to group (on layer %d): ", l);
						int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
						DisplayInt(header, polyGroupId);
						break;
					}
				default:
					// any other mapping modes don't make sense
					DisplayString("        \"unsupported group assignment\"");
					break;
				}
			}
		}

		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

			for (l = 0; l < pMesh->GetLayerCount(); l++)
			{
				KFbxLayerElementVertexColor* leVtxc = pMesh->GetLayer(l)->GetVertexColors();
				if (leVtxc)
				{
					sprintf(header, "            Color vertex (on layer %d): ", l);

					switch (leVtxc->GetMappingMode())
					{
					case KFbxLayerElement::eBY_CONTROL_POINT:
						switch (leVtxc->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case KFbxLayerElement::eBY_POLYGON_VERTEX:
						{
						switch (leVtxc->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leVtxc->GetIndexArray().GetAt(vertexId);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						}
						break;

					case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for UVs
					case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for UVs
					case KFbxLayerElement::eNONE:       // doesn't make much sense for UVs
						break;
					}
				}

				KFbxLayerElementUV* leUV = pMesh->GetLayer(l)->GetUVs();
				if (leUV)
				{
					sprintf(header, "            Texture UV (on layer %d): ", l);

					switch (leUV->GetMappingMode())
					{
					case KFbxLayerElement::eBY_CONTROL_POINT:
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case KFbxLayerElement::eBY_POLYGON_VERTEX:
						{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leUV->GetIndexArray().GetAt(lTextureUVIndex);
								Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						}
						break;

					case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for UVs
					case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for UVs
					case KFbxLayerElement::eNONE:       // doesn't make much sense for UVs
						break;
					}
				}

			} // for layer
			vertexId++;
		} // for polygonSize
	} // for polygonCount

	DisplayString("");
}
