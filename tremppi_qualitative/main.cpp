// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_qualitative(int argc, char ** argv);

int main(int argc, char ** argv) {
	TremppiSystem::standalone = true;
	return tremppi_qualitative(argc, argv);
}