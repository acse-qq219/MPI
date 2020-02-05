# WINS MPI - FOR PERSONAL STUDY USE ONLY

## Configuration for MSVS
* Install MSVC C++ 2017 (or higher version) 
* Install [Miscroft MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
  1. download and install both `msmpisdk.msi` and `msmpisetup.exe` at default path
  2. For WINS, add path "C:\Program Files\Microsoft MPI\Bin\" to environment variables
* Create a new project in MSVC
  1. Right click on the project in `Solution Explorer` and find `Properties`
  2. Find `VC++ Dierctories` in the LHS bar
  3. Add "C:\Program Files (x86)\Microsoft SDKs\Include" to the `Include Directories`
  4. Add "C:\Program Files (x86)\Microsoft SDKs\Lib\x64" to the `Library Directories`
or "...\Lib\x86" (depends on your debugger) 
  5. Add "msmmpi.lib" to the `Additional Dependencies`

## Configuration for HPC (Imperial cx1 and cx2):
* There are many ways to access cx1 and cx2, but the first thing is that you need to connect `Imperial-WPA` wifi, otherwise you can only access it through [Imperial vpn](https://www.imperial.ac.uk/admin-services/ict/self-service/connect-communicate/remote-access/virtual-private-network-vpn/) :
  1. (with GUI, ease to use) use [FileZilla Client](https://filezilla-project.org/)
 
 ![Connected](image/FileZilla Connection.png?raw=true)

## Start
Comments can be found in `.cpp` file

### Follow the [First Hello world program](lect0.cpp) to find your first "HELLO WORLD" code!
* In you MSVC, build it
* Compiling locally:
  1. enter the path where you can find your `.exe` file (it may be stored in "./MPI/x64/Debug")
  2. in your LINUX system like [Git Bash](https://gitforwindows.org/) , go to `lect0.exe` path
  3. Input `mpiexec -n #core mpi_lecture0.exe` and replace `#core` with any positive integer which stands for the number of cores you allocated.

* Compiling under cx1 and cx2:
  1. Connect to the server
  2. In your `/home` directory, create a file called [Makefile](Makefile) in the directory (same with `.cpp` file) (Note that you should use LINUX EOF `LF` instead of WINS EOF `CR LF`)
  3. Load related module
  ```
   module load mpi
   module load intel-suite
  ```
  4. 
  
