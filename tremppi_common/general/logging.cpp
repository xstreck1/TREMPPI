#include "logging.hpp"
#include "system.hpp"

Logging::Logging() : phase_bit(false), initiated(false) {
	if (initiated)
		return;
	else
		initiated = true;

	// Find the logifile name
	string name = (tremppi_system.standalone) ? tremppi_system.PROGRAM_NAME : "tremppi";
	string logfile = name + ".log";

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

	//Set the output buffer size for visual studio
	setvbuf(stdout, 0, _IOLBF, 4096);
	
	// Create the dashes in the logifile
	BOOST_LOG_TRIVIAL(info) << (string(30, '-'));
	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " started.";
}

Logging::~Logging() {
	BOOST_LOG_TRIVIAL(info) << tremppi_system.PROGRAM_NAME << " finished successfuly.";
	blg::core::get()->remove_all_sinks();
}

void Logging::newPhase(const string & _desc, const size_t _step_count) {
	phases.push_back({ _step_count, 0, _desc, phases.size() });
}

void Logging::killPhase() {
	phases.pop_back();
}

void Logging::step() {
	string output = "\r";

	phases.back().step_no += 1;
	while (phases.back().step_count <= phases.back().step_no) {
		phases.pop_back();
		if (!phases.empty())
			phases.back().step_no += 1;
		else {
			string finished = "FINISHED";
			output.resize(LINE_LENGTH - finished.size());
			output += "FINISHED";
			break;
		}
	}

	if (!phases.empty()) {
		phase_bit = !phase_bit;

		for (const auto & phase : phases) {
			output += "[" + phase.description + "]";
		}

		output.resize(LINE_LENGTH - (phases.size() * NUM_CHARS) - PAD, ' ');
		const string marker = (phase_bit ? "." : "|");
		output += " " + marker + " ";

		for (const auto & phase : phases) {
			output += "[";
			string frac = to_string((100.0 * phase.step_no) / phase.step_count);
			frac.resize(NUM_CHARS - PAD, '0');
			output += frac + "%]";
		}
	}

	cout << output;
}


void Logging::exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	cerr << "\nERROR: " << e.what();
	exit(err_no);
}