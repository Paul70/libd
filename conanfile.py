from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import get
import sys

# this check needs a conan version >= 1.28
required_conan_version = ">=1.43.0"                                             

class LibdRecipe(ConanFile):
    name = "libd"
    version = "0.1.0"
    package_type = "shared library"

    ###############################################################################################
    #
    # Package metadata
    #
    ###############################################################################################
    author = "Paul Heidenreich"
    description = "libd - Simulation Laboratory for Stock evaluation and prediction"

    ###############################################################################################
    #
    # Binary configuration - set only options which have a default value.
    # Settings: project-wide configuration that cannot be defaulted in recipes
    # Options: package-specific configuration and can be defaulted in recipes 
    #
    ###############################################################################################
    no_copy_source = True
    settings = "os", "compiler", "build_type", "arch" 
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "default_config": [True, False],
        "project_config_id": ["ANY"],
        "use_cached_cmake": [True, False],
    } 
    default_options = {
        "shared": False,
        "fPIC": True,
        "default_config": False,
        "project_config_id": 2,
        "use_cached_cmake": True,
    }          