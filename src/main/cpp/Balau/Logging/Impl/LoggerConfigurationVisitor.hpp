// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_CONFIGURATION_VISITOR
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_CONFIGURATION_VISITOR

#include <Balau/Application/EnvironmentProperties.hpp>
#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Property/Lang/PropertyAst.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Logging/Impl/LoggerHolder.hpp>

namespace Balau::LoggingSystem {

//
// Whilst not a real double dispatch visitor as such, the logger configuration visitor
// nevertheless mirrors the LoggerPropertyVisitor's functionality for environment configuration
// supplied logging configuration.
//
class LoggerConfigurationVisitor {
	public: LoggerTree visit(const EnvironmentProperties & object) {
		// Create root logger.
		loggerTree.root().value = LoggerHolder(std::shared_ptr<Logger>(new Logger("", "", "")));

		for (auto item : object) {
			if (item.isComposite()) {
				visitComposite(item.getName(), *item.getComposite());
			} else {
				// TODO informative reporting
				fprintf(stderr, "WARNING: logging configuration value property defined outside of namespace.\n");
			}
		}

		return loggerTree;
	}

	private: void visitStringValue(const std::string_view & name, const std::string & value) {
		currentLoggerTreeNode->value.getLogger()->properties[std::string(name)] = value;
	}

	// Boolean values in the configuration are just for validation. The property configuration
	// system currently uses strings, so the boolean is temporarily converted back here.
	private: void visitBooleanValue(const std::string_view & name, bool value) {
		currentLoggerTreeNode->value.getLogger()->properties[std::string(name)] = ::toString(value);
	}

	private: void visitComposite(const std::string_view & name, const EnvironmentProperties & object) {
		currentLoggerTreeNode = &loggerTree.root();

		determineLoggerTreeNode(name, currentLoggerTreeNode);

		for (auto item : object) {
			if (item.isComposite()) {
				// Only value properties may be defined inside a logging composite property.
				if (currentLoggerTreeNode) {
					// TODO informative reporting
					fprintf(stderr, "WARNING: logging namespace composite property found inside another logging composite property.\n");
					return;
				}
			} else if (item.isValue<std::string>()) {
				visitStringValue(item.getName(), item.getValue<std::string>());
			} else if (item.isValue<bool>()) {
				visitBooleanValue(item.getName(), item.getValue<bool>());
			} else {
				// TODO informative reporting
				fprintf(stderr, "WARNING: unknown property type found in logging configuration.\n");
			}
		}

		currentLoggerTreeNode = nullptr;
	}

	friend class LoggerPropertyVisitor;

	//
	// Given the root node pointer reference, descend into the trie, finding
	// or creating children according to the supplied logging namespace.
	//
	private: static void determineLoggerTreeNode(std::string_view loggingNamespace, LoggerTreeNode * & loggerTreeNode) {
		const std::vector<std::string_view> nameSpace = Util::Strings::splitAndTrim(loggingNamespace, ".");

		if (!nameSpace.empty()) {
			std::string nameSpaceStr;
			std::string ns;
			std::string prefix;

			for (size_t m = 0; m < nameSpace.size(); m++) {
				std::string identifier = std::string(nameSpace[m]);
				nameSpaceStr += prefix + identifier;
				std::string thisNs = ns + prefix + (m < nameSpace.size() - 1 ? identifier.substr(0, 1) : identifier);
				ns += prefix + identifier.substr(0, 1);
				prefix = ".";
				loggerTreeNode = &loggerTreeNode->findOrAddChild(
					LoggerHolder(
						std::shared_ptr<Logger>(
							new Logger(std::move(identifier), std::string(nameSpaceStr), std::move(thisNs))
						)
					)
				);
			}
		}
	}

	private: LoggerTree loggerTree;
	private: LoggerTreeNode * currentLoggerTreeNode = nullptr;
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_CONFIGURATION_VISITOR
