// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "EnvironmentConfigurationBuilderTest.hpp"
#include "../../../TestResources.hpp"

#include <Balau/Application/Impl/EnvironmentConfigurationBuilder.hpp>
#include <Balau/Resource/Https.hpp>

using Balau::Testing::assertThat;
using Balau::Testing::is;
using Balau::Testing::isGreaterThan;
using Balau::Testing::throws;

namespace Balau {

namespace Impl {

using BuilderVector                          = EnvironmentConfigurationBuilder::BuilderVector;
using PropertyBindingBuilderFactoryPtrVector = EnvironmentConfigurationBuilder::PropertyBindingBuilderFactoryPtrVector;
using PropertyStringsHierarchy               = EnvironmentConfigurationBuilder::PropertyStringHierarchy;

void EnvironmentConfigurationBuilderTest::empty() {
	const std::vector<std::shared_ptr<Resource::Uri>> typeSpecificationsUris;
	const PropertyBindingBuilderFactoryPtrVector derivedBindingBuilderFactoriesVector;
	const PropertyStringsHierarchy propertyStringsHierarchy;

	BuilderVector actual = EnvironmentConfigurationBuilder::build(
		typeSpecificationsUris, derivedBindingBuilderFactoriesVector, propertyStringsHierarchy
	);

	assertThat(actual.empty(), is(true));
}

void EnvironmentConfigurationBuilderTest::mixed() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	std::vector<std::shared_ptr<Resource::Uri>> typeSpecificationsUris;
	typeSpecificationsUris.emplace_back(spec.clone());

	PropertyBindingBuilderFactoryPtrVector factories;

	factories.emplace_back(
		ValuePropertyBindingBuilderFactory<int>::makePtr("http.server.worker.count")
	);

	factories.emplace_back(
		CompositePropertyBindingBuilderFactory::makePtr("file.serve"
			, ValuePropertyBindingBuilderFactory<std::string>::makePtr("location")
			, UniquePropertyBindingBuilderFactory<Resource::Uri>::makePtr("document.root")
			, ValuePropertyBindingBuilderFactory<int>::makePtr("cache.ttl")
			, CompositePropertyBindingBuilderFactory::makePtr("a.b.c"
				, ValuePropertyBindingBuilderFactory<std::string>::makePtr("d.e")
				, UniquePropertyBindingBuilderFactory<Resource::Uri>::makePtr("f")
			)
		)
	);

	auto propertyStrings = EnvironmentConfigurationBuilderUtils::createPropertyStrings(env.clone());
	auto builders = EnvironmentConfigurationBuilder::build(typeSpecificationsUris, factories, propertyStrings);

	auto injector = Injector::create(EnvironmentConfiguration{std::move(builders)});
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(1200));

	auto abc = fileServe->getShared("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(*abc->getUnique<Resource::Uri>("f"), is(Resource::Https("https://borasoftware.com")));
}

void EnvironmentConfigurationBuilderTest::mixedWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite-with-defaults.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	std::vector<std::shared_ptr<Resource::Uri>> typeSpecificationsUris;
	typeSpecificationsUris.emplace_back(spec.clone());

	PropertyBindingBuilderFactoryPtrVector factories = Util::Vectors::pushBack(
		  ValuePropertyBindingBuilderFactory<int>::makePtr("http.server.worker.count", 16)
		, ValuePropertyBindingBuilderFactory<double>::makePtr("value.multiplier", 12.55e-3)

		, CompositePropertyBindingBuilderFactory::makePtr("file.serve"
			, ValuePropertyBindingBuilderFactory<std::string>::makePtr("location", "/")
			, ValuePropertyBindingBuilderFactory<int>::makePtr("cache.ttl", 3600)
			, CompositePropertyBindingBuilderFactory::makePtr("options"
				, ValuePropertyBindingBuilderFactory<std::string>::makePtr("identity", "My Server")
			)
		)

		, ValuePropertyBindingBuilderFactory<double>::makePtr("value.fraction", 0.432)
	);

	auto propertyStrings = EnvironmentConfigurationBuilderUtils::createPropertyStrings(env.clone());
	auto builders = EnvironmentConfigurationBuilder::build(typeSpecificationsUris, factories, propertyStrings);

	auto injector = Injector::create(EnvironmentConfiguration{std::move(builders)});

	assertThat(injector->getValue<int>("http.server.worker.count"), is(16));
	assertThat(injector->getValue<double>("value.multiplier"), is(12.55e-3));
	assertThat(injector->getValue<double>("value.fraction"), is(0.432));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(3600));

	auto abc = fileServe->getShared("options");

	assertThat(abc->getValue<std::string>("identity"), is("My Server"));
	assertThat(*abc->getUnique<Resource::Uri>("404"), is(Resource::File("404.html")));
}

} // namespace Impl

} // namespace Balau
