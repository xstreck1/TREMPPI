// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_summary(int argc, char ** argv);

int main(int argc, char ** argv) {
	TremppiSystem::standalone = true;
	return tremppi_summary(argc, argv);
}