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
/// @file Fork.hpp
///
/// Convenience wrapper for forking processes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_CONCURRENT__FORK
#define COM_BORA_SOFTWARE__BALAU_CONCURRENT__FORK

#include <Balau/Exception/SystemExceptions.hpp>
#include <Balau/Dev/Assert.hpp>

#include <boost/predef.h>

#if BOOST_OS_UNIX
	#include <sys/wait.h>
#endif

namespace Balau::Concurrent {

///
/// Convenience wrapper for forking processes.
///
/// TODO determine how to simulate this on the Windows platform.
///
class Fork {
	///
	/// Determine whether forking is support on this platform.
	///
	public: static bool forkSupported() {
		return BOOST_OS_UNIX;
	}

	///
	/// Perform a fork operator and run the supplied function for the child.
	///
	/// The child process will exit when the function returns if exitChild is set
	/// to true. Otherwise, the child will return and the caller will need to
	/// handle child termination.
	///
	/// @param  function the function to run in the child process
	/// @param  exitChild if set to true, _Exit(int) is called with the the exit state of the function
	/// @return the child pid for the parent process, the exit state of the function for the child process if it returns
	/// @throw ForkException if the fork call failed
	///
	public: static int performFork(const std::function<long long ()> & function, bool exitChild) {
		Assert::assertion(forkSupported(), "fork() called for platform that does not support it.");

		#if BOOST_OS_UNIX
			const int pid = ::fork();

			if (pid == 0) {
				const int status = (int) function();

				if (exitChild) {
					_Exit(status);
				} else {
					return status;
				}
			} else if (pid > 0) {
				return pid;
			} else {
				ThrowBalauException(Exception::ForkException, errno, "Failed to fork process.");
			}
		#else
			return 0; // not supported
		#endif
	}

	///
	/// Perform a fork operator and run the supplied function for the child.
	///
	/// The child will return when the function has completed and the caller will
	/// need to handle child termination.
	///
	/// This version does not require a function that returns an integer.
	///
	/// @param function the function to run in the child process
	/// @return the child pid for the parent process and zero for the child process
	/// @throw ForkException if the fork call failed
	///
	public: static int performFork(const std::function<long long ()> & function) {
		Assert::assertion(forkSupported(), "fork() called for platform that does not support it.");

		#if BOOST_OS_UNIX
			const int pid = ::fork();

			if (pid == 0) {
				function();
				return 0;
			} else if (pid > 0) {
				return pid;
			} else {
				ThrowBalauException(Exception::ForkException, errno, "Failed to fork process.");
			}
		#else
			return 0; // not supported
		#endif
	}

	///
	/// A termination report, returned by wait methods.
	///
	/// Instances of this structure are returned from the pid checking functions
	/// in this class, in order to communicate the status code of the process.
	///
	public: struct TerminationReport {
		///
		/// The PID of the process.
		///
		int pid;

		///
		/// The signal code.
		///
		int code;

		///
		/// The exit status.
		///
		int exitStatus;

		TerminationReport() : pid(0), code(0), exitStatus(0) {}

		TerminationReport(int pid_, int code_, int exitStatus_)
			: pid(pid_), code(code_), exitStatus(exitStatus_) {}

		TerminationReport(const TerminationReport & copy) = default;

		TerminationReport(TerminationReport && rhs) noexcept
			: pid(rhs.pid), code(rhs.code), exitStatus(rhs.exitStatus) {}

		TerminationReport & operator = (const TerminationReport & copy) = default;

		TerminationReport & operator = (TerminationReport && rhs) noexcept {
			pid = rhs.pid;
			code = rhs.code;
			exitStatus = rhs.exitStatus;
			return *this;
		}
	};

	///
	/// Wait on a process until the process terminates.
	///
	/// If the pid is invalid, an empty termination report is returned.
	///
	/// @param pid the process id
	/// @return a termination report
	/// @throw WaitException if the waitid call failed
	///
	public: static TerminationReport waitOnProcess(long long pid) {
		if (pid <= 0) {
			return TerminationReport();
		}

		#if BOOST_OS_UNIX
			siginfo_t infop;
			int options = WEXITED;

			memset(&infop, 0, sizeof(siginfo_t));

			if (waitid(P_PID, (unsigned int) pid, &infop, options) == -1) {
				ThrowBalauException(Exception::WaitException, errno);
			} else if (infop.si_pid) {
				return TerminationReport(pid, infop.si_code, infop.si_status);
			}
		#endif

		// TODO what else may happen here?
		return TerminationReport();
	}

	///
	/// Check the process for termination without blocking.
	///
	/// If the pid is invalid, an empty termination report is returned.
	///
	/// If the process with the specified id has not terminated, an empty
	/// termination report is returned.
	///
	/// @param pid the process id
	/// @return a termination report
	/// @throw WaitException if the waitid call failed
	///
	public: static TerminationReport checkForTermination(int pid) {
		if (pid <= 0) {
			return TerminationReport();
		}

		#if BOOST_OS_UNIX
			siginfo_t infop;
			int options = WEXITED | WNOHANG;

			memset(&infop, 0, sizeof(siginfo_t));

			if (waitid(P_PID, (unsigned int) pid, &infop, options) == -1) {
				ThrowBalauException(Exception::WaitException, errno);
			} else if (infop.si_pid) {
				switch (infop.si_code) {
					case CLD_EXITED:
					case CLD_KILLED:
					case CLD_DUMPED: {
						return TerminationReport(pid, infop.si_code, infop.si_status);
					}

					default: {
						break;
					}
				}
			}
		#endif

		return TerminationReport();
	}

	///
	/// Check without blocking the supplied processes for termination.
	///
	/// @param pids the process ids
	/// @return a vector of termination reports for the pids that terminated.
	/// @throw WaitException if the waitid call failed
	///
	public: static std::vector<TerminationReport> checkForTermination(const std::vector<int> & pids) {
		std::vector<TerminationReport> reports;

		#if BOOST_OS_UNIX
			siginfo_t infop;
			int options = WEXITED | WNOHANG; // NOLINT

			for (int pid : pids) {
				if (pid <= 0) {
					continue;
				}

				memset(&infop, 0, sizeof(siginfo_t));

				if (waitid(P_PID, (unsigned int) pid, &infop, options) == -1) {
					ThrowBalauException(Exception::WaitException, errno);
				} else if (infop.si_pid) {
					switch (infop.si_code) {
						case CLD_EXITED:
						case CLD_KILLED:
						case CLD_DUMPED: {
							reports.emplace_back(pid, infop.si_code, infop.si_status);
						}

						default: {
							break;
						}
					}
				}
			}
		#endif

		return reports;
	}

	///
	/// Terminate the child process if it is running.
	///
	/// If the process has not terminated, terminate it and return an empty termination report.
	/// If the pid is invalid, an empty termination report is returned.
	///
	/// @param pid the process id
	/// @return a termination report
	/// @throw WaitException if the waitid call failed
	///
	public: static TerminationReport terminateProcess(int pid) {
		if (pid <= 0) {
			return TerminationReport();
		}

		#if BOOST_OS_UNIX
			siginfo_t infop;
			int options = WEXITED | WNOHANG; // NOLINT

			memset(&infop, 0, sizeof(siginfo_t));

			if (waitid(P_PID, (unsigned int) pid, &infop, options) == -1) {
				ThrowBalauException(Exception::WaitException, errno);
			} else if (infop.si_pid) {
				switch (infop.si_code) {
					case CLD_EXITED:
					case CLD_KILLED:
					case CLD_DUMPED: {
						return TerminationReport(pid, infop.si_code, infop.si_status);
					}

					default: {
						kill(pid, SIGKILL);
						break;
					}
				}
			}
		#endif

		return TerminationReport();
	}
};

} // namespace Balau::Concurrent

#endif // COM_BORA_SOFTWARE__BALAU_CONCURRENT__FORK
