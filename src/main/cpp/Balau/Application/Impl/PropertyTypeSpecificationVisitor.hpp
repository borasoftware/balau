// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
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
