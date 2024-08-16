//
// Created by n08i40k on 14.08.2024.
//

#include "server_session.h"

namespace network {
server_session::server_session(
	boost::asio::ip::tcp::socket socket): socket(std::move(socket)) {}

std::unique_ptr<server_session>
server_session::create(
	boost::asio::ip::tcp::socket socket) {
	return std::unique_ptr<server_session>(new server_session(std::move(socket)));
}
} // network
