import os
import sys

if sys.platform == "linux":
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
    os.system("premake5-windows.exe --os=windows vs2022")

    print("done creating solution")
    print("if no errors accured you can open game.sln file with visual studio 2022 and build the project from there.")

else:
    print("Operating system not supported...\n this application only support windows and linux :(")
