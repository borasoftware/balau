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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR

#include <Balau/Application/Impl/EnvironmentConfigurationBuilderUtils.hpp>
#include <Balau/Application/Impl/PropertyBindingBuilderFactory.hpp>
#include <Balau/Application/Impl/PropertyString.hpp>
#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Lang/Property/Util/PropertyVisitor.hpp>
#include <Balau/Resource/UriResolve.hpp>

#include <numeric>

namespace Balau {

namespace Impl {

class PropertyTypeSpecificationVisitorPayload : public Lang::Property::Payload {
	public: std::stack<std::vector<PropertyBindingBuilderFactoryPtr>> nodeVectorStack;
	public: std::vector<Impl::PropertyBindingBuilderFactoryPtr> bindingBuilderFactoriesVector;
};

template <typename Unused = int> class PropertyTypeSpecificationVisitorUtilities {
	private: static std::regex defaultValueDelimiter;
	friend class PropertyTypeSpecificationVisitor;
};

template <typename Unused>
std::regex PropertyTypeSpecificationVisitorUtilities<Unused>::defaultValueDelimiter("\\s*=\\s*");

//
// Used to build the binding builder factories from a type specification property file source.
//
class PropertyTypeSpecificationVisitor : public Lang::Property::PropertyVisitor {
	public: PropertyTypeSpecificationVisitor(const std::shared_ptr<Resource::Uri> & uri_) {
		uris.push(uri_);
	}

	public: void execute(Lang::Property::Payload & payload) {
		auto properties = Lang::Property::PropertyParserService::parse(uris.top());
		visit(payload, properties);
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) {
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

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) {
		auto & pl = static_cast<PropertyTypeSpecificationVisitorPayload &>(payload);

		auto splitValue = Util::Strings::split(object.getValue(), PropertyTypeSpecificationVisitorUtilities<>::defaultValueDelimiter);

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

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) {
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

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) {
		std::shared_ptr<Resource::Uri> resolvedUri = Resource::UriResolve::resolve(*uris.top(), Util::Strings::trim(object.getText()));
		auto properties = Lang::Property::PropertyParserService::parse(resolvedUri);
		uris.push(resolvedUri);

		for (auto & node : properties.getNodes()) {
			node->visit(payload, *this);
		}

		uris.pop();
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) {
		// NOP
	}

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) {
		// NOP
	}

	private: std::stack<std::shared_ptr<Resource::Uri>> uris;
};

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR
