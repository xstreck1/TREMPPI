__author__ = 'adams_000'

import subprocess
import sys
from os.path import join
from tremppi.header import system
from threading import Thread

try:
    from Queue import Queue, Empty
except ImportError:
    from queue import Queue, Empty  # python 3.x

ON_POSIX = 'posix' in sys.builtin_module_names

class ToolManager:
    _commands = []
    _subprocess = None
    _thread = None
    _current = ""
    _last_progress = 0.0
    _queue = Queue()

    def enqueue_output(self, out, queue):
        line = out.read(6)
        while len(line) > 0:
            line = out.read(6).decode("utf-8")[0:5]
            if len(line) is 5:
                queue.put(line)

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
                command = self._commands.pop()
                self._current = command[1]
                self._last_progress = "00.00"
                print('call: ' + join(system.BIN_PATH, "tremppi") + " " + self.cmd_to_string(command))
                self._subprocess = subprocess.Popen([join(system.BIN_PATH, "tremppi")] + [command[1]] + ['--path'] + [command[0]], stdout=subprocess.PIPE)
                self._thread = Thread(target=self.enqueue_output, args=(self._subprocess.stdout, self._queue))
                self._queue = Queue()
                self._thread.start()
                return -1
            else:
                self._current = ""
                self._last_progress = "00.00"
                return -1
        elif self.is_running():
            try:
                while not self._queue.empty():
                    self._last_progress = self._queue.get_nowait() # or q.get(timeout=.1)
            except Empty:
                print('no output yet')
            else:
                return self._last_progress
        else:
            return 0
    def get_commands(self):
        if len(self._commands) == 0:
            return self._current
        else:
            return self._current + " " + " ".join(map(self.cmd_to_string, self._commands))

    def kill_all(self, parsed_path):
        self._subprocess.kill()
        self._commands = []
        self._current = ""
        self._last_progress = "00.00"

    def call_init(self, name):
        subprocess.Popen(join(system.BIN_PATH, "tremppi") + " " + name)

    def is_free(self, name): #return true iff name has no scheduled or running commands
        return len([x for x in self._commands if x[0] != name]) == 0
