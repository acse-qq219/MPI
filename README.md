# WINS MPI - FOR PERSONAL STUDY USE ONLY


## Configuration for MSVS
* Install MSVC C++ 2017 (or higher version) 
* Install [Miscroft MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
  1. download and install both ***msmpisdk.msi*** and ***msmpisetup.exe*** at default path
  2. For WINS, add path "C:\Program Files\Microsoft MPI\Bin\" to environment variables
* Create a new project in MSVC
  1. Right click on the project in ***Solution Explorer*** and find ***Properties***
  2. Find ***VC++ Dierctories*** in the LHS bar
  3. Add "C:\Program Files (x86)\Microsoft SDKs\Include" to the ***Include Directories***
  4. Add "C:\Program Files (x86)\Microsoft SDKs\Lib\x64" to the ***Library Directories***
or "...\Lib\x86" (depends on your debugger) 
  5. Add "msmmpi.lib" to the ***Additional Dependencies***


## Configuration for HPC (Imperial cx1 and cx2):
There are many ways to access cx1 and cx2, but the first thing is that you need to connect ***Imperial-WPA*** wifi, otherwise you can only access it through [Imperial vpn](https://www.imperial.ac.uk/admin-services/ict/self-service/connect-communicate/remote-access/virtual-private-network-vpn/).
```
Host: username@login.hpc.ic.ac.uk
Port: 22
```
* (with GUI, ease to use) use [FileZilla Client](https://filezilla-project.org/)

<img src="images/FileZilla Connection.png">

* (with GUI, recommended for host auto-saving and loading) use [WinSCP](https://winscp.net/eng/download.php)

<img src="images/WinSCP Connection.png">

* (with CLI, necessary for further use) use [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/)

<img src="images/PuTTY Connection.png">



## Start
Comments can be found in ".cpp" file

### Follow the [First Hello world program](lecture_0/lect0.cpp) to find your first "HELLO WORLD" code!
* In you MSVC, build it
* Compiling locally:
  1. enter the path where you can find your ".exe" file (it may be stored in "./MPI/x64/Debug")
  2. in your LINUX system like [Git Bash](https://gitforwindows.org/) , go to ***lect0.exe*** path
  3. Input `mpiexec -n #core# mpi_lecture0.exe` and replace `#core#` with any positive integer which stands for the number of cores you allocated.

* Compiling under cx1 and cx2:
  1. Connect to the server with PuTTY
  2. In your "/home" directory, create your own directory by typing
  ```
  mkdir MPI
  cd MPI
  mkdir lect0
  cd lect0
  ```
  3. Create a file called [Makefile](lecture_0/Makefile) in this directory (upload ***lect0.cpp*** here as well). Note that you should use LINUX EOF `LF` instead of WINS EOF `CR LF`, you may change that using [Notepad++](https://notepad-plus-plus.org/downloads/). Find ***EOL Conversion*** in top ***Edit*** bar, select ***Unix (LF)***
  ```
  MAKE = make                       // command to build
  TARGET = my_code                  // target filename
  SOURCE = Example_1.cpp            // source filename
  default:
  mpicxx -o $(TARGET) $(SOURCE)     // execution instruction 
  ```
  4. Load related module
  ```
   module load intel-suite
   module load mpi
  ```
  5. run ***Makefile*** by typing `make`
  6. In same directory, create a file called [my_script.pbs](lecture_0/my_script.pbs)
  ```
  #PBS -N my_first_job                               // name of the job
  #PBS -l walltime=00:10:00                          // maximum execution time (10 mins in this case)
  
  // "select" - number of machines requested
  // "ncpus" - number of cores per machine
  // "mpiprocs" - use number of cores of "ncpus"
  // "mem" - allowed memory usgae
  #PBS -l select=2:ncpus=12:mpiprocs=12:mem=1GB
  
  // you can load these module in putty CLI as well
  module load intel-suite
  module load mpi
  
  // include CORRECT path for your .exe code 
  mpiexec ~/MPI/lect0/exe_code_for_hpc
  ```
  7. Submit the job `qsub my_script.pbs`
  8. Check your job status by using `qstat -a` or check all queueing job `qstat - Q` 
  9. (If you want to kill a running or queueing job `qdel #number#.cx1` by replacing `#number#` to your identity number of your job in cx1
  10. After completing, you will get two files updated in your current directory. E.g. ***my_first_job.o#number#*** (output text sent to stdout) and ***my_first_job.e#number#*** (error text sent to stderr). You can check ***my_first_job.e#number#*** for output.
