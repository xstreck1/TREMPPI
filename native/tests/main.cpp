// standalone launcher
#include <tremppi_common/general/system.hpp>

int tremppi_test(int argc, char ** argv);

int main(int argc, char ** argv) 
{
	TremppiSystem::standalone = false;
	return tremppi_test(argc, argv);
}