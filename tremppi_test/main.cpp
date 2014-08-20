// standalone launcher
#include <tremppi_common/header.h>

int tremppi_test(int argc, char ** argv);

int main(int argc, char ** argv) {
	tremppi_system.standalone = false;
	return tremppi_test(argc, argv);
}