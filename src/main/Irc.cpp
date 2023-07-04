#include "../../includes/Server.hpp"

int main(int argc, char **argv) {
	Server server;

	try {
		server.inputParser(argc, argv);
		// while (true) {}
		if (server.setup() != -1) {
			while (server.isRunning()) server.run();
			close(server.getServerSocket());
		}
	} catch (const std::exception &error) {
		std::cout << RED << error.what() << std::endl;
	}
	return 0;
}