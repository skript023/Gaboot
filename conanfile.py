from conan import ConanFile

class Gaboot(ConanFile):
    name = "Gaboot"
    version = "1.0.0"

    def requirements(self):
        self.requires("drogon/1.9.3", options={
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
        })
        self.requires("opencv/4.9.0", override=True)