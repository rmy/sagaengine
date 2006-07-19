SHELL       = /bin/sh

FBXDIR      = /usr/local/src/FBXSDK2005.12

LIBDIR      = $(FBXDIR)/lib
INCDIR      = $(FBXDIR)/include
COMMONDIR   = $(FBXDIR)/examples/Common
IMPORTDIR   = $(FBXDIR)/examples/ImportScene

TARGET      = fbx2areaThings
OBJS  = \
	$(COMMONDIR)/Common.o\
	$(IMPORTDIR)/DisplayAnimation.o\
	$(IMPORTDIR)/DisplayCamera.o\
	$(IMPORTDIR)/DisplayCommon.o\
	$(IMPORTDIR)/DisplayGlobalSettings.o\
	$(IMPORTDIR)/DisplayHierarchy.o\
	$(IMPORTDIR)/DisplayLight.o\
	$(IMPORTDIR)/DisplayLink.o\
	$(IMPORTDIR)/DisplayMarker.o\
	$(IMPORTDIR)/DisplayMaterial.o\
	$(IMPORTDIR)/DisplayMesh.o\
	$(IMPORTDIR)/DisplayNurb.o\
	$(IMPORTDIR)/DisplayPatch.o\
	$(IMPORTDIR)/DisplayPivotsAndLimits.o\
	$(IMPORTDIR)/DisplayPose.o\
	$(IMPORTDIR)/DisplayShape.o\
	$(IMPORTDIR)/DisplaySkeleton.o\
	$(IMPORTDIR)/DisplayTexture.o\
	$(IMPORTDIR)/DisplayUserProperties.o\
	./main.o

LIBS        = -lm -lstdc++ -lpthread -ldl -lfbxsdk_gcc4

CC          = gcc
LD          = gcc
RM          = /bin/rm -f

CXXFLAGS    = -DK_PLUGIN -DK_FBXSDK -m32
LDFLAGS     = -m32

.SUFFIXES: .cxx

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -L$(LIBDIR) $(LIBS) 

.cxx.o:
	$(CC) $(CXXFLAGS) -I$(INCDIR) -I$(COMMONDIR) -c $< -o $*.o 

.cpp.o:
	$(CC) $(CXXFLAGS) -I$(INCDIR) -I$(COMMONDIR) -I$(IMPORTDIR) -c $< -o $*.o 

clean:
	$(RM) $(TARGET) *.o $(COMMONDIR)/*.o $(IMPORTDIR)/*.o







