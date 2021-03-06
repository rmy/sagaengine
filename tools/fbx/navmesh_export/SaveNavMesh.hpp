#ifndef _SAVE_NAV_MESH_HPP
#define _SAVE_NAV_MESH_H

#include <fbxfilesdk_def.h>

// Forward declaration.
class FBXFILESDK_NAMESPACE::KFbxNode;

void SaveNavMesh(FBXFILESDK_NAMESPACE::KFbxNode* node, float size, float snap);
void SaveOgre(FBXFILESDK_NAMESPACE::KFbxNode* node, float size, float snap);

#endif // #ifndef _DISPLAY_MESH_H
