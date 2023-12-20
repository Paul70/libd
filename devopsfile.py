#! /usr/bin/python3
import sys
import subprocess


class libdDevOpsRecipe():

    # DevOps setup main data
    name = "libd-devops"
    version = "0.1.0"
    os_list = {
        "Ubuntu": "22.04",
        "Windows": "11"
    }
    IDE_list = [
        "qtcreator"
    ]
    options = {
        "devops-profile": "gcc-131-debug-config.json"
    }
    default_options = {
        "devops-profile": "default.json"
    }

    # DevOps setup metadata
    author = "Paul Heidenreich"
    decription = "Settings configuration recipe file."

    # Check if the specified devops profile fits for this host operating system.
    def checkProfile(self):
        if self.options["devops-profile"]:
            #return Settings.check(self.name, self.options["devops-profile"], self.os_list)
            return False
        else:
            return True
            #return Settings.check(self.name, self.default_options["devops-profile"], self.os_list)
        pass

    # Generate configurations for porject
    def generate(self):
        if not self.options["devops-profile"]:
            self.options["devops-profile"] = self.default_options["devops-profile"]
        pass

        script = [
            "./devops/command.py --bootstrap --name "+ self.name + " --profile " + self.options["devops-profile"]
        ]
        print(script[0])
        for cmd in script:
            subprocess.run([cmd], shell = True, capture_output = False, text = False)
        pass

    def build(self):
        print("This will be the build method call ...")


###################################################################################################
dev = libdDevOpsRecipe()
def generate():
    dev.generate()
def build():
    dev.build()


# args[0]  = current file
# args[1]  = function name
# args[2:] = function args : (*unpacked)
if __name__ == "__main__":
    args = sys.argv
    globals()[args[1]](*args[2:])

