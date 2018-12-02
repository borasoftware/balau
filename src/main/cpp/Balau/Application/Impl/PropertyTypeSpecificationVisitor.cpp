// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "PropertyTypeSpecificationVisitor.hpp"
#include "EnvironmentConfigurationBuilderUtils.hpp"
#include "../EnvironmentConfiguration.hpp"

#include <numeric>

namespace Balau {

namespace Impl {

std::regex PropertyTypeSpecificationVisitor::defaultValueDelimiter("\\s*=\\s*");

void PropertyTypeSpecificationVisitor::execute(Lang::Property::Payload & payload) {
	auto properties = Lang::Property::PropertyParserService::parse(uris.top());
	visit(payload, properties);
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) {
	auto & pl = static_cast<PropertyTypeSpecificationVisitorPayload &>(payload);
	pl.nodeVectorStack.push(std::vector<PropertyBindingBuilderFactoryPtr>());

	for (auto & node : object.getNodes()) {
		node->visit(payload, *this);
	}

	for (auto & factory : pl.nodeVectorStack.top()) {
		pl.bindingBuilderFactoriesVector.emplace_back(factory);
	}

	pl.nodeVectorStack.pop();
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) {
	auto & pl = static_cast<PropertyTypeSpecificationVisitorPayload &>(payload);

	auto splitValue = Util::Strings::split(object.getValue(), defaultValueDelimiter);

	std::string typeSpecification;
	std::string defaultValue;

	switch (splitValue.size()) {
		case 0: {
			ThrowBalauException(
				  Exception::EnvironmentConfigurationException
				, ::toString("Missing type specification for property ", object.getName(), " sourced from ", *uris.top(), ".")
			);
		}

		case 1: {
			typeSpecification = std::string(splitValue[0]);
			break;
		}

		case 2: {
			typeSpecification = std::string(splitValue[0]);
			defaultValue = std::string(splitValue[1]);
			break;
		}

		default: {
			// The multiple default value text contains one or more '='.. combine into a single piece of text.
			// It is up to the fromString function to determine if the text is value or not.
			typeSpecification = std::string(splitValue[0]);

			const size_t length = std::accumulate(
				splitValue.begin(), splitValue.end(), 0U, [] (size_t l, auto & s) { return l + s.length(); }
			) - splitValue[0].length();

			defaultValue = std::string(splitValue[1].data(), length);
		}
	}

	auto factory = getBindingBuilderFactory(typeSpecification, std::string(object.getName()), defaultValue);
	pl.nodeVectorStack.top().emplace_back(factory);
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) {
	auto & pl = static_cast<PropertyTypeSpecificationVisitorPayload &>(payload);
	pl.nodeVectorStack.push(std::vector<PropertyBindingBuilderFactoryPtr>());

	auto factory = PropertyBindingBuilderFactoryPtr(new CompositePropertyBindingBuilderFactory(std::string(object.getName())));
	auto composite = std::static_pointer_cast<CompositePropertyBindingBuilderFactory>(factory);

	for (auto & childNode : object.getNodes()) {
		childNode->visit(payload, *this);
	}

	for (auto & childFactory : pl.nodeVectorStack.top()) {
		EnvironmentConfigurationBuilderUtils::addToComposite(*composite, childFactory);
		EnvironmentConfigurationBuilderUtils::incrementLevel(childFactory);
	}

	pl.nodeVectorStack.pop();
	pl.nodeVectorStack.top().emplace_back(composite);
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) {
	std::shared_ptr<Resource::Uri> resolvedUri = uris.top()->resolve(Util::Strings::trim(object.getText()));
	auto properties = Lang::Property::PropertyParserService::parse(resolvedUri);
	uris.push(resolvedUri);
	visit(payload, properties);
	uris.pop();
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) {
	// NOP
}

void PropertyTypeSpecificationVisitor::visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) {
	// NOP
}

} // namespace Impl

} // namespace Balau
