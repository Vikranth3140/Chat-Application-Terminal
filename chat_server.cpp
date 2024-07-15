#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = net::ip::tcp;               // from <boost/asio/ip/tcp.hpp>

using namespace std;

unordered_map<int, shared_ptr<websocket::stream<beast::tcp_stream>>> clients;
mutex clients_mutex;

string get_current_time() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char time_str[9];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", ltm);
    return string(time_str);
}

void fail(beast::error_code ec, char const* what) {
    cerr << what << ": " << ec.message() << "\n";
}

class session : public enable_shared_from_this<session> {
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    string username_;

public:
    explicit session(tcp::socket socket)
        : ws_(move(socket)) {}

    void run() {
        ws_.async_accept(
            beast::bind_front_handler(
                &session::on_accept,
                shared_from_this()));
    }

private:
    void on_accept(beast::error_code ec) {
        if (ec) return fail(ec, "accept");
        do_read();
    }

    void do_read() {
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void on_read(beast::error_code ec, size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec == websocket::error::closed) return;

        if (ec) return fail(ec, "read");

        string message = beast::buffers_to_string(buffer_.data());
        buffer_.consume(buffer_.size());

        if (username_.empty()) {
            username_ = message;
            {
                lock_guard<mutex> guard(clients_mutex);
                clients[ws_.next_layer().native_handle()] = make_shared<websocket::stream<beast::tcp_stream>>(move(ws_));
            }
            cout << "Client connected: " << username_ << endl;
        } else {
            string timestamped_message = "[" + get_current_time() + "] " + username_ + ": " + message;
            cout << "Received message: " << timestamped_message << endl;
            broadcast(timestamped_message);
        }

        do_read();
    }

    void on_write(beast::error_code ec, size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (ec) return fail(ec, "write");
    }

    void broadcast(const string& message) {
        lock_guard<mutex> guard(clients_mutex);
        for (const auto& client : clients) {
            if (client.first != ws_.next_layer().native_handle()) {
                client.second->text(client.second->got_text());
                client.second->async_write(
                    net::buffer(message),
                    [](beast::error_code ec, size_t) {
                        if (ec) fail(ec, "write");
                    });
            }
        }
    }
};

class listener : public enable_shared_from_this<listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    listener(net::io_context& ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(net::make_strand(ioc)) {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            fail(ec, "open");
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) {
            fail(ec, "set_option");
            return;
        }

        acceptor_.bind(endpoint, ec);
        if (ec) {
            fail(ec, "bind");
            return;
        }

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            fail(ec, "listen");
            return;
        }
    }

    void run() {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if (ec) {
            fail(ec, "accept");
        } else {
            make_shared<session>(move(socket))->run();
        }

        do_accept();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: chat_server <address> <port>\n";
        return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(atoi(argv[2]));

    net::io_context ioc{1};

    make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    ioc.run();

    return EXIT_SUCCESS;
}