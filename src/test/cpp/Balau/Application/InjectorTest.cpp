// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "InjectorTest.hpp"

#include "../../../../main/cpp/Balau/Application/Injectable.hpp"

#include <thread>

using Balau::Testing::is;
using Balau::Testing::isGreaterThanOrEqualTo;
using Balau::Testing::isNot;
using Balau::Testing::throws;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace Balau {

////////////////////////////// Test state capture /////////////////////////////

//#define PRINT_CAPTURE

class TestCapture {
	private: std::vector<std::string> capturedText;
	private: size_t count = 0;

	public: void add(const std::string & str) {
		capturedText.emplace_back(str);
	}

	#ifdef PRINT_CAPTURE
		public: void printLatest(const std::string & title, InjectorTest * testInstance) {
			testInstance->logLine(title);

			for (size_t m = count; m < capturedText.size(); m++) {
				testInstance->logLine(std::string("  ") + capturedText[m]);
			}

			count = capturedText.size();
		};
	#else
		public: void printLatest(const std::string & , InjectorTest * ) {
		};
	#endif

	public: void assertSize(size_t expectedSize) {
		AssertThat(capturedText.size(), is(expectedSize));
	}

	public: void assertEquals(std::vector<std::string> expectedStrings) {
		AssertThat(capturedText.size(), is(expectedStrings.size()));

		for (size_t m = count; m < expectedStrings.size(); m++) {
			AssertThat(capturedText[m], is(expectedStrings[m]));
		}
	}

	public: void assertStartsWith(std::vector<std::string> expectedStrings) {
		AssertThat(capturedText.size(), isGreaterThanOrEqualTo(expectedStrings.size()));

		for (size_t m = count; m < expectedStrings.size(); m++) {
			AssertThat(capturedText[m], is(expectedStrings[m]));
		}
	}

	public: void clear() {
		capturedText.clear();
		count = 0;
	}
};

thread_local TestCapture capture;

///////////////////////////////// Interfaces //////////////////////////////////

class Base {
	public: virtual ~Base() = default;
	public: virtual void foo() = 0;
};

class Base2 {
	public: virtual ~Base2() = default;
	public: virtual void foo2() = 0;
};

/////////////////////////////// Implementations ///////////////////////////////

class DerivedA : public Base {
	BalauInject(DerivedA)

	public: ~DerivedA() override = default;

	public: explicit DerivedA(std::string aName) : name(std::move(aName)) {
		capture.add("DerivedA constructor called manually");
	}

	private: DerivedA() {
		capture.add("DerivedA constructor");
	}

	public: void foo() override {
		if (!name.empty()) {
			capture.add(std::string("DerivedA.foo with name \"") + name + "\"");
		} else {
			capture.add("DerivedA.foo");
		}
	}

	private: const std::string name;
};

class DerivedB : public Base {
	BalauInject(DerivedB)

	public: ~DerivedB() override = default;

	private: DerivedB() {
		capture.add("DerivedB constructor");
	}

	public: void foo() override {
		capture.add("DerivedB.foo");
	}
};

class Derived2 : public Base2 {
	BalauInjectTypes(Derived2, std::shared_ptr<Base>)

	private: explicit Derived2(std::shared_ptr<Base> aDependency)
		: dependency(std::move(aDependency)) {
		capture.add("explicit Derived2 constructor");
	}

	public: ~Derived2() override = default;

	public: void foo2() override {
		capture.add("Derived2.foo2");
		dependency->foo();
	}

	private: std::shared_ptr<Base> dependency;
};

class Derived2WithNamed : public Base2 {
	BalauInjectNamedTypes(Derived2WithNamed, std::shared_ptr<Base>, "namedBase")

	private: explicit Derived2WithNamed(std::shared_ptr<Base> aDependency)
		: dependency(std::move(aDependency)) {
		capture.add("Derived2WithNamed constructor");
	}

	public: ~Derived2WithNamed() override = default;

	public: void foo2() override {
		capture.add("Derived2WithNamed.foo2");
		dependency->foo();
	}

	private: std::shared_ptr<Base> dependency;
};

class Derived2Custom : public Base2 {
	BalauInject(Derived2Custom)

	private: Derived2Custom() {
		capture.add("Derived2Custom constructor");
	}

	public: ~Derived2Custom() override = default;

	public: void foo2() override {
		capture.add("Derived2Custom.foo2");
	}
};

class Derived2Custom2 : public Base2 {
	BalauInject(Derived2Custom2)

	private: Derived2Custom2() {
		capture.add("Derived2Custom2 constructor");
	}

	public: ~Derived2Custom2() override = default;

	public: void foo2() override {
		capture.add("Derived2Custom2.foo2");
	}
};

class Derived2Custom3 : public Base2 {
	BalauInject(Derived2Custom3)

	private: Derived2Custom3() {
		capture.add("Derived2Custom3 constructor");
	}

	public: ~Derived2Custom3() override = default;

	public: void foo2() override {
		capture.add("Derived2Custom3.foo2");
	}
};

//////////////////////////////////// Tests ////////////////////////////////////

void InjectorTest::creationAndDeletion() {
	capture.clear();

	// Create a configuration.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>().toSingleton<DerivedA>();
			bind<Base2>().toUnique<Derived2>();
		}
	};

	// Instantiate the injector.
	auto injector = Injector::create(Configuration());

	// Create a child configuration.
	class ChildConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom>();
		}
	};

	// Create a child injector.
	auto childInjector = injector->createChild(ChildConfiguration());

	// Create a binding in the custom scope

	//
	// Request three instances of Base2 from the injector.
	//
	// First, DerivedA will automatically be instantiated as a singleton because
	// it is a dependency of Derived2. Then Derived2 will be instantiated.
	//
	// Then, after the custom scope is pushed, Derived2Custom will take precedence
	// and will be instantiated.
	//
	// Then, after the custom scope is popped, Derived2 will be instantiated
	// again. Note that the dependency DerivedA is not instantiated again because
	// it is a singleton.
	//

	auto b1 = injector->getUnique<Base2>();

	b1->foo2();

	capture.printLatest("\n\n---------- start ----------\n", this);

	auto b2 = childInjector->getShared<Base2>();
	b2->foo2();

	capture.printLatest("\n---- push custom scope ----\n", this);

	auto b3 = injector->getUnique<Base2>();
	b3->foo2();

	capture.printLatest("\n---- pop custom scope -----\n\n", this);

//	capture.assertEquals({
//		  "DerivedA constructor"
//		, "explicit Derived2 constructor"
//		, "Derived2.foo2"
//		, "DerivedA.foo"
//
//		, "Derived2Custom constructor"
//		, "Derived2Custom.foo2"
//
//		, "explicit Derived2 constructor"
//		, "Derived2.foo2"
//		, "DerivedA.foo"
//	});
}

void InjectorTest::singletonCreation() {
	capture.clear();

	// Create a configuration.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>().toEagerSingleton<DerivedA>();
			bind<Base2>().toUnique<Derived2>();
		}
	};

	// Instantiate the injector.
	auto injector = Injector::create(Configuration());

	auto b2 = injector->getUnique<Base2>();
	auto b = injector->getShared<Base>();

	b2->foo2();
	b->foo();

	// Both objects still work correctly.
	b2->foo2();
	b->foo();

	// The singleton object still works correctly.
	b->foo();

	auto b3 = injector->getUnique<Base2>();
	b3->foo2();
}

void InjectorTest::simpleTypeInjection() {
	capture.clear();

	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<std::ostream>("cout").toReference(std::cout);
			bind<Base>().toSingleton<DerivedA>();
			bind<Base2>().toUnique<Derived2>();
			bind<Base>("namedIBase").toSingleton(std::make_shared<DerivedA>("myobj"));
		}
	};

	// Instantiate the injector.
	auto injector = Injector::create(Configuration());

	auto b2 = injector->getUnique<Base2>();
	b2->foo2();

	auto b = injector->getShared<Base>();
	b->foo();

	auto & output = injector->getReference<std::ostream>("cout");

	output << "write to stdout via injector obtained reference\n";

	auto namedIBase = injector->getShared<Base>("namedIBase");
	namedIBase->foo();
}

void InjectorTest::multipleCustomScopes() {
	capture.clear();

	// Create a configuration.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>().toSingleton<DerivedA>();
			bind<Base2>().toUnique<Derived2>();
		}
	};

	// Instantiate the injector configuration and the injector.
	auto injector = Injector::create(Configuration());

	// Create multiple child injectors in a hierarchy.

	class FirstConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom>();
		}
	};

	class SecondConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom2>();
			bind<Base>().toEagerSingleton<DerivedB>();
		}
	};

	class ThirdConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom3>();
		}
	};

	auto firstChildInjector = injector->createChild(FirstConfiguration());
	auto secondChildInjector = firstChildInjector->createChild(SecondConfiguration());
	auto thirdChildInjector = secondChildInjector->createChild(ThirdConfiguration());

	//
	// Request four instances of Base2 from the injector.
	//
	// From the root injector, DerivedA will automatically be instantiated as a singleton
	// because it is a dependency of Derived2. Then Derived2 will be instantiated.
	//
	// From the first child injector, after the first custom scope is pushed, Derived2Custom
	// will take precedence and will be instantiated.
	//
	// From the second child injector, after the second custom scope is pushed, Derived2Custom2
	// will take precedence and will be instantiated.
	//
	// From the third child injector, , after the third custom scope is pushed, Derived2Custom3
	// will take precedence and will be instantiated.
	//
	//
	// TODO COMPLETE DESIGN CHANGE TESTING
	//
	// The three custom scopes are popped. Then the third custom scope is pushed
	// again.
	//
	// Then, Derived2Custom3 will be instantiated again for Base2 and a DerivedB
	// object will be supplied for Base. This illustrates that the custom scope
	// hierarchy created during the instantiation of the custom scopes is present,
	// even though the other two scopes have not been manually pushed onto the
	// scope stack before pushing the third custom scope.
	//
	// The third custom scope is then popped.
	//
	// Then, Derived2 will be instantiated again for Base2 and the original DerivedA
	// object will be supplied for Base.
	//

	auto b1 = injector->getUnique<Base2>();
	b1->foo2();
	capture.printLatest("\n\n---------- start ----------\n", this);

	auto b2 = firstChildInjector->getShared<Base2>();
	b2->foo2();
	capture.printLatest("\n---- first child injector ----\n", this);

	auto b3 = secondChildInjector->getShared<Base2>();
	b3->foo2();
	capture.printLatest("\n---- second child injector ----\n", this);

	auto b4 = thirdChildInjector->getShared<Base2>();
	b4->foo2();
	capture.printLatest("\n---- third child injector ----\n", this);

//	capture.assertEquals({
//		  "DerivedA constructor"
//		, "explicit Derived2 constructor"
//		, "Derived2.foo2"
//		, "DerivedA.foo"
//		, "Derived2Custom constructor"
//		, "Derived2Custom.foo2"
//		, "Derived2Custom2 constructor"
//		, "Derived2Custom2.foo2"
//		, "Derived2Custom3 constructor"
//		, "Derived2Custom3.foo2"
//		, "DerivedB constructor"
//		, "Derived2Custom2.foo2" // TODO
//		, "DerivedB.foo"
//	});
}

void InjectorTest::namedDependency() {
	capture.clear();

	// Create a configuration.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>("namedBase").toSingleton<DerivedA>();
			bind<Base>().toSingleton<DerivedB>();
			bind<Base2>().toSingleton<Derived2WithNamed>();
		}
	};

	// Instantiate the injector configuration and the injector.
	auto injector = Injector::create(Configuration());

	//
	// Request an instance of Base2 from the injector.
	//
	// DerivedA will automatically be instantiated as a singleton because it is
	// a named dependency of Derived2WithNamed. Then Derived2WithNamed will be
	// instantiated.
	//
	// Then request an instance of Base from the injector. DerivedB will be
	// instantiated.
	//
	// Then request a named instance of Base from the injector. The previously
	// instantiated DerivedA singleton will be returned.
	//

	auto b1 = injector->getShared<Base2>();
	b1->foo2();

	auto b2 = injector->getShared<Base>();
	b2->foo();

	auto b3 = injector->getShared<Base>("namedBase");
	b3->foo();

	capture.printLatest("\n\n", this);

	capture.assertEquals(
		{
			  "DerivedA constructor"
			, "Derived2WithNamed constructor"
			, "Derived2WithNamed.foo2"
			, "DerivedA.foo"
			, "DerivedB constructor"
			, "DerivedB.foo"
			, "DerivedA.foo"
		}
	);
}

void InjectorTest::eagerSingletons() {
	capture.clear();

	// Create a configuration with eager singletons instead of lazy ones.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>("namedBase").toEagerSingleton<DerivedA>();
			bind<Base>().toEagerSingleton<DerivedB>();
			bind<Base2>().toEagerSingleton<Derived2WithNamed>();
		}
	};

	// Instantiate the injector configuration and the injector.
	auto injector = Injector::create(Configuration());

	// All bindings are instantiated before any call to getShared.

	capture.printLatest("\n\n", this);

//	capture.assertEquals(
//		{
//			  "DerivedA constructor"
//			, "DerivedB constructor"
//			, "Derived2WithNamed constructor"
//		}
//	);
}

void InjectorTest::runtimeCustomScopes() {
	capture.clear();

	// Create a configuration with no custom scopes.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>().toSingleton<DerivedA>();
			bind<Base2>().toUnique<Derived2>();
		}
	};

	// Instantiate the injector configuration and the injector.
	auto injector = Injector::create(Configuration());

	//
	// Request four instances of Base2 from the injector.
	//
	// First, DerivedA will automatically be instantiated as a singleton because
	// it is a dependency of Derived2. Then Derived2 will be instantiated.
	//
	// Then, after the first custom scope is pushed, Derived2Custom will take
	// precedence and will be instantiated.
	//
	// Then, after the second custom scope is pushed, Derived2Custom2 will take
	// precedence and will be instantiated.
	//
	// Then, after the third custom scope is pushed, Derived2Custom3 will take
	// precedence and will be instantiated.
	//
	// The three custom scopes are popped. Then the third custom scope is pushed
	// again.
	//
	// Then, Derived2Custom3 will be instantiated again for Base2 and a DerivedB
	// object will be supplied for Base. This illustrates that the custom scope
	// hierarchy created during the instantiation of the custom scopes is present,
	// even though the other two scopes have not been manually pushed onto the scope
	// stack before pushing the third custom scope.
	//
	// The third custom scope is then popped.
	//
	// Then, Derived2 will be instantiated again for Base2 and the original DerivedA
	// object will be supplied for Base.
	//

	auto b1 = injector->getUnique<Base2>();
	b1->foo2();
	capture.printLatest("\n\n---------- start ----------\n", this);

	// Create custom scope.

	class FirstConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom>();
		}
	};

	auto firstChildInjector = injector->createChild(FirstConfiguration());

	auto b2 = firstChildInjector->getShared<Base2>();
	b2->foo2();
	capture.printLatest("\n---- push first custom scope ----\n", this);


	class SecondConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom2>();
			bind<Base>().toEagerSingleton<DerivedB>();
		}
	};

	class ThirdConfiguration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base2>().toEagerSingleton<Derived2Custom3>();
		}
	};

	auto secondChildInjector = firstChildInjector->createChild(SecondConfiguration());
	auto thirdChildInjector = secondChildInjector->createChild(ThirdConfiguration());

	auto b3 = secondChildInjector->getShared<Base2>();
	b3->foo2();
	capture.printLatest("\n---- push second custom scope ----\n", this);

	auto b4 = thirdChildInjector->getShared<Base2>();
	b4->foo2();
	capture.printLatest("\n---- push third custom scope ----\n", this);

//	injector->pushScope(thirdScope);
//
//	auto b5 = injector->getShared<Base2>();
//	auto b6 = injector->getShared<Base>();
//	b3->foo2();
//	b6->foo();

	capture.printLatest("\n---- popped all and push third custom scope again -----\n\n", this);

//	capture.assertEquals(
//		{
//			  "DerivedA constructor"
//			, "explicit Derived2 constructor"
//			, "Derived2.foo2"
//			, "DerivedA.foo"
//			, "Derived2Custom constructor"
//			, "Derived2Custom.foo2"
//			, "Derived2Custom2 constructor"
//			, "Derived2Custom2.foo2"
//			, "Derived2Custom3 constructor"
//			, "Derived2Custom3.foo2"
//			, "DerivedB constructor"
//			, "Derived2Custom2.foo2"
//			, "DerivedB.foo"
//		}
//	);
}

void InjectorTest::threadLocalScopeUsage() {
	capture.clear();

	// Create a configuration.
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<Base>().toSingleton<DerivedA>();
			bind<Base2>().toThreadLocal<Derived2>();
		}
	};

	// Instantiate the injector.
	auto injector = Injector::create(Configuration());

	//
	// Request multiple instances of Base2 from the injector, each in different
	// threads.
	//
	// First, DerivedA will automatically be instantiated as a singleton because
	// it is a dependency of Derived2. Then different instances of Derived2 will
	// be instantiated in each thread.
	//
	// Then request the same instances again in the threads. The same instances
	// will be supplied to each thread.
	//

	class ThreadFunction {
		public: std::shared_ptr<Base2> instance1;
		public: std::shared_ptr<Base2> instance2;

		public: static void threadFunction(ThreadFunction & self, const std::shared_ptr<Injector> & injector) {
			self.run(injector);
		}

		public: void run(const std::shared_ptr<Injector> & injector) {
			instance1 = injector->getShared<Base2>();
			instance2 = injector->getShared<Base2>();
		}
	};

	const size_t threadCount = 50;
	std::array<ThreadFunction, threadCount> threadFunctions;
	std::array<std::thread, threadCount> threads;

	for (size_t m = 0; m < threadFunctions.size(); m++) {
		threads[m] = std::thread(ThreadFunction::threadFunction, std::ref(threadFunctions[m]), std::ref(injector));
	}

	for (auto & thread : threads) {
		thread.join();
	}

	// The two instances of a thread function are the same.
	for (auto & threadFunction : threadFunctions) {
		AssertThat(threadFunction.instance1.get(), is(threadFunction.instance2.get()));
	}

	// The instances of different thread functions are different.
	for (size_t m = 0; m < threadFunctions.size() - 1; m++) {
		AssertThat(threadFunctions[m].instance1.get(), isNot(threadFunctions[m + 1].instance1.get()));
	}

	// Nothing is created on this thread.
	capture.assertSize(0);
}

///////////////////////////////////////////////////////////////////////////////

struct CT {
	BalauInject(CT);
};

class FloatProvider {
	BalauInject(FloatProvider);

	public: float operator () () {
		return 432.1f;
	}
};

struct I {
	virtual int getValue() = 0;
	virtual ~I() = default;
};

int cCounter = 0;

struct C : public I {
	BalauInject(C, value);

	int value;

	explicit C(int value_) : value(value_) {}

	int getValue() override {
		return value;
	}
};

struct J {
	virtual int getValue() = 0;
	virtual ~J() = default;
};

struct Counter {
	BalauInject(Counter);

	std::atomic_int count = 100;
};

struct D : public J {
	int value;

	BalauInjectConstruct(D, value);

	int getValue() override {
		return value;
	}
};

class UProvider {
	public: std::unique_ptr<J> operator () () {
		return std::unique_ptr<J>(new D(counter->count++));
	}

	std::shared_ptr<Counter> counter;

	BalauInjectConstruct(UProvider, counter);
};

int sCounter = 0;

struct S {
	explicit S() : value(++sCounter) {}

	int getValue() {
		return value;
	}

	int value;

	BalauInject(S);
};

std::atomic_int tli;

struct TL {
	explicit TL() : value(++tli) {}

	int getValue() {
		return value;
	}

	int value;

	BalauInject(TL);
};


struct A {
	CT ct;
	std::string s;
	int i;
	char g;
	double d;
	float f;
	std::unique_ptr<Base2> b;
	std::unique_ptr<I> ii;
	std::unique_ptr<J> jj;
	std::shared_ptr<Base> sb;
	std::shared_ptr<TL> tl;
	std::shared_ptr<S> ls;

	BalauInjectConstruct(A, ct, s, i, g, d, f, b, ii, jj, sb, tl, ls);
};

struct B {
	std::ostream & stream;

	BalauInjectConstructNamed(B, stream, "testStream");
};

struct CC {
	double value;

	explicit CC(double value_) : value(value_) {}

	// No copy constructor, so the type of the lhs of the getReference
	// call to the injector must be auto &, not auto.
	CC(const CC &) = delete;
};

struct CCS {
	double value;

	// No copy constructor, so the type of the lhs of the getReference
	// call to the injector must be auto &, not auto.
	CCS(const CCS &) = delete;

	BalauInjectConstruct(CCS, value);
};

struct CD {
	std::shared_ptr<const CCS> ccsObj;

	BalauInjectConstruct(CD, ccsObj);
};

// This object is bound to a reference binding.
// The lifetime of the object must be longer than the injector lifetime.
std::ostringstream referencedStream;

const CC cc(3.14159);

void InjectorTest::allBindings() {
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			// Bind a concrete type.
			bind<CT>().toValue();

			// Bind a concrete type to a prototype value.
			bind<std::string>().toValue("test string");
			bind<int>().toValue(1234567);
			bind<char>().toValue('g');

			// Bind a concrete type to a value provider function.
			bind<double>().toValueProvider([] () { return 123.456; });

			// Bind a concrete type to a value provider class.
			bind<float>().toValueProvider<FloatProvider>();

			// Bind a reference.
			bind<std::ostream>("testStream").toReference(referencedStream);

			// Bind a const reference.
			bind<const CC>().toReference(cc);

			// Bind an interface to a concrete type with thread-local singleton semantics.
			bind<Base>().toThreadLocal<DerivedA>();

			// Bind an interface to an implementing class.
			bind<Base2>().toUnique<Derived2>();

			// Bind an interface to a unique pointer provider function.
			bind<I>().toUniqueProvider([]() { return std::unique_ptr<I>(new C(++cCounter)); });

			// Bind an concrete singleton type.
			bind<Counter>().toEagerSingleton();

			// Bind an interface to a unique pointer provider class.
			bind<J>().toUniqueProvider<UProvider>();

			// Bind a concrete type with thread-local singleton semantics.
			bind<TL>().toThreadLocal();

			// Bind a concrete singleton type.
			bind<S>().toSingleton();

			// Bind a concrete singleton type with a const binding.
			bind<const CCS>().toSingleton();

			bind<CD>().toValue();

			////////////// Injectable test classes //////////////

			bind<A>().toEagerSingleton();

			bind<B>().toValue();
		}
	};

	auto injector = Injector::create(Configuration());

	auto a = injector->getShared<A>();

	AssertThat(a->s, is("test string"));
	AssertThat(a->i, is(1234567));
	AssertThat(a->g, is('g'));
	AssertThat(a->d, is(123.456));
	AssertThat(a->f, is(432.1f));
	AssertThat(a->ii->getValue(), is(1));
	AssertThat(a->jj->getValue(), is(100));

	auto b = injector->getValue<B>();

	b.stream << "hello, world!";

	AssertThat(referencedStream.str(), is("hello, world!"));

	// Get the reference stream via getInstance.
	auto & s = injector->getInstance<std::ostream &>("testStream");

	s << " hello again!";

	AssertThat(referencedStream.str(), is("hello, world! hello again!"));

	// Get the const reference object. This must be auto &, not auto,
	// because the type CC has a deleted copy constructor.
	const auto & ccc = injector->getReference<const CC>();

	AssertThat(ccc.value, is(3.14159));

	// Get non-const singleton?
	// Only the const version of the CCS singleton has been bound.
	AssertThat(
		  [&injector] () { injector->getShared<CCS>(); }
		, throws(
			Exception::NoBindingException(
				Impl::BindingKey(typeid(Impl::BindingKeyType<Impl::BindingMetaType::Shared, Balau::CCS>), "")
			)
		)
	);

	// Get the const singleton.
	auto ccs = injector->getShared<const CCS>();

	AssertThat(ccs->value, is(123.456));

	// Promote a new value to a const value.
	// This just queries for the value binding without the const qualifier.
	const auto cd = injector->getValue<const CD>();

	AssertThat(cd.ccsObj->value, is(123.456));

	// Promote a new reference to a const reference.
	// This queries for the const reference binding first, then it tries the non-const reference binding.
	const auto & cStream = injector->getReference<const std::ostream>("testStream");

	const auto & cStringStream = dynamic_cast<const std::ostringstream &>(cStream);
	AssertThat(cStringStream.str(), is(referencedStream.str()));

	// Promote a shared instance to a shared const instance .
	// This queries for the shared const instance binding first, then it tries the shared non-const instance binding.
	auto cS = injector->getShared<const S>();

	// Promote a shared instance to a const shared const instance.
	// This queries for the shared const instance binding first, then it tries the shared non-const instance binding.
	const auto ccS = injector->getInstance<const std::shared_ptr<const S>>();
}

struct AA {
	double value;
	BalauInjectConstruct(AA, value);
	AA(const AA & ) = delete; // Prevent copying.
};

const AA aa(543.2);

void InjectorTest::docTest() {
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			// A double value injected into A.
			bind<double>().toValue(123.456);

			// Bind a const singleton.
			bind<const AA>().toSingleton();

			// Bind a const reference.
			bind<const AA>().toReference(aa);
		}
	};

	auto injector = Injector::create(Configuration());

	// Compile time error:
	//auto a1 = injector->getReference<AA>();

	// Compile time error:
	//auto a2 = injector->getShared<AA>();

	auto & a3 = injector->getReference<const AA>();
	auto a4 = injector->getShared<const AA>();

	AssertThat(a3.value, is(543.2));
	AssertThat(a4->value, is(123.456));
}

class HasInjector {
	std::weak_ptr<Injector> injector;
	BalauInjectConstruct(HasInjector, injector);
};

void InjectorTest::injectTheInjector() {
	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			// New instances do not cause cyclic shared pointer container issues.
			bind<HasInjector>().toValue();

			// New instances do not cause cyclic shared pointer container issues.
			// If HasInjector's injector field was a shared_ptr instead of a weak_ptr,
			// This shared binding would cause a cyclic shared pointer container issue.
			bind<HasInjector>().toSingleton();
		}
	};

	auto injector = Injector::create(Configuration());

	auto v = injector->getValue<HasInjector>();
	auto s = injector->getShared<HasInjector>();
}

class CycleSB;
class CycleSC;

class CycleSA {
	std::shared_ptr<CycleSB> b;
	BalauInjectConstruct(CycleSA, b);
};

class CycleSB {
	std::shared_ptr<CycleSC> c;
	BalauInjectConstruct(CycleSB, c);
};

class CycleSC {
	std::shared_ptr<CycleSA> a;
	BalauInjectConstruct(CycleSC, a);
};

void InjectorTest::sharedCycleChecks() {
	class Configuration1 : public ApplicationConfiguration {
		public: void configure() const override {
			bind<CycleSA>().toSingleton();
			bind<CycleSB>().toSingleton();
			bind<CycleSC>().toSingleton();
		}
	};

	AssertThat(
		[] () {
			Injector::create(Configuration1());
		}
		, throws<Exception::CyclicDependencyException>()
	);
}

class CycleVB;
class CycleVC;

class CycleVA {
	std::shared_ptr<CycleVB> b;
	BalauInjectConstruct(CycleVA, b);
};

class CycleVB {
	std::shared_ptr<CycleVC> c;
	BalauInjectConstruct(CycleVB, c);
};

class CycleVC {
	CycleVA a;
	BalauInjectConstruct(CycleVC, a);
};

void InjectorTest::mixedCycleChecks() {
	class Conf : public ApplicationConfiguration {
		public: void configure() const override {
			bind<CycleVA>().toValue();
			bind<CycleVB>().toSingleton();
			bind<CycleVC>().toSingleton();
		}
	};

	AssertThat(
		[] () {
			Injector::create(Conf());
		}
		, throws<Exception::CyclicDependencyException>()
	);
}

class ASingletonBaseWithInjector {};

class ASingletonWithInjector : public ASingletonBaseWithInjector {
	std::shared_ptr<Injector> injector;
	BalauInjectConstruct(ASingletonWithInjector, injector);
};

class ASingletonBaseWithWeakInjector {};

class ASingletonWithWeakInjector : public ASingletonBaseWithWeakInjector {
	std::weak_ptr<Injector> injector;
	BalauInjectConstruct(ASingletonWithWeakInjector, injector);
};

void InjectorTest::injectorCycleAvoidance() {
	// Create a singleton binding with a shared pointer to the injector inside it?
	// Throws exception.

	class Configuration1 : public ApplicationConfiguration {
		public: void configure() const override {
			bind<ASingletonBaseWithInjector>().toSingleton<ASingletonWithInjector>();
		}
	};

	AssertThat([] () { Injector::create(Configuration1()); }, throws<Exception::SharedInjectorException>());

	// Create a new instance binding with a shared pointer to the injector inside it.
	// Does not throw an exception.

	class Configuration2 : public ApplicationConfiguration {
		public: void configure() const override {
			bind<ASingletonBaseWithInjector>().toUnique<ASingletonWithInjector>();
		}
	};

	// No exception thrown.
	auto injector = Injector::create(Configuration2());

	// Create a new instance binding with a weak pointer to the injector inside it.
	// Does not throw an exception.

	class Configuration3 : public ApplicationConfiguration {
		public: void configure() const override {
			bind<ASingletonBaseWithWeakInjector>().toSingleton<ASingletonWithWeakInjector>();
		}
	};

	// No exception thrown.
	auto injector2 = Injector::create(Configuration3());
}

class SingletonProvider {
	public: std::shared_ptr<int> operator () () {
		auto r = std::shared_ptr<int>(new int);
		*r = value;
		return r;
	}

	private: int value;

	BalauInjectConstructNamed(SingletonProvider, value, "reference-value");
};

void InjectorTest::singletonProvider() {
	// Create a singleton binding via the provider (which has its own dependency).

	const int expected = 123;

	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			bind<int>("reference-value").toValue(expected);
			bind<int>().toSingletonProvider<SingletonProvider>();
		}
	};

	auto injector = Injector::create(Configuration());

	auto provided = injector->getShared<int>();

	AssertThat(*provided, is(expected));
}

void InjectorTest::providedSingletonProvider() {
	// Create a singleton binding via a provider instance.

	const int expected = 123;

	class Configuration : public ApplicationConfiguration {
		public: void configure() const override {
			auto provider = std::shared_ptr<SingletonProvider>(new SingletonProvider(expected));

			bind<int>().toSingletonProvider(provider);
		}
	};

	auto injector = Injector::create(Configuration());

	auto provided = injector->getShared<int>();

	AssertThat(*provided, is(expected));
}

} // namespace Balau

#pragma clang diagnostic pop
