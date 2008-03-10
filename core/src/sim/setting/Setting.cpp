#include "Setting.hpp"
#include "SettingManager.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	Setting
	::Setting(const char* name, int defaultValue, int maxValue) 
			: name_(name), value_(defaultValue), oldValue_(-1), valueCount_(maxValue + 1) {
		SettingManager::singleton().add(*this);
		static const char* numbers[] = {
			"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"
		};
		AssertFatal(valueCount_ <= sizeof(numbers) / sizeof(numbers[0] + 1), "Max value too big");
		valueNames_ = numbers;
	}


	Setting
	::~Setting() {
		SettingManager::singleton().remove(*this);
	}


	void Setting
	::setValueNames(const char** valueNames) {
		valueNames_ = valueNames;
		valueCount_ = 0;
		while(valueNames_[ valueCount_ ]) {
			++valueCount_;
		}
	}

}