#include "logging.hpp"

void Logging::init(const size_t _phase_count) {
	// Someone has already initialised
	if (phase_count == 0) {
		blg::register_simple_formatter_factory< blg::trivial::severity_level, char >("Severity");
		blg::add_common_attributes();

		string logfile = (tremppi_system.standalone ? tremppi_system.PROGRAM_NAME : "tremppi") + ".log";
		blg::add_file_log
			(
			kwd::file_name = (tremppi_system.WORK_PATH /= bfs::path{ logfile }).string(),
			kwd::auto_flush = true,
			kwd::open_mode = (std::ios::out | std::ios::app),
			kwd::format = "[%TimeStamp%] (%LineID%) <%Severity%>: %Message%"
			);

		blg::core::get()->set_filter(blg::trivial::severity >= blg::trivial::info);
#ifdef _MFC_VER
		setvbuf(stdout, 0, _IOLBF, 4096);
#endif

		phase_count = _phase_count;
		if (phase_count > MAX_PHASES)
			throw runtime_error("Max " + to_string(MAX_PHASES) + " phases allowed.");
	}


	BOOST_LOG_TRIVIAL(info) << (string(30, '-'));
}

void Logging::newPhase(const size_t _step_count, const string & _desc) {
	if (_desc.size() > MAX_WIDHT)
		throw runtime_error("Description of a phase " + quote(_desc) + "is longer that maximum " + to_string(MAX_PHASES) + ".");
	phase_no++;
	step_count = _step_count;
	desc = _desc;
	step_no = 1;
}

void Logging::step() {
	cout << left
		<< "\r[" << setw(2) << phase_no << "/" << setw(2) << phase_count << "]: "
		<< setw(MAX_WIDHT) << desc
		<< right
		<< fixed << setw(7) << setprecision(3) << (step_no * 100.) / step_count << "%.";
	step_no++;
}

void Logging::exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	exit(err_no);
}

Logging::Logging() : phase_count(0){

}

// Global variable
Logging logging;