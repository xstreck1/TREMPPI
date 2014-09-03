#include "logging.hpp"

void Logging::init() {
	// Set values
	step_count = step_no = substep_count = substep_no = 0;

	// Find the logifile name
	string logfile = (tremppi_system.standalone ? tremppi_system.PROGRAM_NAME : "tremppi") + ".log";

	// Set the formatting
	blg::register_simple_formatter_factory< blg::trivial::severity_level, char >("Severity");
	blg::add_common_attributes();
	blg::add_file_log
		(
		kwd::file_name = (tremppi_system.WORK_PATH / bfs::path{ logfile }).string(),
		kwd::auto_flush = true,
		kwd::open_mode = (std::ios::out | std::ios::app),
		kwd::format = "[%TimeStamp%] (%LineID%) <%Severity%>: %Message%"
		);
	blg::core::get()->set_filter(blg::trivial::severity >= blg::trivial::info);

	// Set the output buffer size for visual studio
#ifdef _MFC_VER
	setvbuf(stdout, 0, _IOLBF, 4096);
#endif

	// Create the dashes in the logifile
	BOOST_LOG_TRIVIAL(info) << (string(30, '-'));
}

void Logging::newPhase(const string & _desc, const size_t _step_count) {
	if (_desc.size() > MAX_WIDHT)
		throw runtime_error("Description of a phase " + quote(_desc) + "is longer that maximum " + to_string(MAX_WIDHT) + ".");
	step_count = _step_count;
	phase_desc = _desc;
	step_no = 1;
}

void Logging::newSubPhase(const string & _desc, const size_t _substep_count) {
	if (_desc.size() > MAX_WIDHT)
		throw runtime_error("Description of a subphase " + quote(_desc) + "is longer that maximum " + to_string(MAX_WIDHT) + ".");
	substep_count = _substep_count;
	subphase_desc = _desc;
	substep_no = 1;
}

void Logging::uniStep(bool is_sub) {
	cout << left << "\r" << setw(MAX_WIDHT * 2);
	string line;
	line += "[" + phase_desc + "]";
	if (is_sub)
		line += "[" + subphase_desc + "]";
	cout << line;

	size_t & s_no = is_sub ? substep_no : step_no;
	cout << right << fixed << setw(7) << setprecision(3)
		<< (s_no * 100.) / (is_sub ? substep_count : step_count) << "%.";
	s_no++;
}

void Logging::step() {
	uniStep(false);
}

void Logging::subStep() {
	uniStep(true);
}

void Logging::exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	cerr << "\nERROR: " << e.what();
	exit(err_no);
}

Logging logging; //< the global log