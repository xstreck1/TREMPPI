// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_spawn(int argc, char ** argv);

int main(int argc, char ** argv) {
	tremppi_system.standalone = true;
	return tremppi_spawn(argc, argv);
}