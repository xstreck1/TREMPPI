// standalone launcher
#include <tremppi_common/header.h>

int tremppi_validate(int argc, char ** argv);

int main(int argc, char ** argv) {
	tremppi_system.standalone = true;
	return tremppi_validate(argc, argv);
}