from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
#from conan.tools.files import get
import sys


required_conan_version = ">=2.0.0"

class LibdRecipe(ConanFile):

    # Package binray configuration and metadata
    # Settings: project-wide configuration that cannot be defaulted in recipes
    #           See alos devopsfile.py which maintains settings
    # Options:  package-specific configurations which can be defaulted in recipes (no operating 
    #           related settings)
    name = "libd"
    version = "0.1.0"
    package_type = "shared-library"
    author = "Paul Heidenreich"
    description = "libd - Simulation Laboratory for Stock evaluation and prediction"
    license = ""
    url = ""
    settings = "os", "compiler", "build_type", "arch" 
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    } 
    default_options = {
        "shared": True,
        "fPIC": True
    }
    no_copy_source = True
    
    # Requirements for build (tool_requires) and host (requires) context 
    def requirements(self):
        self.requires("gtest/1.14.0")
        pass

    def build_requirements(self):
        self.tool_requires("cmake/3.27.7")
        self.tool_requires("ninja/1.11.1")
        pass

    
    # Packaging - how to build this package 
    def config_options(self):
        self.conanfilePrint("conanfile.py ("+ self.name +"/" + self.version+"): Calling config_options()")
        if self.settings.os == "Windows":
            del self.options.fPIC
        pass

    def layout(self):
        cmake_layout(self)
        pass

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()
        pass

    def build(self):
        cmake = CMake(self)
        cmake.configure()

        # Actual cmake call ("cmake -DC...=conan_toolchain.cmake") happens here
        cmake.build()        
        pass

    def package(self):
        cmake = CMake(self)
        cmake.install()
        pass

    def package_info(self):
        self.cpp_info.libs = [
            "dutil"
        ]

        pass


    ###############################################################################################
    #
    # Helper methods used in this conanfile 
    #
    ###############################################################################################

    # we print to stderr since we do not want to alter conan fucntion stdoout output
    def conanfilePrint(self, string):
        print(string, file=sys.stderr)
        pass
    
