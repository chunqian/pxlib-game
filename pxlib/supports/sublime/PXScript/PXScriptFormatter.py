import os
import sublime, sublime_plugin, sys
import subprocess

class PxFormatCommand(sublime_plugin.TextCommand):
    def run(self, edit, error=True, save=True):
        if not self.view.settings().get("syntax").endswith('PXScript.sublime-syntax'):
            return
        # run px-format
        package_path = os.path.split(os.path.dirname(__file__))[1]
        executable_path = os.path.join(
            sublime.packages_path(), package_path, "bin", sys.platform, "px-format"
        )

        contents = self.view.substr(sublime.Region(0, self.view.size()))

        cmd = [executable_path]
        style = sublime.load_settings("PXScriptFormatter.sublime-settings").get("style", "")
        if style != "":
            cmd.append("-style")
            cmd.append(style)

        process = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        process.stdin.write(str.encode(contents))
        process.stdin.close()
        output = bytes.decode(process.stdout.read())
        error = bytes.decode(process.stderr.read())
        if error == "":
            self.view.replace(edit, sublime.Region(0, self.view.size()), output)
        else:
            sublime.error_message(error)
