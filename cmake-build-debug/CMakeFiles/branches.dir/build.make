# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\the_E\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\201.7223.86\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\the_E\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\201.7223.86\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\OneDrive - University of Pisa\Shared Projects\branches"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles\branches.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\branches.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\branches.dir\flags.make

CMakeFiles\branches.dir\main.cpp.obj: CMakeFiles\branches.dir\flags.make
CMakeFiles\branches.dir\main.cpp.obj: ..\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/branches.dir/main.cpp.obj"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\branches.dir\main.cpp.obj /FdCMakeFiles\branches.dir\ /FS -c "D:\OneDrive - University of Pisa\Shared Projects\branches\main.cpp"
<<

CMakeFiles\branches.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/branches.dir/main.cpp.i"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" > CMakeFiles\branches.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\OneDrive - University of Pisa\Shared Projects\branches\main.cpp"
<<

CMakeFiles\branches.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/branches.dir/main.cpp.s"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\branches.dir\main.cpp.s /c "D:\OneDrive - University of Pisa\Shared Projects\branches\main.cpp"
<<

CMakeFiles\branches.dir\branches_core.cpp.obj: CMakeFiles\branches.dir\flags.make
CMakeFiles\branches.dir\branches_core.cpp.obj: ..\branches_core.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/branches.dir/branches_core.cpp.obj"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\branches.dir\branches_core.cpp.obj /FdCMakeFiles\branches.dir\ /FS -c "D:\OneDrive - University of Pisa\Shared Projects\branches\branches_core.cpp"
<<

CMakeFiles\branches.dir\branches_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/branches.dir/branches_core.cpp.i"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" > CMakeFiles\branches.dir\branches_core.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\OneDrive - University of Pisa\Shared Projects\branches\branches_core.cpp"
<<

CMakeFiles\branches.dir\branches_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/branches.dir/branches_core.cpp.s"
	"D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\branches.dir\branches_core.cpp.s /c "D:\OneDrive - University of Pisa\Shared Projects\branches\branches_core.cpp"
<<

# Object files for target branches
branches_OBJECTS = \
"CMakeFiles\branches.dir\main.cpp.obj" \
"CMakeFiles\branches.dir\branches_core.cpp.obj"

# External object files for target branches
branches_EXTERNAL_OBJECTS =

branches.exe: CMakeFiles\branches.dir\main.cpp.obj
branches.exe: CMakeFiles\branches.dir\branches_core.cpp.obj
branches.exe: CMakeFiles\branches.dir\build.make
branches.exe: CMakeFiles\branches.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable branches.exe"
	C:\Users\the_E\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\201.7223.86\bin\cmake\win\bin\cmake.exe -E vs_link_exe --intdir=CMakeFiles\branches.dir --rc="C:\PROGRA~2\Windows Kits\10\bin\10.0.18362.0\x86\rc.exe" --mt="C:\PROGRA~2\Windows Kits\10\bin\10.0.18362.0\x86\mt.exe" --manifests  -- "D:\Program Files\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx86\x86\link.exe" /nologo @CMakeFiles\branches.dir\objects1.rsp @<<
 /out:branches.exe /implib:branches.lib /pdb:"D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug\branches.pdb" /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\branches.dir\build: branches.exe

.PHONY : CMakeFiles\branches.dir\build

CMakeFiles\branches.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\branches.dir\cmake_clean.cmake
.PHONY : CMakeFiles\branches.dir\clean

CMakeFiles\branches.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "D:\OneDrive - University of Pisa\Shared Projects\branches" "D:\OneDrive - University of Pisa\Shared Projects\branches" "D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug" "D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug" "D:\OneDrive - University of Pisa\Shared Projects\branches\cmake-build-debug\CMakeFiles\branches.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles\branches.dir\depend

