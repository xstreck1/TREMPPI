// standalone launcher
#include <tremppi_common/header.h>

int tremppi_report(int argc, char ** argv);

int main(int argc, char ** argv) {
	tremppi_system.standalone = true;
	return tremppi_report(argc, argv);
}