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


#ifndef SimObject_hpp
#define SimObject_hpp

#include "sim.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"

namespace se_core {

	/**
	 * Base class for important game entities.
	 * Base class for game entitites like Action,
	 * Area, Terrain, Thing,
	 * eninge_script::Script, Cutscene,
	 * QuestGoal and subclasses of these.
	 *
	 * Maintains name, id and type identifiers.
	 */
	class _SeCoreExport SimObject {
	public:
		/**
		 * Construct unnamed SimObject.
		 *
		 * @param type The type of SimObject (got_ACTION, got_THING, etc)
		 */
		SimObject(enum SimObjectType type) : type_(type) {
			id_ = createId();
		}

		/**
		 * Construct named SimObject.
		 *
		 * @param type The type of SimObject (got_ACTION, got_THING, etc)
		 * @param name The name of the game object
		 */
		SimObject(enum SimObjectType type, const char* name)
			: type_(type), name_(name) {
			id_ = createId();
		}

		/**
		 * Destructor.
		 */
		virtual ~SimObject() {
		}

		/**
		 * Get the SimObject id of the object.
		 * Each instance of SimObject has a unique SimObject id assigned to it
		 * during construction. The id is stable during the lifetime of the object,
		 * but may change from one run to the next, and probably will during
		 * development when the game content is still evolving.
		 *
		 * @return The unique SimObject instance id.
		 */
		int id() const {
			Assert(this != 0);
			return id_;
		}

		/**
		 * Get the name of the SimObject.
		 *
		 * @return The name of the SimObject, or 0 if is not named.
		 */
		virtual const char* name() const {
			// Unnamed objects should probably not be asked for their names.
			Assert(name_);
			return name_;
		}

		/**
		 * Get the type of the SimObject (got_ACTION, got_THING, etc).
		 * Classes subclassing game objects should have a type enumerated in
		 * the enum SimObjectType. The subclass is itself responsible for reporting
		 * the type during construction, and a whole subclass hierarchy may share
		 * the same type (like Action and its subclasses, Script and its subclasses, etc).
		 *
		 * @return The type of the game object.
		 */
		enum SimObjectType type() const {
			return type_;
		}

		/**
		 * Register SimObject in SimSchema::sortedSimObjectList().
		 */
		void registerInSortedList();

	private:
		/**
		 * Id generator.
		 * Create unique ids for SimObjects.
		 *
		 * @return Unique SimObject id.
		 */
		int createId() {
			static int idPool = 0;
			return idPool++;
		}

		/**
		 * The type of SimObject.
		 */
		enum SimObjectType type_;

		/**
		 * The unique instance id of the object.
		 */
		int id_;

		/**
		 * The name of the SimObject.
		 * @TODO: Should be String ??
		 */
		const char* name_;
	};

}

#endif
