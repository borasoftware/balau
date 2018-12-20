// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file RedirectingHttpWebApp.hpp
///
/// An HTTP web application handler that performs 301 or 302 redirection for a range of HTTP paths.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__REDIRECTING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__REDIRECTING_HTTP_WEB_APP

#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau {

class BalauLogger;
class EnvironmentProperties;

namespace Network::Http::HttpWebApps {

///
/// An HTTP web application handler that performs 301 or 302 redirection for a range of HTTP paths.
///
/// Redirect entries consist of a regular expression and a redirection.
///
/// Each regular expression may contain capture groups. These become request
/// variables $1, $2, etc. that may then be specified in the associated
/// redirection.
///
/// Each redirection consists of an optional HTTP code (301 or 302), plus a
/// redirection path. The redirection path may contain any request variables,
/// including the ones created from the regular expression capture.
///
/// If the HTTP code is omitted, HTTP 302 is returned by default.
///
class RedirectingHttpWebApp : public HttpWebApp {
	///
	/// Constructor called by the HTTP server during construction.
	///
	/// @param configuration The environment configuration for the redirecting web application.
	///
	public: RedirectingHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & logger);

	public: void handleGetRequest(HttpSession & session,
	                              const StringRequest & request,
	                              std::map<std::string, std::string> & variables) override;

	public: void handleHeadRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	public: void handlePostRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override;

	///////////////////////// Private implementation //////////////////////////

	private: void handle(HttpSession & session,
	                     const StringRequest & request,
	                     std::map<std::string, std::string> & variables);

	public: struct PathComponent {
		bool isVariable;
		std::string value;

		// Appends the string or variable value to the stream.
		void append(std::ostringstream & stream, std::map<std::string, std::string> & variables) const {
			if (isVariable) {
				auto iter = variables.find(value);

				if (iter != variables.end()) {
					stream << iter->second;
				}
			} else {
				stream << value;
			}
		}
	};

	public: struct Redirection {
		std::regex matcher;
		unsigned int code;
		std::vector<PathComponent> pathComponents;
		#ifdef BALAU_DEBUG
		std::string regexStr;
		#endif

		std::string createPath(std::map<std::string, std::string> & variables) const {
			std::ostringstream stream;

			for (auto & component : pathComponents) {
				component.append(stream, variables);
			}

			return stream.str();
		}

		Redirection() = default;
		Redirection(const Redirection & ) = default;
		Redirection & operator = (const Redirection & ) = default;

		Redirection(const std::regex & matcher_,
		            unsigned int code_,
		            const std::vector<PathComponent> & pathComponents_
		            #ifdef BALAU_DEBUG
		            , const std::string regexStr_
		            #endif
					)
			: matcher(matcher_)
			, code(code_)
			, pathComponents(pathComponents_)
			#ifdef BALAU_DEBUG
			, regexStr(regexStr_)
			#endif
			{}
	};

	private: static std::vector<Redirection> buildRedirections(const EnvironmentProperties & configuration,
	                                                           const BalauLogger & logger);

	private: static std::vector<PathComponent> buildPathComponents(const std::string & path);

	private: const std::vector<Redirection> redirections;
};

} // namespace Network::Http::HttpWebApps

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__REDIRECTING_HTTP_WEB_APP
