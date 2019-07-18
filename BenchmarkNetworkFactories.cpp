/*
 * Copyright (c) 2011 Tobias Markmann
 * Licensed under the GNU General Public License v3.
 * See gpl-3.0.txt for more information.
 */

#include "BenchmarkNetworkFactories.h"

#include <Swiften/Crypto/PlatformCryptoProvider.h>
#include <Swiften/IDN/PlatformIDNConverter.h>
#include <Swiften/Network/BoostConnectionFactory.h>
#include <Swiften/Network/BoostConnectionServerFactory.h>
#include <Swiften/Network/BoostTimerFactory.h>
#include <Swiften/Network/PlatformProxyProvider.h>
#include <Swiften/Parser/PlatformXMLParserFactory.h>
#include <Swiften/TLS/PlatformTLSFactories.h>

BenchmarkNetworkFactories::BenchmarkNetworkFactories(Swift::BoostASIOEventLoop* eventLoop, const std::string& ip) : eventLoop(eventLoop) {
	proxyProvider = new Swift::PlatformProxyProvider();
	platformTlsFactories= new Swift::PlatformTLSFactories();
	xmlParserFactory = new Swift::PlatformXMLParserFactory();
	connectionServerFactory = new Swift::BoostConnectionServerFactory(eventLoop->ioService_, eventLoop);
	domainNameResolver = new StaticDomainNameResolver(ip);
	connectionFactory = new Swift::BoostConnectionFactory(eventLoop->ioService_, eventLoop);
	timerFactory = new Swift::BoostTimerFactory(eventLoop->ioService_, eventLoop);
}

BenchmarkNetworkFactories::~BenchmarkNetworkFactories() {
	delete timerFactory;
	delete connectionFactory;
	delete domainNameResolver;
	delete connectionServerFactory;
	delete xmlParserFactory;
	delete platformTlsFactories;
	delete proxyProvider;
}

Swift::TimerFactory* BenchmarkNetworkFactories::getTimerFactory() const {
	return timerFactory;
}

Swift::ConnectionFactory* BenchmarkNetworkFactories::getConnectionFactory() const {
	return connectionFactory;
}

Swift::DomainNameResolver* BenchmarkNetworkFactories::getDomainNameResolver() const {
	return domainNameResolver;
}

Swift::ConnectionServerFactory* BenchmarkNetworkFactories::getConnectionServerFactory() const {
	return connectionServerFactory;
}

Swift::NATTraverser* BenchmarkNetworkFactories::getNATTraverser() const {
	return nullptr;
}

Swift::XMLParserFactory* BenchmarkNetworkFactories::getXMLParserFactory() const {
	return xmlParserFactory;
}

Swift::TLSContextFactory* BenchmarkNetworkFactories::getTLSContextFactory() const {
	return platformTlsFactories->getTLSContextFactory();
}

Swift::ProxyProvider* BenchmarkNetworkFactories::getProxyProvider() const {
	return proxyProvider;
}

Swift::EventLoop* BenchmarkNetworkFactories::getEventLoop() const {
	return eventLoop;
}

Swift::NetworkEnvironment* BenchmarkNetworkFactories::getNetworkEnvironment() const {
	return nullptr;
}

Swift::IDNConverter* BenchmarkNetworkFactories::getIDNConverter() const {
	static Swift::IDNConverter* idn = Swift::PlatformIDNConverter::create();
	return idn;
}

Swift::CryptoProvider* BenchmarkNetworkFactories::getCryptoProvider() const {
	static Swift::CryptoProvider* crypto = Swift::PlatformCryptoProvider::create();
	return crypto;
}
