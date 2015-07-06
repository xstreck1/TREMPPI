__author__ = 'adams_000'

import subprocess
import os
import sys

class ToolManager:
    _commands = []
    _subprocess = None
    _current = ""
    _last_progress = 0.0

    def isRunning(self):
        if self._subprocess is None:
            return False
        elif self._subprocess.poll() is None:
            return True
        else:
            return False

    def add_to_queue(self, command):
        self._commands.append(command)

    # Return progress of currently executed process in percents.
    # If it is done, start a new one from the queue.
    # If all is done, return -1.
    def get_progress(self):
        if not self.isRunning():
            if len(self._commands) > 0:
                self._current = self._commands.pop()
                self._last_progress = 0.0
                self._subprocess = subprocess.Popen("tremppi " + self._current, stdout=subprocess.PIPE)
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
            return self._current + " " + " ".join(self._commands)

    def kill_all(self):
        self._subprocess.kill()
        self._commands = []