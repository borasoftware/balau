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

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR

#include <Balau/Application/Impl/PropertyBindingBuilderFactory.hpp>
#include <Balau/Application/Impl/PropertyString.hpp>
#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Lang/Property/Util/PropertyVisitor.hpp>

namespace Balau {

namespace Impl {

class PropertyTypeSpecificationVisitorPayload : public Lang::Property::Payload {
	public: std::stack<std::vector<PropertyBindingBuilderFactoryPtr>> nodeVectorStack;
	public: std::vector<Impl::PropertyBindingBuilderFactoryPtr> bindingBuilderFactoriesVector;
};

//
// Used to build the binding builder factories from a type specification property file source.
//
class PropertyTypeSpecificationVisitor : public Lang::Property::PropertyVisitor {
	public: PropertyTypeSpecificationVisitor(const std::shared_ptr<Resource::Uri> & uri_) {
		uris.push(uri_);
	}

	public: void execute(Lang::Property::Payload & payload);

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) override;

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) override;

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) override;

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) override;

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) override;

	public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) override;

	private: std::stack<std::shared_ptr<Resource::Uri>> uris;

	private: static std::regex defaultValueDelimiter;
};

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__PROPERTY_TYPE_SPECIFICATION_VISITOR
