/* 
   mdtrajsim (molecular dynamics trajectory simulator)

   Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011
   Oleksandr Yermolenko <oleksandr.yermolenko@gmail.com>

   This file is part of MDTK, the Molecular Dynamics Toolkit.

   MDTK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MDTK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MDTK.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <mdtk/SimLoop.hpp>

#include "../common.h"

using namespace mdtk;

class CustomSimLoop : public SimLoop
{
public:
  CustomSimLoop();
  void doBeforeIteration();
  void doAfterIteration();
};

CustomSimLoop::CustomSimLoop()
  :SimLoop()
{
}

void
CustomSimLoop::doBeforeIteration()
{
  if (simTime < 4.0*ps) simTimeSaveTrajInterval = 0.05*ps;
  else simTimeSaveTrajInterval = 0.2*ps;
}

void
CustomSimLoop::doAfterIteration()
{
}

bool
isAlreadyFinished();

int runTraj(std::string inputFile);

#ifdef MPIBATCH

#define main_mpibatch main

#include <dirent.h>

void
addTrajDirNames(std::vector<std::string> &stateFileNames,const char *trajsetDir_)
{
  std::cout << "Adding states from " << trajsetDir_ << std::endl;

  char trajsetDir[10000];
  strcpy(trajsetDir,trajsetDir_);

  if (trajsetDir[strlen(trajsetDir)-1] != DIR_DELIMIT_CHAR)
    strcat(trajsetDir,DIR_DELIMIT_STR);
  
  {
    {
      char trajdir_src[10000];
      char stateFileName[10000];

      DIR* trajsetDirHandle = opendir(trajsetDir);
      REQUIRE(trajsetDirHandle != NULL);


      struct dirent* entry = readdir(trajsetDirHandle);
      while (entry != NULL)
      {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
//        if (entry->d_name[0] == '0')
        {
          std::sprintf(trajdir_src,"%s%s",trajsetDir,entry->d_name);
          std::sprintf(stateFileName,"%s"DIR_DELIMIT_STR,trajdir_src);
//          if (passedTheFilter(stateFileName))
          stateFileNames.push_back(stateFileName);
//          TRACE(stateFileName);
        }
        entry = readdir(trajsetDirHandle);
      };

      int res_closedir = closedir(trajsetDirHandle);
      REQUIRE(res_closedir == 0);
    }
  }  
}

#include <mpi.h>
#include <algorithm>

int
main_mpibatch(int argc , char *argv[])
{
  MPI_TEST_SUCCESS(MPI_Init(&argc,&argv));

  int comm_rank;
  int comm_size;
  char comm_name[MPI_MAX_PROCESSOR_NAME+1]; int len;
  MPI_TEST_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD,&comm_size));
  MPI_TEST_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD,&comm_rank));
  MPI_TEST_SUCCESS(MPI_Get_processor_name(comm_name,&len));
    if (comm_rank == 0) {
    std::cout << "mdtrajsim (Molecular dynamics trajectory simulator), mpibatch version ";
    mdtk::release_info.print();
    }
//  std::cerr << "I'm " << rank << " of " << size << ". My name is " << name << std::endl;
  fflush(stderr);
  fprintf(stderr,"I'm %d of %d. My name is %s.\n",comm_rank,comm_size,comm_name);
//  std::cout << "I'm "%d" of "%d". My name is "%s".\n";
//  std::cerr << "RRRRRRRRRRRRRRRRRRRRRRRRR\n";
  fflush(stderr);


  MPI_Barrier(MPI_COMM_WORLD);

std::vector<std::string> trajDirNames;
addTrajDirNames(trajDirNames,"./");
sort(trajDirNames.begin(),trajDirNames.end());


    if (comm_rank == 0) {
/*
for(size_t i = 0; i < trajDirNames.size(); i++)
  TRACE(trajDirNames[i]);
*/
    } // if (rank == 0)
size_t firstTraj = ceil(double(trajDirNames.size())/comm_size)*comm_rank;
size_t lastTraj  = ceil(double(trajDirNames.size())/comm_size)*(comm_rank+1);

for(size_t ti = firstTraj; ti < lastTraj; ti++)
{
  if (ti >= trajDirNames.size()) break;
  TRACE(comm_rank);
  TRACE(trajDirNames[ti]);
yaatk::chdir(trajDirNames[ti].c_str());
yaatk::mkdir("_zipped_tmp");
    std::streambuf* cout_sbuf = std::cout.rdbuf(); // save original sbuf
    std::ofstream   fcout("stdout.txt",std::ios::app);
    std::cout.rdbuf(fcout.rdbuf()); // redirect 'cout' to a 'fout'
    std::streambuf* cerr_sbuf = std::cerr.rdbuf();
    std::ofstream   fcerr("stderr.txt",std::ios::app);
    std::cerr.rdbuf(fcerr.rdbuf());
runTraj("in.mde");
    std::cout.rdbuf(cout_sbuf); // restore the original stream buffer
    std::cerr.rdbuf(cerr_sbuf); // restore the original stream buffer
yaatk::chdir("..");
}

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_TEST_SUCCESS(MPI_Finalize());

  return 0;
}

#else
#define main_seq main
#endif

int
main_seq(int argc , char *argv[])
{
  procmon::ProcmonTimer pmtTotal;

  TRACE(pmtTotal.getDeltaTimeInSeconds());
  double val;
  for(double x = 0; x < 100000000; x+=1)
  {
    val += sqrt(x);
  }
  TRACE(pmtTotal.getDeltaTimeInSeconds());
  for(double x = 0; x < 100000000; x+=1)
  {
    val += (x*x);
  }
  TRACE(pmtTotal.getDeltaTimeInSeconds());

  TRACE(val);

  return 0;

  if (argc > 1 && !strcmp(argv[1],"--version"))
  {
    std::cout << "mdtrajsim (Molecular dynamics trajectory simulator) ";
    mdtk::release_info.print();
    return 0;
  }

  if (argc > 1 && (!std::strcmp(argv[1],"--help") || !std::strcmp(argv[1],"-h")))
  {
    std::cout << "\
Usage: mdtrajsim [OPTION]... [FILE]\n\
Simulates molecular dynamics trajectory described in the FILE\
 (in.mde.gz file in the current directory by default)\n\
\n\
      --help     display this help and exit\n\
      --version  output version information and exit\n\
\n\
Report bugs to <oleksandr.yermolenko@gmail.com>\n\
";
    return 0;
  }

  std::string inputFile = "in.mde";
  if (argc > 1) inputFile = argv[1];

  return runTraj(inputFile);
}

int runTraj(std::string inputFile)
{
  if (isAlreadyFinished()) return 0;

try
{
  CustomSimLoop mdloop;

  setupPotentials(mdloop);
  if (yaatk::exists("simloop.conf") || yaatk::exists("simloop.conf.bak")) // have to continue interrupted simulation ?
  {
    mdloop.loadstate();
  }
  else
  {
    yaatk::text_ifstream fi(inputFile.c_str());
    mdloop.loadFromMDE(fi);
//    mdloop.loadFromMDE_OLD(fi);
    fi.close();
  yaatk::text_ofstream fo1("mde""_init");
    mdloop.saveToStream(fo1);
  fo1.close();
  }

  mdloop.execute();

  if (mdloop.simTime >= mdloop.simTimeFinal) // is simulation really finished ?
  {
    yaatk::text_ofstream fo2("mde""_final");
    mdloop.saveToStream(fo2);
    fo2.close();
    mdloop.writetrajXVA();
  }
}  
catch(mdtk::Exception& e)
{ 
  std::cerr << "MDTK exception in the main thread: "
            << e.what() << std::endl;
  std::cerr << "Probably wrong input file format or no input file (default is in.mde)" << std::endl;
  return 1;
}
catch(...)
{
  std::cerr << "Unknown exception in the main thread." << std::endl; 
  return 2;
}  

  return 0;
}

bool
isAlreadyFinished()
{
  {
    if (yaatk::exists("mde_final")) return true;
  }
  {
    std::ifstream ifinal("in.mde.after_crash");
    if (ifinal)
      return true;
    else
      ifinal.close();
  }
  return false;
}

