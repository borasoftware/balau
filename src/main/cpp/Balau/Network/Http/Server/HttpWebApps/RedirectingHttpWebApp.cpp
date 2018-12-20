// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RedirectingHttpWebApp.hpp"
#include "../HttpSession.hpp"
#include "../../../../Application/EnvironmentProperties.hpp"
#include "../../../../Resource/StringUri.hpp"

namespace Balau::Network::Http::HttpWebApps {

RedirectingHttpWebApp::RedirectingHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & logger)
	: redirections(buildRedirections(configuration, logger)) {}

void RedirectingHttpWebApp::handleGetRequest(HttpSession & session,
                                             const StringRequest & request,
                                             std::map<std::string, std::string> & variables) {
	handle(session, request, variables);
}

void RedirectingHttpWebApp::handleHeadRequest(HttpSession & session,
                                              const StringRequest & request,
                                              std::map<std::string, std::string> & variables) {
	handle(session, request, variables);
}

void RedirectingHttpWebApp::handlePostRequest(HttpSession & session,
                                              const StringRequest & request,
                                              std::map<std::string, std::string> & variables) {
	handle(session, request, variables);
}

void RedirectingHttpWebApp::handle(HttpSession & session,
                                   const StringRequest & request,
                                   std::map<std::string, std::string> & variables) {
	const auto path = std::string(request.target());
	std::smatch matchResult;

	for (const auto & redirection : redirections) {
		if (std::regex_match(path, matchResult, redirection.matcher)) {
			// Add sub-matches to variables before constructing the redirection path.
			size_t subMatchIndex = 0;
			for (const auto & subMatch : matchResult) {
				if (subMatchIndex == 0) { // Miss first result (the whole string).
					++subMatchIndex;
					continue;
				}

				variables[::toString("$", subMatchIndex)] = subMatch;
				++subMatchIndex;
			}

			const std::string redirectionPath = redirection.createPath(variables);

			if (redirection.code == 301) {
				session.sendResponse(createPermanentRedirectResponse(session, request, redirectionPath), redirectionPath);
			} else {
				session.sendResponse(createRedirectResponse(session, request, redirectionPath), redirectionPath);
			}

			return;
		}
	}

	session.sendResponse(createNotFoundStringResponse(session, request));
}

std::vector<RedirectingHttpWebApp::Redirection>
RedirectingHttpWebApp::buildRedirections(const EnvironmentProperties & configuration, const BalauLogger & logger) {
	if (!configuration.hasComposite("matches")) {
		return std::vector<Redirection>();
	}

	std::map<size_t, Redirection> redirectionMap;
	auto matches = configuration.getComposite("matches");

	for (const auto & match : *matches) {
		if (match.isValue<std::string>()) {

			static const std::regex positionAndCode("^(([0-9]+) +([0-9]+)).*$");
			static const std::regex codeOnly("^([0-9]+).*$");

			auto regexStr = std::string(Util::Strings::trim(match.getName()));
			auto path = std::string(Util::Strings::trim(match.getValue<std::string>()));

			unsigned int position;
			unsigned int code;
			std::smatch matchResult;

			if (std::regex_match(path, matchResult, positionAndCode)) {
				// The position and HTTP codes are present
				const std::string positionStr = matchResult[2];
				const std::string codeStr = matchResult[3];
				fromString(position, positionStr);
				fromString(code, codeStr);
				path = std::string(Util::Strings::trim(path.substr((unsigned long) matchResult[1].length())));
			} else if (std::regex_match(path, matchResult, codeOnly)) {
				// The HTTP code has been provided but no position.
				const std::string codeStr = matchResult[1];
				position = INT_MAX;
				fromString(code, codeStr);
				path = std::string(Util::Strings::trim(path.substr(codeStr.length())));
			} else {
				// No index or HTTP code present. Position undefined, default code of 302.
				position = INT_MAX;
				code = 302;
			}

			if (code != 301 && code != 302) {
				BalauBalauLogWarn(
					  logger
					, "Invalid redirect HTTP code provided ({}) for redirect \"{}\". Using default 302 instead."
					, code
					, regexStr
				);
			}

			try {
				std::regex matcher(regexStr);
				std::vector<RedirectingHttpWebApp::PathComponent> pathComponents = buildPathComponents(path);

				redirectionMap[position] = Redirection(
					matcher, code, pathComponents
					#ifdef BALAU_DEBUG
					, regexStr
					#endif
				);
			} catch (const std::regex_error &) {
				BalauBalauLogError(
					  logger
					, "Invalid regular expression \"{}\" specified in redirection HTTP web application."
					, match.getName()
				);

				// Fatal error.
				throw;
			}
		} else {
			BalauBalauLogWarn(
				  logger
				, "Ignoring match property entry {} because the property is not of type string."
				, match.getName()
			);
		}
	}

	std::vector<Redirection> paths;

	std::for_each(
		  redirectionMap.begin()
		, redirectionMap.end()
		, [&paths] (const std::map<size_t, Redirection>::value_type & pair) { paths.push_back(pair.second); }
	);

	return paths;
}

// TODO validation of configuration supplied path
std::vector<RedirectingHttpWebApp::PathComponent> RedirectingHttpWebApp::buildPathComponents(const std::string & path) {
	std::vector<RedirectingHttpWebApp::PathComponent> pathComponents;
	bool inVariable = false;
	std::string currentText;
	int currentEndOffset = 0;
	char32_t c;

	while (currentEndOffset < (int) path.length()) {
		c = Character::getNextUtf8(path, currentEndOffset);

		if (!inVariable) {
			if (c == '$') {
				if (!currentText.empty()) {
					// End of text component.
					pathComponents.push_back({ false, currentText });
					currentText = "";
				}

				inVariable = true;
				currentText += c;
			} else {
				currentText += c;
			}
		} else { // inVariable == true
			if (Character::isAlphaOrDecimal(c) || c == U'-' || c == U'_' || c == U'$') {
				// Continuing variable name.
				currentText += c;
			} else {
				// End of variable name.
				pathComponents.push_back({ true, currentText });
				currentText = "";
				inVariable = false;
				currentText += c;
			}
		}
	}

	if (!currentText.empty()) {
		// End of text or variable component.
		pathComponents.push_back({ inVariable, currentText });
	}

	return pathComponents;
}

} // namespace Balau::Network::Http::HttpWebApps
