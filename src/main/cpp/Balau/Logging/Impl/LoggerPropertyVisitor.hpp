// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR
#define COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Property/Lang/PropertyAst.hpp>
#include <Balau/Lang/Property/PropertyParserService.hpp>
#include <Balau/Logging/Impl/LoggerHolder.hpp>

namespace Balau::LoggingSystem {

class LoggerPropertyAstPayload : public Lang::Property::Payload {
	public: LoggerTree configuration;
	public: LoggerTreeNode * currentCompositeNode = nullptr;
};

class LoggerPropertyVisitor : public Lang::Property::PropertyVisitor {
	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload);

		// Create root logger.
		pl.configuration.root().value = LoggerHolder(std::shared_ptr<Logger>(new Logger("", "", "")));

		for (auto & node : object.getNodes()) {
			node->visit(payload, *this);
		}
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload);

		if (!pl.currentCompositeNode) {
			// TODO location reporting
			fprintf(stderr, "WARNING: logging configuration value property defined outside of namespace.\n");
			return;
		}

		pl.currentCompositeNode->value.getLogger()->properties[std::string(object.getName())] = std::string(object.getValue());
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) override {
		auto & pl = static_cast<LoggerPropertyAstPayload &>(payload);

		// Only value properties may be defined inside a logging composite property.
		if (pl.currentCompositeNode) {
			// TODO location reporting
			fprintf(stderr, "WARNING: logging namespace composite property found inside another logging composite property.\n");
			return;
		}

		pl.currentCompositeNode = &pl.configuration.root();

		const std::vector<std::string_view> nameSpace = Util::Strings::splitAndTrim(object.getName(), ".");

		// Create the logger unless the namespace is the root.
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
				pl.currentCompositeNode = &pl.currentCompositeNode->findOrAddChild(
					LoggerHolder(
						std::shared_ptr<Logger>(
							new Logger(std::move(identifier), std::string(nameSpaceStr), std::move(thisNs))
						)
					)
				);
			}
		}

		for (const auto & valueProperty : object.getNodes()) {
			valueProperty->visit(payload, *this);
		}

		pl.currentCompositeNode = nullptr;
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) override {
		std::shared_ptr<Resource::Uri> resolvedUri = uris.top()->resolve(Util::Strings::trim(object.getText()));
		auto properties = Lang::Property::PropertyParserService::parse(resolvedUri);
		uris.push(resolvedUri);

		for (auto & node : properties.getNodes()) {
			node->visit(payload, *this);
		}

		uris.pop();
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) override {
		// NOP
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) override {
		// NOP
	}

	private: std::stack<std::shared_ptr<Resource::Uri>> uris;
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE__BALAU_LOGGING_IMPL__LOGGER_PROPERTY_VISITOR
