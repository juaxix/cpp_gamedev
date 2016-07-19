/// Updater.h
/// Header of the class Updater

#pragma once

namespace xixEngine {
	///Class used to call Update from the Engine implementation every frame
	class Updater {
	public:
		/// Update is called once per frame
		virtual void Update() = 0;
	protected:
		~Updater() {}
	};
}
