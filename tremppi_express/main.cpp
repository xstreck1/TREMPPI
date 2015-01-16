// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_express(int argc, char ** argv);

int main(int argc, char ** argv) {
	TremppiSystem::standalone = true;
	return tremppi_express(argc, argv);
}