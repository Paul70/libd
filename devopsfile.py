#! /usr/bin/python3
import sys
import subprocess


class libdDevOpsRecipe():

    # DevOps setup main data
    name = "libd-devops"
    version = "0.1.0"
    git_tag = "xyz"
    options = {
        "devops-profile": [None,
                           "default-debug-config.json",
                           "default-release-config.json", 
                           "gcc-131-debug-config.json",
                           "gcc-131-release-config.json"],
        "os": [None, "Ubuntu", "Windows"],
        "ide": [None, "qtcreator"]
    }
    default_options = {
        "devops-profile": "default-debug-config.json",
        "os": "Ubuntu",
        "ide": "qtcreator"
    }

    # DevOps setup metadata
    author = "Paul Heidenreich"
    decription = "Settings configuration recipe file."

    def configure(self):
        self.options["devops-profile"] = ["gcc-131-debug-config.json"]
        self.options["os"] = ["Ubuntu"]
        self.options["ide"] = ["qtcreator"]
        pass


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

        self.configure()

        if len(self.options["devops-profile"]) > 1:
            self.options["devops-profile"] = self.default_options["devops-profile"]

        script = [
            "./devops/command.py --bootstrap --name "+ self.name + " --profile " + self.options["devops-profile"][0]
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
