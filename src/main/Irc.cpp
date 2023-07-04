#include "../../includes/Server.hpp"

int main(int argc, char **argv) {
	Server server;

	try {
		server.inputParser(argc, argv);
		while (true) {}
		// while (server.isRunning()) server.run();
	} catch(const std::exception &error) {
		std::cout << RED << error.what() << std::endl;
	}
	return 0;
}