// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_report(int argc, char ** argv);

int main(int argc, char ** argv) {
	TremppiSystem::standalone = true;
	return tremppi_report(argc, argv);
}