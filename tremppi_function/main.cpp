// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_function(int argc, char ** argv);

int main(int argc, char ** argv) {
	TremppiSystem::standalone = true;
	return tremppi_function(argc, argv);
}