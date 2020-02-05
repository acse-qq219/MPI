# MPI - FOR PERSONAL STUDY USE ONLY

## Configuration
1. Install MSVC C++ 2017 (or higher version) 
2. Install [Miscroft MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
* download and install both `msmpisdk.msi` and `msmpisetup.exe` at default path
* For WINS, add path "C:\Program Files\Microsoft MPI\Bin\" to environment variables
3. Create a new project in MSVC
* Right click on the project in `Solution Explorer` and find `Properties`
* Find `VC++ Dierctories` in the LHS bar
* Add "C:\Program Files (x86)\Microsoft SDKs\Include" to the `Include Directories`
* Add "C:\Program Files (x86)\Microsoft SDKs\Lib\x64" to the `Library Directories`
or "...\Lib\x86" (depends on your debugger) 
* Add "msmmpi.lib" to the `Additional Dependencies`

## Start
Comments can be found in .cpp file

* [First Hello world program]
