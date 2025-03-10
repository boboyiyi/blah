#include <blah/time.h>
#include <chrono>

using namespace Blah;
using namespace std::chrono;

u64 Time::ticks = 0;
u64 Time::previous_ticks = 0;
double Time::seconds = 0;
double Time::previous_seconds = 0;
float Time::delta = 0;
float Time::pause_timer = 0;

void Time::pause_for(float duration)
{
	if (duration >= pause_timer)
		pause_timer = duration;
}

bool Time::on_interval(double time, float delta, float interval, float offset)
{
	auto last = static_cast<long>((time - offset - delta) / interval);
	auto next = static_cast<long>((time - offset) / interval);
	return last < next;
}

bool Time::on_interval(float delta, float interval, float offset)
{
	return Time::on_interval(Time::seconds, delta, interval, offset);
}

bool Time::on_interval(float interval, float offset)
{
	return Time::on_interval(Time::seconds, Time::delta, interval, offset);
}

bool Time::on_time(double time, double timestamp) 
{
	float c = static_cast<float>(time) - Time::delta;
	return time >= timestamp && c < timestamp;
}

bool Time::between_interval(double time, float interval, float offset)
{
	static const auto modf = [](double x, double m)
	{
		return x - (int)(x / m) * m;
	};

	return modf(time - offset, ((double)interval) * 2) >= interval;
}

bool Time::between_interval(float interval, float offset)
{
	return between_interval(Time::seconds, interval, offset);
}

Stopwatch::Stopwatch()
{
	reset();
}

void Stopwatch::reset()
{
	start_time = std::chrono::duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count();
}

u64 Stopwatch::milliseconds()
{
	return microseconds() / 1000;
}

u64 Stopwatch::microseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count() - start_time;
}
