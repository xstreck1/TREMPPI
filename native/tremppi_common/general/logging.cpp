/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "logging.hpp"
#include "system.hpp"

ofstream Logging::file;
vector<LogPhase> Logging::phases;
int Logging::last_progress_val;

Logging::Logging()
{
	Logging::getFile() << " --- " << TremppiSystem::PROGRAM_NAME << " has started." << " --- " << std::flush;
	last_progress_val = 0;
	cout << fixed << setprecision(PRECISION);
	cout << "00.000\r";
}

Logging::~Logging()
{
	DEBUG_LOG << TremppiSystem::PROGRAM_NAME << " finished successfully." << endl;
}

ofstream & Logging::getFile()
{
	if (!file.is_open())
	{
		acquireFile();
	}
	return file;
}

void Logging::acquireFile()
{
	file.open((TremppiSystem::WORK_PATH / "log.txt").string(), ios::app);

	if (!file)
	{
		throw runtime_error("Failed to open the log.txt.");
	}
}

void Logging::releaseFile()
{
	file.close();
}

void Logging::newPhase(const string & _desc, const PhaseNo _step_count)
{
	Logging::phases.push_back({ _step_count, 0, _desc, phases.size() });
}

void Logging::killPhase()
{
	Logging::phases.pop_back();
}

void Logging::step()
{
	Logging::phases.back().step_no += 1;

	while (Logging::phases.back().step_count <= Logging::phases.back().step_no)
	{
		Logging::phases.pop_back();
		if (!Logging::phases.empty()) 
		{
			Logging::phases.back().step_no += 1;
		}
		else
		{
			break;
		}
	}

	float progress = 0.0;
	float frac = 1.0;

	for (int i = 0; i < phases.size(); i++)
	{
		progress += Logging::phases[i].step_no / (Logging::phases[i].step_count * frac);
		frac *= Logging::phases[i].step_count;
	}
	int progress_i = static_cast<int>(100 * progress * SHIFT_FACTOR);
	// print only if enough of the change accumulates

	if (progress_i < 10 * SHIFT_FACTOR) {
		cout << "0";
	}
	cout << progress_i / SHIFT_FACTOR << "\r";
}

void Logging::exceptionMessage(const exception & e, const int err_no)
{
	cerr << "\nERROR: " << e.what() << std::endl << flush;
	ERROR_LOG << "Top level exception: " << e.what() << std::endl << flush;
	releaseFile();
	exit(err_no);
}