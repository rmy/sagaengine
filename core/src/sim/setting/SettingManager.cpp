#include "SettingManager.hpp"
#include "util/error/Log.hpp"

namespace se_core {

	void SettingManager
	::add(Setting& s) {
		AssertFatal(settingCount_ < MAX_SETTINGS, "Too many settings in SettingManager.");
		AssertFatal(find(s.name()) == 0, "Value named " << s.name() << " already exists");
		settings_[ settingCount_++ ] = &s;
	}


	void SettingManager
	::remove(Setting& s) {
		for(int i = 0; i < settingCount_; ++i) {
			if(settings_[i] == &s) {
				settings_[i] = settings_[ --settingCount_ ];
				break;
			}
		}
	}

	Setting* SettingManager
	::find(const char* name) const {
		for(int i = 0; i < settingCount_; ++i) {
			if(settings_[i]->is(name)) {
				return settings_[i];
			}
		}
		return 0;
	}

	void SettingManager
	::commit() const {
		for(int i = 0; i < settingCount_; ++i) {
			if(settings_[i]->hasValueChanged())
				settings_[i]->commit();
		}
	}

	void SettingManager
	::revert() const {
		for(int i = 0; i < settingCount_; ++i) {
			if(settings_[i]->hasValueChanged())
				settings_[i]->revert();
		}
	}

	Setting& SettingManager
	::setting(int index) const {
		AssertFatal(index >= 0 && index < settingCount_, "Illegal setting index.");
		return *settings_[ index ];
	}
}
