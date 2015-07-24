__author__ = 'adams_000'

import subprocess
import os
from os.path import join
from tremppi.header import system

class ToolManager:
    _commands = []
    _subprocess = None
    _current = ""
    _last_progress = 0.0

    def cmd_to_string(self, cmd):
        if (cmd[0] is not ""):
            return cmd[1] + ' --path ' + cmd[0]
        else:
            return cmd[1]

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
                self._current = self.cmd_to_string(self._commands.pop())
                self._last_progress = 0.0
                print('call: ' + join(system.BIN_PATH, "tremppi") + " " + self._current)
                self._subprocess = subprocess.Popen(join(system.BIN_PATH, "tremppi") + " " + self._current, stdout=subprocess.PIPE)
                return -1
            else:
                self._current = ""
                self._last_progress = 0.0
                return -1
        else:
            self._subprocess.stdout.seek(0, os.SEEK_END)
            current_pos = self._subprocess.stdout.tell()
            if current_pos >= 11:
                self._subprocess.stdout.seek(-11,1)
                progress = self._subprocess.stdout.read(11)
                position = progress.find(b'\r')
                self._last_progress = progress[(position+1):(position+6)]
                print(self._last_progress)
            return float(self._last_progress)

    def get_commands(self):
        if len(self._commands) == 0:
            return self._current
        else:
            return self._current + " " + " ".join(map(self.cmd_to_string, self._commands))

    def kill_all(self):
        self._subprocess.kill()
        self._commands = []

    def call_init(self, name):
        subprocess.Popen(join(system.BIN_PATH, "tremppi") + " " + name)

    def is_free(self, name): #return true iff name has no scheduled or running commands
        [x for x in self._commands if x[0] != name]
