#ifndef sim_setting_Setting_hpp
#define sim_setting_Setting_hpp

#include "util/type/String.hpp"

namespace se_core {

	class Setting {
	public:
		Setting(const char* name, int defaultValue, int maxValue);
		~Setting();

		const char* name() const {
			return name_.get();
		}

		bool is(const char* name) const {
			return name_.equals(name);
		}

		virtual void commit() {
			oldValue_ = value_;
		}

		void revert() {
			value_ = oldValue_;
		}

		int value() const {
			return value_;
		}

		virtual const char* valueName(int v = -1) const {
			if(v < 0)
				return valueNames_[ value_ ];
			return valueNames_[ v ];
		}

		bool hasValueChanged() {
			return oldValue_ != value_;
		}

		void setValueNames(const char** valueNames);

		void inc() {
			++value_;
			value_ %= valueCount_;
		}

		void setValue(int v) {
			value_ = v % valueCount_;
		}

		void dec() {
			--value_;
			if(value_ < 0)
				value_ += valueCount_;
		}

	protected:
		String name_;
		int oldValue_, value_, valueCount_;
		const char** valueNames_;
	};
}

#endif
