#ifndef sim_setting_SettingManager_hpp
#define sim_setting_SettingManager_hpp

#include "Setting.hpp"

namespace se_core {

	class SettingManager {
	public:
		static SettingManager& singleton() {
			static SettingManager sm;
			return sm;
		}

		void add(Setting& s);
		void remove(Setting& s);
		int settingCount() const {
			return settingCount_;
		}
		Setting& setting(int index) const;
		Setting* find(const char* name) const;

		void commit() const;
		void revert() const;

	private:
		SettingManager() : settingCount_(0) {
		}

		static const int MAX_SETTINGS = 64;
		int settingCount_;
		Setting* settings_[ MAX_SETTINGS ];
	};

}

#endif
