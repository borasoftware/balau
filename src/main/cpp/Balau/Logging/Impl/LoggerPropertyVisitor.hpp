// @formatter:off
//
// Balau core C++ library
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

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR

#include <Balau/Lang/Property/Lang/PropertyAst.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Logging/Impl/LoggerConfigurationVisitor.hpp>

namespace Balau::LoggingSystem {

class LoggerPropertyVisitor : private Lang::Property::PropertyVisitor {
	private: struct LoggerPropertyAstPayload : public Lang::Property::Payload {
		LoggerTree loggerTree;
		LoggerTreeNode * currentLoggerTreeNode = nullptr;
	};

	public: LoggerTree visit(const Lang::Property::AST::Properties & properties) {
		LoggerPropertyAstPayload payload;
		visit(payload, properties);
		return payload.loggerTree;
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload); // NOLINT

		// Create root logger.
		pl.loggerTree.root().value = LoggerHolder(std::shared_ptr<Logger>(new Logger("", "", "")));

		for (auto & node : object.getNodes()) {
			node->visit(payload, *this);
		}
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload); // NOLINT

		if (!pl.currentLoggerTreeNode) {
			// TODO informative reporting
			fprintf(stderr, "WARNING: logging configuration value property defined outside of namespace.\n");
			return;
		}

		pl.currentLoggerTreeNode->value.getLogger()->properties[std::string(object.getName())] = std::string(object.getValue());
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload); // NOLINT

		// Only value properties may be defined inside a logging composite property.
		if (pl.currentLoggerTreeNode) {
			// TODO informative reporting
			fprintf(stderr, "WARNING: logging namespace composite property found inside another logging composite property.\n");
			return;
		}

		pl.currentLoggerTreeNode = &pl.loggerTree.root();

		LoggerConfigurationVisitor::determineLoggerTreeNode(object.getName(), pl.currentLoggerTreeNode);

		for (const auto & valueProperty : object.getNodes()) {
			valueProperty->visit(payload, *this);
		}

		pl.currentLoggerTreeNode = nullptr;
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) override {
		std::shared_ptr<Resource::Uri> resolvedUri = uris.top()->resolve(Util::Strings::trim(object.getText()));
		auto properties = Lang::Property::PropertyParserService::parse(resolvedUri);
		uris.push(resolvedUri);

		for (auto & node : properties.getNodes()) {
			node->visit(payload, *this);
		}

		uris.pop();
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) override {
		// NOP
	}

	private: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) override {
		// NOP
	}

	private: std::stack<std::shared_ptr<Resource::Uri>> uris;
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR
