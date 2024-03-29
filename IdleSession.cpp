/*
 * Copyright (c) 2011 Tobias Markmann
 * Licensed under the GNU General Public License v3.
 * See gpl-3.0.txt for more information.
 */

#include "IdleSession.h"

#include <iostream>

#include <boost/bind.hpp>

using namespace Swift;

IdleSession::IdleSession(AccountDataProvider* accountDataProvider, Swift::NetworkFactories* networkFactories, Swift::CertificateTrustChecker* trustChecker, bool noCompression, bool noTLS, const Swift::URL& boshURL) : noCompression(noCompression), noTLS(noTLS), boshURL(boshURL) {
	AccountDataProvider::Account account = accountDataProvider->getAccount();

	client = new CoreClient(Swift::JID(account.jid), createSafeByteArray(account.password), networkFactories);
	client->setCertificateTrustChecker(trustChecker);
	client->onConnected.connect(boost::bind(&IdleSession::handleConnected, this));
	client->onDisconnected.connect(boost::bind(&IdleSession::handleDisconnected, this, _1));
}

IdleSession::~IdleSession() {

}

void IdleSession::start() {
	ClientOptions options;
	options.allowPLAINWithoutTLS = true;
	options.useStreamCompression = !noCompression;
	options.useAcks = false;
	options.useTLS = noTLS ? ClientOptions::NeverUseTLS : ClientOptions::UseTLSWhenAvailable;
	options.boshURL = boshURL;

	client->connect(options);
}

void IdleSession::stop() {
	client->disconnect();
}

void IdleSession::benchmark(const boost::posix_time::ptime& now) {
	// no op; we're idle here
	onDoneBenchmarking();
}

void IdleSession::handleConnected() {
	onReadyToBenchmark();
}

void IdleSession::handleDisconnected(const boost::optional<ClientError>& error) {
	if (error) {
		std::cout << "Idle session disconnected with error ( " << error.get().getType() << " )!" << std::endl;
	}
	onStopped();
}
