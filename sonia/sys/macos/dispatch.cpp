//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "dispatch.hpp"
#include "sonia/exceptions.hpp"
namespace sonia::macos {

dispatch_queue_t global_queue_{nullptr};

void run_queue()
{
	global_queue_ = dispatch_queue_create("timerQueue", 0);

}

void stop_queue()
{
	dispatch_release(global_queue_);
}

timer_descriptor::timer_descriptor(function<void()> const& h)
	: handler_{h}
{
	timer_ = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, global_queue_);
}

void timer_descriptor::create(bool realtime)
{
	if (!realtime) {
		THROW_NOT_IMPLEMENTED_ERROR("monotonic is not supported");
	}
	auto self = shared_from_this();
	dispatch_source_set_event_handler(timer_, ^{ self->handler_(); });
}

timer_descriptor::~timer_descriptor()
{
	release();
}

void timer_descriptor::set(std::chrono::milliseconds ms)
{
	uint64_t interval = (uint64_t)ms.count() * 1000000;
	dispatch_source_set_timer(timer_, DISPATCH_TIME_NOW, interval, 0);
	dispatch_resume(timer_);
	while (disarmed_cnt_.load()) {
		--disarmed_cnt_;
		dispatch_resume(timer_);
	}
}

void timer_descriptor::disarm()
{
	dispatch_suspend(timer_);
	++disarmed_cnt_;
}


void timer_descriptor::release()
{
	if (timer_) {
		while (disarmed_cnt_.load()) {
			--disarmed_cnt_;
			dispatch_resume(timer_);
		}
		dispatch_source_cancel(timer_);
		timer_ = nullptr;
	}
}

}
