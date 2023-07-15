#include "../../includes/Server.hpp"

int main(int argc, char **argv) {
	Server server;
	try {
		Server server(argc, argv);
		while (server.shouldReset()) {
			server.setup();
			server.run();
			close(server.getServerSocket());
		}
	} catch (const std::exception &error) {
		std::cout << RED << error.what() << END << std::endl;
	}
	close(server.getServerSocket());
	return 0;
}