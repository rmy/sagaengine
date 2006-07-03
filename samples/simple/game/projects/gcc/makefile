
-include rel_type
include module_config

#
# Headers
#
SimplePlayerParserModule_hpp = ../../src/game/io/SimplePlayerParserModule.hpp 

RotateNone_hpp = ../../src/game/action/RotateNone.hpp 

all_hpp = ../../src/game/io/SimplePlayerParserModule.hpp ../../src/game/io/all.hpp ../../src/game/io/SimpleAreaThingParserModule.hpp ../../src/game/io/SimpleAreaParserModule.hpp ../../src/game/io/SimpleCameraParserModule.hpp ../../src/game/io/SimpleActorParserModule.hpp 

all_hpp = ../../src/game/physics/PhSimple.hpp ../../src/game/physics/all.hpp 

SimplePlayerFactory_hpp = ../../src/game/thing/SimplePlayerFactory.hpp 

GameSchema_hpp = ../../src/game/schema/GameSchema.hpp 

all_hpp = ../../src/game/schema/GameSchema.hpp ../../src/game/schema/all.hpp 

SimpleCameraParserModule_hpp = ../../src/game/io/SimpleCameraParserModule.hpp 

RotateRight_hpp = ../../src/game/action/RotateRight.hpp 

Scripting_hpp = ../../src/game/app/Scripting.hpp 

SimpleActorFactory_hpp = ../../src/game/thing/SimpleActorFactory.hpp 

Application_hpp = ../../src/game/app/Application.hpp 

Jump_hpp = ../../src/game/action/Jump.hpp 

Backward_hpp = ../../src/game/action/Backward.hpp 

SimpleCameraFactory_hpp = ../../src/game/thing/SimpleCameraFactory.hpp 

all_hpp = ../../src/game/action/all.hpp ../../src/game/action/RotateNone.hpp ../../src/game/action/Forward.hpp ../../src/game/action/Stop.hpp ../../src/game/action/RotateLeft.hpp ../../src/game/action/RotateRight.hpp ../../src/game/action/Jump.hpp ../../src/game/action/Backward.hpp 

Forward_hpp = ../../src/game/action/Forward.hpp 

PhSimple_hpp = ../../src/game/physics/PhSimple.hpp 

game_area_hpp = ../../src/game/area/game_area.hpp 

all_hpp = ../../src/game/thing/SimpleActorFactory.hpp ../../src/game/thing/all.hpp ../../src/game/thing/SimplePlayerFactory.hpp 

SimpleActorParserModule_hpp = ../../src/game/io/SimpleActorParserModule.hpp 

all_hpp = ../../src/game/area/game_area.hpp ../../src/game/area/all.hpp ../../src/game/area/SimpleArea.hpp 

SimpleAreaParserModule_hpp = ../../src/game/io/SimpleAreaParserModule.hpp 

Stop_hpp = ../../src/game/action/Stop.hpp 

SimpleAreaThingParserModule_hpp = ../../src/game/io/SimpleAreaThingParserModule.hpp 

RotateLeft_hpp = ../../src/game/action/RotateLeft.hpp 

SimpleArea_hpp = ../../src/game/area/SimpleArea.hpp 

#
# Compile
#
../../src/game/io/SimplePlayerParserModule.o: ../../src/game/io/SimplePlayerParserModule.cpp $(SimplePlayerParserModule_hpp) $(GamePre_H) $(SimplePlayerFactory_hpp)
	$(COMPILE)


../../src/game/thing/SimpleCameraFactory.o: ../../src/game/thing/SimpleCameraFactory.cpp $(SimpleCameraFactory_hpp)
	$(COMPILE)


../../src/game/thing/SimpleActorFactory.o: ../../src/game/thing/SimpleActorFactory.cpp $(SimpleActorFactory_hpp)
	$(COMPILE)


../../src/game/action/RotateLeft.o: ../../src/game/action/RotateLeft.cpp $(RotateLeft_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/action/Forward.o: ../../src/game/action/Forward.cpp $(Forward_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/thing/SimplePlayerFactory.o: ../../src/game/thing/SimplePlayerFactory.cpp $(SimplePlayerFactory_hpp)
	$(COMPILE)


../../src/game/io/SimpleCameraParserModule.o: ../../src/game/io/SimpleCameraParserModule.cpp $(SimpleCameraParserModule_hpp) $(SimpleCameraFactory_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/physics/PhSimple.o: ../../src/game/physics/PhSimple.cpp $(SimpleArea_hpp) $(PhSimple_hpp)
	$(COMPILE)


../../src/game/io/SimpleActorParserModule.o: ../../src/game/io/SimpleActorParserModule.cpp $(SimpleActorFactory_hpp) $(SimpleActorParserModule_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/action/Jump.o: ../../src/game/action/Jump.cpp $(Jump_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/action/Backward.o: ../../src/game/action/Backward.cpp $(GamePre_H) $(Backward_hpp)
	$(COMPILE)


../../src/game/schema/GameSchema.o: ../../src/game/schema/GameSchema.cpp $(GameSchema_hpp) $(all_hpp) $(all_hpp) $(GamePre_H)
	$(COMPILE)


../../src/Start.o: ../../src/Start.cpp $(windows_h) $(Application_hpp)
	$(COMPILE)


../../src/game/action/RotateNone.o: ../../src/game/action/RotateNone.cpp $(GamePre_H) $(RotateNone_hpp)
	$(COMPILE)


../../src/game/app/Application.o: ../../src/game/app/Application.cpp $(GameSchema_hpp) $(GamePre_H) $(Application_hpp) $(Scripting_hpp)
	$(COMPILE)


../../src/game/action/RotateRight.o: ../../src/game/action/RotateRight.cpp $(RotateRight_hpp) $(GamePre_H)
	$(COMPILE)


../../src/game/app/Scripting.o: ../../src/game/app/Scripting.cpp $(GamePre_H) $(Scripting_hpp)
	$(COMPILE)


../../src/game/io/SimpleAreaParserModule.o: ../../src/game/io/SimpleAreaParserModule.cpp $(SimpleAreaParserModule_hpp) $(SimpleArea_hpp)
	$(COMPILE)


../../src/game/io/SimpleAreaThingParserModule.o: ../../src/game/io/SimpleAreaThingParserModule.cpp $(SimpleAreaThingParserModule_hpp)
	$(COMPILE)


../../src/game/area/SimpleArea.o: ../../src/game/area/SimpleArea.cpp $(GamePre_H) $(SimpleArea_hpp)
	$(COMPILE)


../../src/game/action/Stop.o: ../../src/game/action/Stop.cpp $(Stop_hpp) $(GamePre_H)
	$(COMPILE)


#
# Link
#
$(LINK_TARGET):   ../../src/game/io/SimplePlayerParserModule.o ../../src/game/thing/SimpleCameraFactory.o ../../src/game/thing/SimpleActorFactory.o ../../src/game/action/RotateLeft.o ../../src/game/action/Forward.o ../../src/game/thing/SimplePlayerFactory.o ../../src/game/io/SimpleCameraParserModule.o ../../src/game/physics/PhSimple.o ../../src/game/io/SimpleActorParserModule.o ../../src/game/action/Jump.o ../../src/game/action/Backward.o ../../src/game/schema/GameSchema.o ../../src/Start.o ../../src/game/action/RotateNone.o ../../src/game/app/Application.o ../../src/game/action/RotateRight.o ../../src/game/app/Scripting.o ../../src/game/io/SimpleAreaParserModule.o ../../src/game/io/SimpleAreaThingParserModule.o ../../src/game/area/SimpleArea.o ../../src/game/action/Stop.o
	$(LINK)



#
# Clean
#

clean:
	python $(SE_SCRIPTS)/clean.py ../../src
	rm -f $(LINK_TARGET) $(TARGET_DIR)/$(LINK_TARGET)



#
# Auto generate all.hpp files
#

alls:

	python $(SE_SCRIPTS)/createAllHpp.py ../../src/


#
# Makefile
#

mf:
	python $(SE_SCRIPTS)/createGccMakefile.py > makefile
    
