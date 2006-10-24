#ifndef enums
#define enums

enum FileDataRoles {
	FILE_PATH = Qt::UserRole + 1,
	HEADER_CODE,
	LEVELS
};


enum FolderRoles {
	TYPE = Qt::UserRole + 1,
	INCLUDE
};


#endif
