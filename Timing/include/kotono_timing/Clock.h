#pragma once
#include <concepts>
#include <kotono_common/types.h>
/// Clock helper class, time is expressed in seconds
class UClock final
{
public:
	/// Set the internal time variables to ExactUTC()
	void Init();
	/// Set the internal nowUTC_ variable to ExactUTC()
	void Update();

	/// UTC time at which the program started.
	auto GetStartUTC() const -> f64 { return startUTC_; }
	/// UTC time at which the program last updated.
	auto GetNowUTC() const -> f64 { return nowUTC_; }

	/// Elapsed time since the start of the program in seconds.
	template <std::floating_point T>
	auto Now() const -> T { return static_cast<T>(nowUTC_ - startUTC_); }

	/// Exact UTC time since Epoch in seconds.
	static auto ExactUTC() -> f64;

private:
	f64 startUTC_;
	f64 nowUTC_;
};