Balau
====

**Documentation:** [developer manual](https://borasoftware.com/doc/balau/latest/manual), [API documentation](https://borasoftware.com/doc/balau/latest/api)

**Main repository:** [Balau Git repository](https://github.com/borasoftware/balau)

**License:** [Boost Software License Version 1.0](https://borasoftware.com/licenses/balau-license.html)


Balau is a C++ application framework designed for enterprise C++ software development.

Following the recent revisions of the language, C++ has matured to become an attractive candidate for rapid enterprise application development. Balau provides tools designed to support the rapid development of high performance C++ enterprise applications.

Balau builds on the foundations of the [ICU](http://site.icu-project.org) and [Boost](http://www.boost.org) projects, and focuses on using modern C++17 features and the standard unicode string classes.

The library has been conceived for the development of applications that have a dependency injection based architecture, have complex logging requirements, and will be developed with a test driven development methodology. Balau has also been designed to provide a base on which Unicode aware C++ software applications may be written with ease.

Four key components of the Balau library are the [injector](https://github.com/borasoftware/balau/blob/master/src/main/cpp/Balau/Application/Injector.hpp), the [environment configuration](https://github.com/borasoftware/balau/blob/master/src/main/cpp/Balau/Application/EnvironmentConfiguration.hpp) framework, the [logging system](https://github.com/borasoftware/balau/blob/master/src/main/cpp/Balau/Logging/Logger.hpp), and the [test runner](https://github.com/borasoftware/balau/blob/master/src/main/cpp/Balau/Testing/TestRunner.hpp). In this respect, part of Balau is a C++ equivalent to the de facto standard Java application development components consisting of the Java standard library for Unicode support, Guice/Spring for dependency injection and environment configuration, Log4j/Slf4j/Logback for logging, and JUnit/TestNG for testing.

In addition to the injector, environment configuration, logger, and test runner, Balau also provides a set of components and utilities with simple APIs, including an HTTP/WebSocket web application framework.
 
For more information on using and contributing to the library, please refer to the Balau [developer manual](https://borasoftware.com/doc/balau/latest/manual).
