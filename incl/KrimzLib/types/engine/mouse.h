#pragma once


namespace kl {
    struct mouse {
		bool lmb;
		bool mmb;
		bool rmb;
		kl::point position;

		// Constructor
		mouse() {
			lmb = false;
			mmb = false;
			rmb = false;
			position = kl::point();
		}
	};
}