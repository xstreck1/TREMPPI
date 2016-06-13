# Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de
#
# This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
# via Parameter Identification (TREMPPI)
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

from sys import builtin_module_names
from subprocess import Popen, PIPE
from os.path import join
from threading import Thread

from .header import system

try:
    from Queue import Queue, Empty
except ImportError:
    from queue import Queue, Empty  # python 3.x

ON_POSIX = 'posix' in builtin_module_names

class ToolManager:
    _commands = []
    _subprocess = None
    _thread = None
    _current = ""
    _last_progress = 0.0
    _queue = Queue()

    def enqueue_output(self, out, queue):
        line = out.read(7)
        while len(line) > 0:
            line = (out.read(7)[0:6]).decode("UTF-8")
            if len(line) is 6:
                queue.put(line)

    def cmd_to_string(self, cmd):
        if cmd[0]:
            return cmd[1] + ' --path ' + cmd[0]
        else:
            return cmd[1] + ' --path ' + "."

    def is_running(self):
        if self._subprocess is None:
            return False
        elif self._subprocess.poll() is None:
            return True
        else:
            return False

    def add_to_queue(self, path, command):
        self._commands.append((path, command))

    # Return progress of currently executed process in percents.
    # If it is done, start a new one from the queue.
    # If all is done, return -1.
    def get_progress(self):
        if not self.is_running():
            if len(self._commands) > 0:
                command = self._commands.pop()
                self._current = command[1]
                self._last_progress = "00.000"
                print('call: ' + join(system.BIN_PATH, "tremppi") + " " + self.cmd_to_string(command))

                self._subprocess = Popen([join(system.BIN_PATH, "tremppi")] + [command[1]] + ['--path'] + [command[0]], stdout=PIPE)
                self._queue = Queue()

                self._thread = Thread(target=self.enqueue_output, args=(self._subprocess.stdout, self._queue))
                self._thread.start()

                return -1
            else:
                self._current = ""
                self._last_progress = ""
                return -1
        else:
            while not self._queue.empty():
                self._last_progress = self._queue.get_nowait() # or q.get(timeout=.1)
            return self._last_progress


    def get_commands(self):
        if len(self._commands) == 0:
            return self._current
        else:
            return self._current + " " + " ".join(map(self.cmd_to_string, self._commands))

    def kill_all(self, parsed_path):
        self._subprocess.kill()
        self._subprocess = None
        self._commands = []
        self._current = ""
        self._last_progress = "00.000"

    def is_free(self, name): #return true iff name has no scheduled or running commands
        return len([x for x in self._commands if x[0] != name]) == 0
