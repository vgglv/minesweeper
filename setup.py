import subprocess
import os
import sys

ROOT_PATH = os.path.dirname(os.path.realpath(__file__)).replace('\\', '/')
BUILD_FOLDER_PATH = os.path.join(ROOT_PATH, "build").replace('\\', '/')

def generate_cmake():
    print("===== Starting CMake project generation =====")
    cmake = ['cmake', '-S', ROOT_PATH, '-B', BUILD_FOLDER_PATH]

    if sys.platform == 'win32':
        cmake.append('-DWIN32=1')
        cmake.append('-AWin32')
    else:
        cmake.append('-DMACOS=1')

    cmake.append('-DCMAKE_EXPORT_COMPILE_COMMANDS=1')
    process = subprocess.run(cmake)
    print(f'CMake generated with return code {process.returncode}.')
    return process.returncode == 0

def build_executable():
    print('===== Starting CMake build =====')
    build = ['cmake', '--build', BUILD_FOLDER_PATH, "--config", "Debug"]
    process = subprocess.run(build)
    return process.returncode == 0

def main():
    if not generate_cmake():
        print('[ERROR] cmake project failed to generate, exiting...')
        return
    if not build_executable():
        print('[ERROR] cmake project failed to build, exiting...')
        return
    print("=== ALL CLEAR ===")

if __name__ == "__main__":
    main()
