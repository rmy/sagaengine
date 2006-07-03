#SAGAENGINE = ../../../../
-include ../../../../env/gcc/make_env
-include ../../../../env/gcc/project_config

run:
	rm -f $(SAGAENGINE)/samples/navmesh/game/projects/gcc/run.elf
	cd $(SAGAENGINE)/core/projects/gcc && make
	cd $(SAGAENGINE)/add_on/client/projects/gcc && make
	cd $(SAGAENGINE)/add_on/angelscript/projects/gcc && make
	cd $(SAGAENGINE)/add_on/ogre/projects/gcc && make
	cd $(SAGAENGINE)/samples/navmesh/ogre_ui/projects/gcc && make
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && make && cp run.elf ../../../projects/gcc/

link:
	rm -f $(SAGAENGINE)/samples/navmesh/game/projects/gcc/run.elf
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && make && cp run.elf ../../../projects/gcc/


clean:
	cd $(SAGAENGINE)/core/projects/gcc && make clean
	cd $(SAGAENGINE)/add_on/client/projects/gcc && make clean
	cd $(SAGAENGINE)/add_on/angelscript/projects/gcc && make clean
	cd $(SAGAENGINE)/add_on/ogre/projects/gcc && make clean
	cd $(SAGAENGINE)/samples/navmesh/ogre_ui/projects/gcc && make clean
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && make clean

debug:
	cd $(SAGAENGINE)/core/projects/gcc && make debug
	cd $(SAGAENGINE)/add_on/client/projects/gcc && make debug
	cd $(SAGAENGINE)/add_on/angelscript/projects/gcc && make debug
	cd $(SAGAENGINE)/add_on/ogre/projects/gcc && make debug
	cd $(SAGAENGINE)/samples/navmesh/ogre_ui/projects/gcc && make debug
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && make debug
	rm run.elf

release:
	cd $(SAGAENGINE)/core/projects/gcc && make release
	cd $(SAGAENGINE)/add_on/client/projects/gcc && make release
	cd $(SAGAENGINE)/add_on/angelscript/projects/gcc && make release
	cd $(SAGAENGINE)/add_on/ogre/projects/gcc && make release
	cd $(SAGAENGINE)/samples/navmesh/ogre_ui/projects/gcc && make release
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && make release
	rm run.elf

bootstrap:
	cd $(SAGAENGINE)/core/projects/gcc && sh linux_bootstrap.sh
	cd $(SAGAENGINE)/add_on/client/projects/gcc && sh linux_bootstrap.sh
	cd $(SAGAENGINE)/add_on/angelscript/projects/gcc && sh linux_bootstrap.sh
	cd $(SAGAENGINE)/add_on/ogre/projects/gcc && sh linux_bootstrap.sh
	cd $(SAGAENGINE)/samples/navmesh/ogre_ui/projects/gcc && sh linux_bootstrap.sh
	cd $(SAGAENGINE)/samples/navmesh/game/projects/gcc && sh linux_bootstrap.sh
	cp ../../game/projects/gcc/plugins.cfg .
	cp project_config ../../../../env/gcc/

mf: bootstrap
