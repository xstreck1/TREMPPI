#include "logging.hpp"
#include "system.hpp"

// Static variable allocations
vector<LogPhase> Logging::phases;
bool Logging::phase_bit;
bool Logging::initiated;

Logging::Logging() {
	if (Logging::initiated)
		return;
	else
		Logging::initiated = true;

	Logging::phase_bit = false;
	Logging::initiated = false;

	// Find the logifile name
	string name = (TremppiSystem::standalone) ? TremppiSystem::PROGRAM_NAME : "tremppi";
	string logfile = name + ".log";

	// Set the formatting
	blg::register_simple_formatter_factory< blg::trivial::severity_level, char >("Severity");
	blg::add_common_attributes();
	blg::add_file_log
		(
		kwd::file_name = (TremppiSystem::WORK_PATH / bfs::path{ logfile }).string(),
		kwd::auto_flush = true,
		kwd::open_mode = (std::ios::out | std::ios::app),
		kwd::format = "[%TimeStamp%] (%LineID%) <%Severity%>: %Message%"
		);
	blg::core::get()->set_filter(blg::trivial::severity >= blg::trivial::info);

#ifdef _MSC_VER
	//Set the output buffer size for visual studio
	setvbuf(stdout, 0, _IOLBF, 4096);
#endif
	
	// Create the dashes in the logifile
	BOOST_LOG_TRIVIAL(info) << (string(30, '-'));
	BOOST_LOG_TRIVIAL(info) << TremppiSystem::PROGRAM_NAME << " started.";
}

Logging::~Logging() {
	BOOST_LOG_TRIVIAL(info) << TremppiSystem::PROGRAM_NAME << " finished successfully.";
	blg::core::get()->remove_all_sinks();
}

void Logging::newPhase(const string & _desc, const phase_no _step_count) {
	Logging::phases.push_back({ _step_count, 0, _desc, phases.size() });
}

void Logging::killPhase() {
	Logging::phases.pop_back();
}

void Logging::step() {
	string output = "\r";

	Logging::phases.back().step_no += 1;
	while (Logging::phases.back().step_count <= Logging::phases.back().step_no) {
		Logging::phases.pop_back();
		if (!Logging::phases.empty())
			Logging::phases.back().step_no += 1;
		else {
			string finished = "FINISHED";
			output.resize(LINE_LENGTH - finished.size() - 1);
			output += finished;
			break;
		}
	}

	if (!Logging::phases.empty()) {
		Logging::phase_bit ^= Logging::phase_bit;

		for (const auto & phase : phases) {
			output += "[" + phase.description + "]";
		}

		output.resize(Logging::LINE_LENGTH - (Logging::phases.size() * Logging::NUM_CHARS) - Logging::PAD, ' ');
		const string marker = (Logging::phase_bit ? "." : "|");
		output += " " + marker + " ";

		for (const auto & phase : Logging::phases) {
			output += "[";
			string frac = to_string((100.0 * phase.step_no) / phase.step_count);
			frac.resize(Logging::NUM_CHARS - Logging::PAD, '0');
			output += frac + "%]";
		}
	}

	cout << output;

#ifdef _MSC_VER
	// flush for VS
	if (!Logging::phases.empty() && Logging::phases.back().step_count < 100)
		cout << flush;
#endif
}


void Logging::exceptionMessage(const exception & e, const int err_no) {
	BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
	cerr << "\nERROR: " << e.what();
	exit(err_no);
}