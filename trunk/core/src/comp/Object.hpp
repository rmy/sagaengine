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
	 *
	 * All instances of Object must be constant
	 * throughout the lifetime of the application.
	 *
	 * Base class for game entitites like Action,
	 * Physics, Collide, etc, that contain
	 * behaviour but no non-const data.
	 *
	 * Maintains name, id and type identifiers.
	 */
	class _SeCoreExport Object {
	public:
		/**
		 * Construct named Object.
		 *
		 * @param type The type of Object (got_ACTION, got_THING, etc)
		 * @param name The name of the game object
		 */
		Object(int type, const char* name);

		/**
		 * Destructor.
		 */
		virtual ~Object();

		/**
		 * Get the Object id of the object.
		 * Each instance of Object has a unique Object id assigned to it
		 * during construction. The id_ is a hash of the name. And will be
		 * the same every time the game is run.
		 *
		 * Sometimes two different names can give the same hash, and id. When
		 * this occurs the name of the object must be changed.
		 *
		 * @return The unique Object instance id.
		 */
		int id() const {
			return id_;
		}

		/**
		 * Get the name of the Object.
		 *
		 * @return The name of the Object, or 0 if is not named.
		 */
		virtual const char* name() const {
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

		/**
		 * Calculate hash (object id) given object type and name.
		 */
		static int hash(int type, const char* name);

	protected:
		static const Object* _lookup(int hash);
		inline static const Object* _lookup(int type, const char* name) {
			return _lookup(hash(type, name));
		}
	private:
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
		 */
		const char* name_;
	};

	se_err::Log& operator<< (se_err::Log& log, const Object& b);
}

#endif
