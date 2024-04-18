from conan import ConanFile
from conan.tools.cmake import cmake_layout

class Gaboot(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    default_options = {
        "drogon/*:shared": False,
        "drogon/*:with_ctl": True,
        "drogon/*:with_orm": True,
        "drogon/*:with_boost": True,
        "drogon/*:with_mysql": False,
        "drogon/*:with_redis": False,
        "drogon/*:with_brotli": False,
        "drogon/*:with_sqlite": False,
        "drogon/*:with_profile": False,
        "drogon/*:with_postgres": True,
        "drogon/*:with_yaml_cpp": False
    }

    def requirements(self):
        self.requires("drogon/1.9.3")
        self.requires("opencv/4.9.0", override=True)

    def layout(self):
        cmake_layout(self)