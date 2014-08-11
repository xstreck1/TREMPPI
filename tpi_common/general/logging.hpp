#pragma once

#include "../header.h"
#include "common_functions.hpp"

namespace Logging {
	size_t phase_no;
	size_t phase_count;
	size_t step_count;
	size_t step_no;
	string desc;
	const size_t MAX_PHASES = 99;
	const size_t MAX_WIDHT = 60; // Maximal lenght of the message

	void init(const string LOGFILE = "logfile.log") {
		blg::register_simple_formatter_factory< blg::trivial::severity_level, char >("Severity");
		blg::add_common_attributes();

		blg::add_file_log
			(
			kwd::file_name = string(TREMPPI_PATH) + "/logs/" + LOGFILE,
			kwd::auto_flush = true,
			kwd::open_mode = (std::ios::out),
			kwd::format = "[%TimeStamp%] (%LineID%) <%Severity%>: %Message%"
			);

		blg::core::get()->set_filter
			(
			blg::trivial::severity >= blg::trivial::info
			);

#ifdef _MFC_VER
		setvbuf(stdout, 0, _IOLBF, 4096);
#endif
		if (phase_count > MAX_PHASES)
			throw runtime_error("Max " + to_string(MAX_PHASES) + " phases allowed.");
		Logging::phase_count = phase_count;
	}

	void newPhase(const size_t step_count, const string & desc) {
		if (desc.size() > MAX_WIDHT)
			throw runtime_error("Description of a phase " + quote(desc) + "is longer that maximum "+ to_string(MAX_PHASES) + ".");
		phase_no++;
		Logging::step_count = step_count;
		Logging::desc = desc;
		Logging::step_no = 1;
	}

	void step() {
		cout << left
			<< "\r[" << setw(2) << phase_no << "/" << setw(2) << phase_count << "]: " 
			<< setw(MAX_WIDHT) << desc
			<< right 
			<< fixed << setw(7) << setprecision(3) << (step_no * 100.) / step_count << "%.";
		step_no++;
	}

	// 
	void exceptionMessage(const exception & e, const int err_no) {
		BOOST_LOG_TRIVIAL(error) << "Top level exception: " << e.what();
		exit(err_no);
	}
}