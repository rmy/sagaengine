# What is SagaEngine?

SagaEngine is a game logic engine. It is centered around the concepts Actor, Action and Area. An Actor may exist and perform Actions inside an Area. This is the simple view, though. The library is a quite large object oriented architecture for resolving game situations. You are expected to subclass heavily for the purposes of your own game.

# Feature highlights

 * Hierarchy of game entities: Game entities like Area, Thing and Actor are stored in a hierarchy, where a child moves relative to its parent. An Area is usually at the root. A rider Actor would be the child of a horse Actor while saddled.
 * Scripting: Subclasses of the Script class can be assigned an Actor to automatically plan the next action when a previous one is complete. This architecture makes it simple to integrate scripting languages.
 * Custom physics: Sublclasses of Physics are assigned to an Actor to control movement. This means that a ground walker and a bird may be controlled by different physics algorithms.

Ogre integration: SagaEngine Core contains no render logic in itself, but is built with the intention that you program add-on modules that serves as a glue between SagaEngine Core and a render engine. SagaEngine Ogre is such a gluing layer between SagaEngine Core and the opensource Ogre engine (http://www.ogre3d.org/).

# License

Permission is granted to distribute the library under the terms of the Q Public License version 1.0.
