import os

print("build options: ")
print("1: Developer - Linux")
print("2: User - Linux")
print("3: Developer - Windows")
print("4: User - Windows")
print("5: Abort")

valid = False
while True:
    choice = input("your choice: ")
    try:
        choice = int(choice)
    except ValueError:
        print("must enter a number!! try again.")
        continue
    if choice > 5 or choice < 1:
        print("must choose number between 1 and 5. try again")
    else:
        break

print("building . . .")


if choice == 1:
    # Build Debug configuration on Linux using GNU Make
    os.system("premake5 --os=linux gmake")
    os.system("make config=debug")
elif choice == 2:
    # Build Release configuration on Linux using GNU Make
    os.system("premake5 --os=linux gmake")
    os.system("make config=release")
elif choice == 3:
    # Build Debug configuration on Windows using Visual Studio 2019
    os.system("premake5 --os=windows vs2019")
    os.system("msbuild /p:Configuration=Debug")
elif choice == 4:
    # Build Release configuration on Windows using Visual Studio 2019
    os.system("premake5 --os=windows vs2019")
    os.system("msbuild /p:Configuration=Release")

print("done building.")
print("if no errors accured the executable can be found in the bin/Debug or bin/Release according to your build.")
