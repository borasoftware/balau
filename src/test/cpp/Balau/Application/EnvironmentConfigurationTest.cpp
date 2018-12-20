// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "EnvironmentConfigurationTest.hpp"
#include "../../TestResources.hpp"

#include <Balau/Network/Http/Client/HttpClient.hpp>
#include <Balau/Network/Http/Server/HttpServer.hpp>
#include <Balau/Network/Http/Server/HttpWebApps/FileServingHttpWebApp.hpp>
#include <Balau/Network/Http/Server/WsWebApps/NullWsWebApp.hpp>
#include <Balau/Testing/Util/NetworkTesting.hpp>
#include <Balau/Resource/Https.hpp>
#include <Balau/System/SystemClock.hpp>
#include <Balau/Type/OnScopeExit.hpp>
#include <Balau/Util/Files.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

using namespace Balau::Network;
using namespace Balau::Network::Http;

using Balau::Testing::assertThat;
using Balau::Testing::is;
using Balau::Testing::isGreaterThan;
using Balau::Testing::throws;

namespace Balau {

void EnvironmentConfigurationTest::derivedEmpty() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {}
	};

	auto injector = Injector::create(EnvConfig(env));

	auto count = injector->getValue<std::string>("http.server.worker.count");

	assertThat(count, is("12"));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::derivedSimple() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {
			value<int>("http.server.worker.count");
		}
	};

	auto injector = Injector::create(EnvConfig(env));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::derivedComposite() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {
			value<int>("http.server.worker.count");

			group("file.serve"
				, value<std::string>("location")
				, unique<Resource::Uri>("document.root")
				, value<int>("cache.ttl")
				, group("a.b.c"
					, value<std::string>("d.e")
					, unique<Resource::Uri>("f")
				)
			);
		}
	};

	auto injector = Injector::create(EnvConfig(env));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(1200));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(*abc->getUnique<Resource::Uri>("f"), is(Resource::Https("https://borasoftware.com")));
}

void EnvironmentConfigurationTest::directEmpty() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "empty.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	auto injector = Injector::create(EnvironmentConfiguration(env, spec));
	auto count = injector->getValue<std::string>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is("12"));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::directSimple() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "simple.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	auto injector = Injector::create(EnvironmentConfiguration(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	// Present in type specification file without default, but not in env conf.
	assertThat([&fileServe] () { fileServe->getValue<double>("multiple"); }, throws<Exception::NoBindingException>());

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::directComposite() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	auto injector = Injector::create(EnvironmentConfiguration(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(1200));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(*abc->getUnique<Resource::Uri>("f"), is(Resource::Https("https://borasoftware.com")));
}

void EnvironmentConfigurationTest::directMultiple() {
	// TODO
}

void EnvironmentConfigurationTest::mixedEmpty() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "empty.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file, const Resource::File & spec)
			: EnvironmentConfiguration(file, spec) {}

		public: void configure() const override {}
	};

	auto injector = Injector::create(EnvConfig(env, spec));
	auto count = injector->getValue<std::string>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is("12"));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::mixedSimple() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "simple.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file, const Resource::File & spec)
			: EnvironmentConfiguration(file, spec) {}

		public: void configure() const override {
			value<int>("http.server.worker.count");
		}
	};

	auto injector = Injector::create(EnvConfig(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("1200"));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(abc->getValue<std::string>("f"), is("https://borasoftware.com"));
}

void EnvironmentConfigurationTest::mixedComposite() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env1.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file, const Resource::File & spec)
			: EnvironmentConfiguration(file, spec) {}

		public: void configure() const override {
			value<int>("http.server.worker.count");

			group("file.serve"
				, value<std::string>("location")
				, unique<Resource::Uri>("document.root")
				, value<int>("cache.ttl")
				, group("a.b.c"
					, value<std::string>("d.e")
					, unique<Resource::Uri>("f")
				)
			);
		}
	};

	auto injector = Injector::create(EnvConfig(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(12));
	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(1200));

	auto abc = fileServe->getComposite("a.b.c");

	assertThat(abc->getValue<std::string>("d.e"), is("hello, world"));
	assertThat(*abc->getUnique<Resource::Uri>("f"), is(Resource::Https("https://borasoftware.com")));
}

void EnvironmentConfigurationTest::mixedMultiple() {
	// TODO
}

void EnvironmentConfigurationTest::derivedSimpleWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {
			value<int>("http.server.worker.count", 32);
			value<double>("value.fraction", 0.432);
			value<double>("value.multiplier", 12.55e-3);
		}
	};

	auto injector = Injector::create(EnvConfig(env));

	assertThat(injector->getValue<int>("http.server.worker.count"), is(32));
	assertThat(injector->getValue<double>("value.fraction"), is(0.432));
	assertThat(injector->getValue<double>("value.multiplier"), is(12.55e-3));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));

	assertThat([&fileServe] () { fileServe->getValue<std::string>("cache.ttl"); }, throws<Exception::NoBindingException>());
}

void EnvironmentConfigurationTest::derivedCompositeWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {
			value<int>("http.server.worker.count", 16);

			group("file.serve"
				, value<std::string>("location")
				, unique<Resource::Uri>("document.root")
				, value<int>("cache.ttl", 3600)
				, group("a.b.c"
					, value<std::string>("d.e")
					, unique<Resource::Uri>("f")
				)
				, group("with.defaults1"
					, value<std::string>("message1", "hello1")
					, group("with.defaults2"
						, value<std::string>("message2", "hello2")
					)
				)
			);

			value<double>("value.fraction", 0.432);
			value<double>("value.multiplier", 12.55e-3);
		}
	};

	auto injector = Injector::create(EnvConfig(env));

	assertThat(injector->getValue<int>("http.server.worker.count"), is(16));
	assertThat(injector->getValue<double>("value.fraction"), is(0.432));
	assertThat(injector->getValue<double>("value.multiplier"), is(12.55e-3));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));

	assertThat(fileServe->getValue<int>("cache.ttl"), is(3600));

	assertThat([&fileServe] () { fileServe->getValue<std::string>("cache.ttl"); }, throws<Exception::NoBindingException>());
	assertThat([&fileServe] () { fileServe->getComposite("a.b.c"); }, throws<Exception::NoBindingException>());

	// Present in type specifications with default, but not in env conf.
	auto withDefaults1 = fileServe->getComposite("with.defaults1");

	assertThat(withDefaults1->getValue<std::string>("message1"), is("hello1"));

	auto withDefaults2 = withDefaults1->getComposite("with.defaults2");

	assertThat(withDefaults2->getValue<std::string>("message2"), is("hello2"));

	assertThat([&withDefaults1] () { withDefaults1->getValue<std::string>("message2"); }, throws<Exception::NoBindingException>());
}

void EnvironmentConfigurationTest::directSimpleWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "simple-with-defaults.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	auto injector = Injector::create(EnvironmentConfiguration(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(8));
	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));

	assertThat([&fileServe] () { fileServe->getUnique<std::string>("document.root"); }, throws<Exception::NoBindingException>());

	assertThat([&fileServe] () { fileServe->getValue<std::string>("cache.ttl"); }, throws<Exception::NoBindingException>());
}

void EnvironmentConfigurationTest::directCompositeWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite-with-defaults.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	auto injector = Injector::create(EnvironmentConfiguration(env, spec));
	auto count = injector->getValue<int>("http.server.worker.count");
	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(count, is(6));
	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));
	assertThat(fileServe->getValue<int>("cache.ttl"), is(10000));
}

void EnvironmentConfigurationTest::directMultipleWithDefaults() {
	// TODO
}

void EnvironmentConfigurationTest::mixedSimpleWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "simple-with-defaults.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file, const Resource::File & spec)
			: EnvironmentConfiguration(file, spec) {}

		public: void configure() const override {
			value<int>("http.server.worker.count", 32);
			value<double>("value.fraction", 0.432);
			value<double>("value.multiplier", 12.55e-3);
		}
	};

	auto injector = Injector::create(EnvConfig(env, spec));

	assertThat(injector->getValue<int>("http.server.worker.count"), is(32));
	assertThat(injector->getValue<double>("value.fraction"), is(0.432));
	assertThat(injector->getValue<double>("value.multiplier"), is(12.55e-3));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:doc"));

	assertThat([&fileServe] () { fileServe->getValue<int>("cache.ttl"); }, throws<Exception::NoBindingException>());
	assertThat([&fileServe] () { fileServe->getValue<std::string>("cache.ttl"); }, throws<Exception::NoBindingException>());
}

void EnvironmentConfigurationTest::mixedCompositeWithDefaults() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	const auto spec = resourceFolder / "Application" / "EnvironmentConfiguration" / "composite-with-defaults.thconf";
	const auto env = resourceFolder / "Application" / "EnvironmentConfiguration" / "env2.hconf";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file, const Resource::File & spec)
			: EnvironmentConfiguration(file, spec) {}

		public: void configure() const override {
			value<int>("http.server.worker.count", 16);

			group("file.serve"
				, value<std::string>("location")
				, unique<Resource::Uri>("document.root")
				, value<int>("cache.ttl") // This overrides the defaulted cache.ttl in the spec file.
				, group("a.b.c"
					, value<std::string>("d.e")
					, unique<Resource::Uri>("f")
				)
			);

			value<double>("value.fraction", 0.432);
			value<double>("value.multiplier", 12.55e-3);
		}
	};

	auto injector = Injector::create(EnvConfig(env, spec));

	assertThat(injector->getValue<int>("http.server.worker.count"), is(16));
	assertThat(injector->getValue<double>("value.fraction"), is(0.432));
	assertThat(injector->getValue<double>("value.multiplier"), is(12.55e-3));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/doc"));
	assertThat(*fileServe->getUnique<Resource::Uri>("document.root"), is(Resource::File("doc")));

	assertThat([&fileServe] () { fileServe->getValue<int>("cache.ttl"); }, throws<Exception::NoBindingException>());
	assertThat([&fileServe] () { fileServe->getValue<std::string>("cache.ttl"); }, throws<Exception::NoBindingException>());

	auto options = fileServe->getComposite("options");

	assertThat(options->getValue<std::string>("identity"), is("Balau Server"));
	assertThat(*options->getUnique<Resource::Uri>("404"), is(Resource::File("404.html")));
}

void EnvironmentConfigurationTest::mixedMultipleWithDefaults() {
	// TODO
}

void EnvironmentConfigurationTest::includedFilesConfig() {
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	Resource::File env = resourceFolder / "Application" / "EnvironmentConfiguration" / "hasDescendantInclude.properties";

	class EnvConfig : public EnvironmentConfiguration {
		public: EnvConfig(const Resource::File & file) : EnvironmentConfiguration(file) {}

		public: void configure() const override {}
	};

	auto injector = Injector::create(EnvConfig(env));

	assertThat(injector->getValue<std::string>("http.server.worker.count"), is("8"));

	auto fileServe = injector->getShared<EnvironmentProperties>("file.serve");

	assertThat(fileServe->getValue<std::string>("location"), is("/"));
	assertThat(fileServe->getValue<std::string>("document.root"), is("file:src/doc"));
	assertThat(fileServe->getValue<std::string>("cache.ttl"), is("3600"));
	assertThat(fileServe->getValue<std::string>("a.b.c"), is("test value"));

	auto abc = fileServe->getComposite("another.block");

	assertThat(abc->getValue<std::string>("first"), is("1"));
	assertThat(abc->getValue<std::string>("second"), is("2"));
	assertThat(abc->getValue<std::string>("third"), is("3"));
	assertThat(abc->getValue<std::string>("fourth"), is("4"));
}

void EnvironmentConfigurationTest::customConfigurationTypes() {
	const auto envSpec = Resource::StringUri("listen : endpoint");
	const auto env     = Resource::StringUri("listen = 192.168.1.23:4567");

	// Injector instantiation without custom type registration.
	assertThat(
		  [env, envSpec] () { Injector::create(EnvironmentConfiguration(env, envSpec)); }
		, throws<Exception::EnvironmentConfigurationException>()
	);

	EnvironmentConfiguration::registerValueType<Endpoint>("endpoint");

	auto injector = Injector::create(EnvironmentConfiguration(env, envSpec));

	assertThat(injector->getValue<Endpoint>("listen"), is(makeEndpoint("192.168.1.23", 4567U)));
}

void EnvironmentConfigurationTest::balauConfig() {
	// The Balau environment configuration property specifications.
	const auto srcFolder = TestResources::BalauSourceMainFolder;
	Resource::File specs = srcFolder / "resources" / "BalauConfig" / "balau.thconf";

	// Example environment.
	const auto resourceFolder = TestResources::BalauSourceTestResourcesFolder;
	Resource::File env = resourceFolder / "Application" / "EnvironmentConfiguration" / "testenv" / "env.hconf";
	Resource::File creds = resourceFolder / "Application" / "EnvironmentConfiguration" / "testenv" / "creds.hconf";

	// Register non-standard type.
	EnvironmentConfiguration::registerValueType<Endpoint>("endpoint");

	auto injector = Injector::create(EnvironmentConfiguration({ env, creds }, { specs }));

	auto httpServerConfig = injector->getShared<EnvironmentProperties>("http.server");

	assertThat(httpServerConfig->getValue<std::string>("info.log"), is("stream: file:access.log"));
	assertThat(httpServerConfig->getValue<std::string>("error.log"), is("stream: file:error.log"));
	assertThat(httpServerConfig->getValue<std::string>("server.id"), is("Http server"));
	assertThat(httpServerConfig->getValue<int>("worker.count"), is(2));
	assertThat(httpServerConfig->getValue<Network::Endpoint>("listen"), is(Network::makeEndpoint("127.0.0.1", 12345)));

	auto mimeTypes = httpServerConfig->getComposite("mime.types");

	assertThat(mimeTypes->getValue<std::string>("image/jpeg"),    is("jpg jpeg"));
	assertThat(mimeTypes->getValue<std::string>("image/png"),     is("png"));
	assertThat(mimeTypes->getValue<std::string>("image/svg+xml"), is("svg svgz"));
	assertThat(mimeTypes->getValue<std::string>("image/x-icon"),  is("ico"));
	assertThat(mimeTypes->getValue<std::string>("text/css"),      is("css"));
	assertThat(mimeTypes->getValue<std::string>("text/html"),     is("html htm"));
	assertThat(mimeTypes->getValue<std::string>("text/xml"),      is("xml bdml"));

	auto filters = httpServerConfig->getComposite("filters");

	// TODO
	assertThat(filters->begin() == filters->end(), is(true));

	auto httpConfig = httpServerConfig->getComposite("http");

	auto filesConfig = httpConfig->getComposite("files");

	assertThat(filesConfig->getValue<std::string>("location"), is("/"));
	assertThat(*filesConfig->getUnique<Resource::Uri>("root"), is(Resource::File("/var/www")));

	auto emailSenderConfig = httpConfig->getComposite("email.sender");

	assertThat(emailSenderConfig->getValue<std::string>("location"), is("/send-message"));
	assertThat(emailSenderConfig->getValue<std::string>("host"), is("smtp.do_not_use.com"));
	assertThat(emailSenderConfig->getValue<int>("port"), is(465));
	assertThat(emailSenderConfig->getValue<std::string>("user"), is("testuser"));
	assertThat(emailSenderConfig->getValue<std::string>("subject"), is("message"));
	assertThat(emailSenderConfig->getValue<std::string>("from"), is("testuser@do_not_use.com"));
	assertThat(emailSenderConfig->getValue<std::string>("to"), is("testuser@do_not_use.com"));
	assertThat(emailSenderConfig->getValue<std::string>("user-agent"), is("Test Agent"));
	assertThat(emailSenderConfig->getValue<std::string>("success"), is("/s/success.html"));
	assertThat(emailSenderConfig->getValue<std::string>("failure"), is("/f/failed.html"));

	auto parametersConfig = emailSenderConfig->getComposite("parameters");

	assertThat(parametersConfig->getValue<std::string>("Name"), is("1"));
	assertThat(parametersConfig->getValue<std::string>("Email"), is("2"));
	assertThat(parametersConfig->getValue<std::string>("Message"), is("3"));
}

} // namespace Balau

#pragma clang diagnostic pop
