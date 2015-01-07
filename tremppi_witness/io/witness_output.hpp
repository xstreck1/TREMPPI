#pragma once

#include <tremppi_common/general/file_manipulation.hpp>
#include <tremppi_common/general/system.hpp>

namespace WitnessOutput {
	void copyWitnessFiles(const bfs::path & witness_path) {
		// Copy the data from "home/javascript/report"
		FileManipulation::copyDir(tremppi_system.HOME_PATH / bfs::path{ "javascript" } / bfs::path{ "witness" }, witness_path);
		bfs::path report_html = witness_path;
		report_html.replace_extension("html");
		bfs::copy_file(tremppi_system.HOME_PATH / bfs::path{ "javascript" } / bfs::path{ "witness.html" }, report_html);
		FileManipulation::replaceInFile(report_html, "./witness", "./" + witness_path.stem().string());
		FileManipulation::replaceInFile(report_html, "tremppi_common.widget = \"witness\";", "tremppi_common.widget = \"" + witness_path.stem().string() + "\";");
	}
}