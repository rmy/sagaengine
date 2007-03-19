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


#ifndef RefPtr_hpp
#define RefPtr_hpp

#include "util/error/Log.hpp"

namespace se_core {

	/**
	 */
	class _SeCoreExport RefPtr {
	private:
		class Ptr {
		public:
			Ptr(void* ptr)
				: ptr_(ptr), refCount_(1) {
			}

			void* object() {
				return ptr_;
			}

			void incRef() {
				++refCount_;
			}

			void decRef() {
				--refCount_;
				if(!refCount_)
					delete this;
			}
			void* ptr_;
			int refCount_;

		private:
			~Ptr() {}
		};

	public:
		/**
		 * Construct unnamed RefPtr.
		 */
		RefPtr()
			: ptr_(0), isOwner_(false) {
		}

		/**
		 * Destructor.
		 */
		virtual ~RefPtr() {
			if(ptr_) {
				if(isOwner_)
					ptr_->ptr_ = 0;
				ptr_->decRef();
			}
		}

		void set(RefPtr& sp) {
			Assert(!isOwner_ && "Owner RefPtr's should never be assigned a new value");
			if(ptr_)
				ptr_->decRef();

			ptr_ = sp.ptr_;

			ptr_->incRef();
		}

		void reset() {
			if(ptr_)
				ptr_->decRef();

			ptr_ = 0;
		}

		bool isNull() const {
			return (ptr_ == 0 || ptr_->ptr_ == 0);
		}

		void* object() {
			Assert(ptr_ && "Check for isNull() before fetching object");
			return ptr_->ptr_;
		}

		void* object() const {
			Assert(ptr_ && "Check for isNull() before fetching object");
			return ptr_->ptr_;
		}

		RefPtr(void* ptr)
			: ptr_(new Ptr(ptr)), isOwner_(true) {
		}

	private:
		/**
		 */
		Ptr* ptr_;
		bool isOwner_;

		RefPtr(const RefPtr&);
		RefPtr& operator=(const RefPtr&);
	};

}

#endif
