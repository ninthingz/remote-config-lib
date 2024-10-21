import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class RemoteConfigRecipe(ConanFile):
    """
    install command:
    conan install . --profile=vs2022_x64_release --build=missing -o skip_test=False -o use_clion=True
    conan install . --profile=vs2022_x86_release --build=missing -o skip_test=False -o use_clion=True
    conan install . --profile=vs2022_x64_debug --build=missing -o skip_test=False -o use_clion=True

    create command:
    conan create .

    export command:
    conan export .

    CMake config:
    if (USE_CLION)
        set(CMAKE_PREFIX_PATH ${CMAKE_CACHEFILE_DIR} ${CMAKE_PREFIX_PATH})
    endif ()

    CMake gtest config:
    if (NOT SKIP_TEST)
        add_executable(main_test test/test_fds_utils.cpp)
        enable_testing()

        find_package(GTest CONFIG REQUIRED)
        target_link_libraries(main_test PUBLIC ${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

        add_test(AllTestsInMain main_test)
    endif ()

    CMake config options:
    -DUSE_CLION=True -DSKIP_TEST=False
    """
    name = "remote-config"
    version = "1.0.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "zzyi"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "remote-config"
    topics = "utils"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "skip_test": [True, False], "use_clion": [True, False]}
    default_options = {"shared": False, "fPIC": True, "skip_test": True, "use_clion": False}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def requirements(self):
        self.requires("nlohmann_json/3.11.3")
        if not self.options.skip_test:
            self.requires("gtest/[~1.14]")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)
        if self.options.use_clion:
            install_path = ""
            if self.settings.get_safe("arch") == "x86":
                install_path = "-x86"

            if self.settings.get_safe("build_type") == "Debug":
                install_path = "cmake-build-debug" + install_path
            else:
                install_path = "cmake-build-release" + install_path

            self.folders.generators = os.path.join(install_path)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["SKIP_TEST"] = self.options.skip_test
        tc.variables["USE_CLION"] = self.options.use_clion
        tc.generator = "Ninja"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["remote_config"]
