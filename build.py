import os
import sys

print("build options: ")
print("1: Developer (build in debug mode)")
print("2: User (build in release mode)")
print("3: Abort")

valid = False
while True:
    choice = input("your choice: ")
    try:
        choice = int(choice)
    except ValueError:
        print("must enter a number!! try again.")
        continue
    if choice > 3 or choice < 1:
        print("must choose number between 1 and 5. try again")
    else:
        break

if choice == 3:
    print("building process aborted by user.")
    exit(0)

executablepath = "Debug" if choice == 1 else "Release"
print("building . . .")

if sys.platform == "linux":
    if choice == 1:
        # Build Debug configuration on Linux using GNU Make
        os.system("premake5 --os=linux gmake")
        os.system("make config=debug")
    elif choice == 2:
        # Build Release configuration on Linux using GNU Make
        os.system("premake5 --os=linux gmake")
        os.system("make config=release")

    print("done building.")
    print(f"if no errors accured the executable can be found in the bin/{executablepath}")

elif sys.platform == "win32":
    if choice == 1:
        # Build Debug configuration on Windows using Visual Studio 2019
        os.system("premake5 --os=windows vs2022")
        os.system("msbuild /p:Configuration=Debug")
    elif choice == 2:
        # Build Release configuration on Windows using Visual Studio 2019
        os.system("premake5 --os=windows vs2022")
        os.system("msbuild /p:Configuration=Release")

    print("done building.")
    print("if no errors accured the executable can be found in the bin/Debug or bin/Release according to your build.")

else:
    print("Operating system not supported...\n this application only support windows and linux :(")
