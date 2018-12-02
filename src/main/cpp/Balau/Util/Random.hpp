// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file Random.hpp
///
/// Convenience wrappers around the C++ 11 random number generator library.
///

#ifndef COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM
#define COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM

#include <Balau/Util/Impl/RandomImpl.hpp>

namespace Balau::Util {

///////////////////////////// The implementation //////////////////////////////

/// The random number generator implementation class. Typedefs are defined for different primitive types.
///
/// The Mersenne Twister generator is used internally.
///
template <typename T, typename D> class RandomNumberGenerator final {
	///
	/// Create a randomly seeded random number generator with a range of [lower, upper].
	///
	public: RandomNumberGenerator(T lower, T upper)
		: generator(std::random_device()())
		, distribution(D(lower, Impl::inclusivise(upper))) {}

	///
	/// Create an explicitly seeded random number generator with a range of [lower, upper].
	///
	public: RandomNumberGenerator(T lower, T upper, unsigned int seed)
		: generator(seed)
		, distribution(D(lower, Impl::inclusivise(upper))) {}

	///
	/// Get the next random number.
	///
	public: T operator () () {
		return distribution(generator);
	}

	////////////////////////// Private implementation /////////////////////////

	public: RandomNumberGenerator(const RandomNumberGenerator & copy) = delete;
	public: RandomNumberGenerator & operator = (const RandomNumberGenerator & copy) = delete;

	private: std::mt19937 generator;
	private: D distribution;
};

/////////////////////// Uniform distribution generators ///////////////////////

///
/// A uniform distribution, double precision floating point random number generator.
///
using UniformDouble = RandomNumberGenerator<double, std::uniform_real_distribution<double>>;

///
/// A uniform distribution, single precision floating point random number generator.
///
using UniformFloat = RandomNumberGenerator<float, std::uniform_real_distribution<float>>;

///
/// A uniform distribution, 32 bit signed integer floating point random number generator.
///
using UniformInt32 = RandomNumberGenerator<int, std::uniform_int_distribution<int>>;

///
/// A uniform distribution, 64 bit signed integer floating point random number generator.
///
using UniformInt64 = RandomNumberGenerator<long, std::uniform_int_distribution<long>>;

///
/// A uniform distribution, 32 bit signed integer floating point random number generator.
///
using UniformUInt32 = RandomNumberGenerator<unsigned int, std::uniform_int_distribution<unsigned int>>;

///
/// A uniform distribution, 64 bit signed integer floating point random number generator.
///
using UniformUInt64 = RandomNumberGenerator<unsigned long long, std::uniform_int_distribution<unsigned long long>>;

/////////////////////// Normal distribution generators ////////////////////////

///
/// A normal distribution, double precision floating point random number generator.
///
using NormalDouble = RandomNumberGenerator<double, std::normal_distribution<double>>;

///
/// A normal distribution, single precision floating point random number generator.
///
using NormalFloat = RandomNumberGenerator<float, std::normal_distribution<float>>;

//////////////////////// All C++ 11 distribution types ////////////////////////

///
/// A uniform distribution, floating point random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using UniformReal = RandomNumberGenerator<T, std::uniform_real_distribution<T>>;

///
/// A uniform distribution, integer random number generator.
///
/// @tparam T the integer type
///
template <typename T> using UniformInt = RandomNumberGenerator<T, std::uniform_int_distribution<T>>;

///
/// A normal distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using Normal = RandomNumberGenerator<T, std::normal_distribution<T>>;

///
/// A log normal distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using LogNormal = RandomNumberGenerator<T, std::lognormal_distribution<T>>;

///
/// A gamma distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using Gamma = RandomNumberGenerator<T, std::gamma_distribution<T>>;

///
/// A chi squared distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using ChiSquared = RandomNumberGenerator<T, std::chi_squared_distribution<T>>;

///
/// A Cauchy distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using Cauchy = RandomNumberGenerator<T, std::cauchy_distribution<T>>;

///
/// A Fisher F distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using FisherF = RandomNumberGenerator<T, std::fisher_f_distribution<T>>;

///
/// A Student T distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using StudentT = RandomNumberGenerator<T, std::student_t_distribution<T>>;

///
/// A discrete binomial distribution random number generator.
///
/// @tparam T the integer type
///
template <typename T> using Binomial = RandomNumberGenerator<T, std::binomial_distribution<T>>;

///
/// A geometric distribution random number generator.
///
/// @tparam T the integer type
///
template <typename T> using Geometric = RandomNumberGenerator<T, std::geometric_distribution<T>>;

///
/// A negative binomial distribution random number generator.
///
/// @tparam T the integer type
///
template <typename T> using NegativeBinomial = RandomNumberGenerator<T, std::negative_binomial_distribution<T>>;

///
/// A Poisson distribution random number generator.
///
/// @tparam T the integer type
///
template <typename T> using Poisson = RandomNumberGenerator<T, std::poisson_distribution<T>>;

///
/// An exponential distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using Exponential = RandomNumberGenerator<T, std::exponential_distribution<T>>;

///
/// A Weibull distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using Weibull = RandomNumberGenerator<T, std::weibull_distribution<T>>;

///
/// An extreme value distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using ExtremeValue = RandomNumberGenerator<T, std::extreme_value_distribution<T>>;

///
/// A discrete distribution random number generator.
///
/// @tparam T the integer type
///
template <typename T> using Discrete = RandomNumberGenerator<T, std::discrete_distribution<T>>;

///
/// A piecewise constant distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using PiecewiseConstant = RandomNumberGenerator<T, std::piecewise_constant_distribution<T>>;

///
/// A piecewise linear distribution random number generator.
///
/// @tparam T the floating point type
///
template <typename T> using PiecewiseLinear = RandomNumberGenerator<T, std::piecewise_linear_distribution<T>>;

} // namespace Balau::Util

#endif // COM_BORA_SOFTWARE__BALAU_UTIL__RANDOM
