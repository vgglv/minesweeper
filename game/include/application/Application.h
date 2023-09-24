#pragma once

namespace game::app {
	class Application {
	public:
		Application() = default;
		~Application() = default;

		bool initialize();
		void update();
		bool isTerminating() const;
		void terminate();
		void run();

	private:
		bool is_terminating = false;
	};
}