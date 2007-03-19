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


#ifndef Object_hpp
#define Object_hpp

#include "comp.hpp"
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
	class _SeCoreExport Object {
	public:
		/**
		 * Construct unnamed Object.
		 *
		 * @param type The type of Object (got_ACTION, got_THING, etc)
		 */
		Object(int type) : type_(type) {
			id_ = createId();
		}

		/**
		 * Construct named Object.
		 *
		 * @param type The type of Object (got_ACTION, got_THING, etc)
		 * @param name The name of the game object
		 */
		Object(int type, const char* name)
			: type_(type), name_(name) {
			id_ = createId();
		}

		/**
		 * Destructor.
		 */
		virtual ~Object() {
		}

		/**
		 * Get the Object id of the object.
		 * Each instance of Object has a unique Object id assigned to it
		 * during construction. The id is stable during the lifetime of the object,
		 * but may change from one run to the next, and probably will during
		 * development when the game content is still evolving.
		 *
		 * @return The unique Object instance id.
		 */
		int id() const {
			Assert(this != 0);
			return id_;
		}

		/**
		 * Get the name of the Object.
		 *
		 * @return The name of the Object, or 0 if is not named.
		 */
		virtual const char* name() const {
			// Unnamed objects should probably not be asked for their names.
			Assert(name_);
			return name_;
		}

		/**
		 * Get the type of the Object (got_ACTION, got_THING, etc).
		 * Classes subclassing game objects should have a type enumerated in
		 * the enum ObjectType. The subclass is itself responsible for reporting
		 * the type during construction, and a whole subclass hierarchy may share
		 * the same type (like Action and its subclasses, Script and its subclasses, etc).
		 *
		 * @return The type of the game object.
		 */
		int type() const {
			return type_;
		}

		virtual bool isType(int type) const {
			return type_ == type;
		}


	private:
		/**
		 * Id generator.
		 * Create unique ids for Objects.
		 *
		 * @return Unique Object id.
		 */
		int createId() {
			static int idPool = 0;
			return idPool++;
		}

		/**
		 * The type of Object.
		 */
		int type_;

		/**
		 * The unique instance id of the object.
		 */
		int id_;

		/**
		 * The name of the Object.
		 * @TODO: Should be String ??
		 */
		const char* name_;
	};

}

#endif
