SAGAENGINE = ../../../../

run:
	cd $(SAGAENGINE)/core/projects/qmake && make
	cd $(SAGAENGINE)/add_on/client/projects/qmake && make
	cd $(SAGAENGINE)/add_on/ogre/projects/qmake && make
	cd $(SAGAENGINE)/samples/simple/ogre_ui/projects/qmake && make
	cd $(SAGAENGINE)/samples/simple/game/projects/qmake && make


bootstrap:
	cp proj.pro ../../../../env/qmake/
	cd $(SAGAENGINE)/core/projects/qmake && sh bootstrap.sh
	cd $(SAGAENGINE)/add_on/client/projects/qmake && sh bootstrap.sh
	cd $(SAGAENGINE)/add_on/ogre/projects/qmake && sh bootstrap.sh
	cd $(SAGAENGINE)/samples/simple/ogre_ui/projects/qmake && sh bootstrap.sh
	cd $(SAGAENGINE)/samples/simple/game/projects/qmake && sh bootstrap.sh
