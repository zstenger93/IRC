#include <exception>

#include "../../includes/Server.hpp"

int run_server(Server server) {
	try {
		if (server.setup() != -1) {
			server.setAdminDetails();
			while (server.isRunning()) {server.run();}
			close(server.getServerSocket());
		}
	} catch (const std::exception &error) {
		run_server(server);
	}
}

int main(int argc, char **argv) {
	Server server;
	try {
		server.inputParser(argc, argv);
		run_server(server);
	} catch (const std::exception &error) {
		std::cout << RED << error.what() << std::endl;
	}
	return 0;
}