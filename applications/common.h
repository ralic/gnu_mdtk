/*
   Common configuration for mdtrajsim and mdtrajview.

   Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013
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

#ifndef mdtk_apps_common_h
#define mdtk_apps_common_h

#include <mdtk/potentials/pairwise/FBM.hpp>
#include <mdtk/potentials/pairwise/FBZL.hpp>
#include <mdtk/potentials/pairwise/FLJ.hpp>
#include <mdtk/potentials/manybody/AIREBO/AIREBO_LJ.hpp>
#include <mdtk/potentials/manybody/AIREBO/AIREBO_ETors.hpp>
#include <mdtk/potentials/manybody/TightBinding/TightBinding.hpp>
#include <mdtk/potentials/manybody/Ackland/Ackland.hpp>
#include <mdtk/SimLoop.hpp>
#include <mdtk/SimLoopSaver.hpp>

inline
void
setupPotentials(mdtk::SimLoop& simloop)
{
  using namespace mdtk;

  REQUIRE(simloop.fpot.potentials.size()==0);

  mdtk::FGeneral* pot = NULL;

  pot = new mdtk::FBZL(Rcutoff(5.0*Ao,5.5*Ao));
  simloop.fpot.addPotential(pot);

  pot = new mdtk::FLJ(Rcutoff(5.0*Ao,5.5*Ao));
  simloop.fpot.addPotential(pot);

#ifdef  AIREBO_USING_BRENNER
  pot = new mdtk::Brenner(Brenner::POTENTIAL2);
  simloop.fpot.addPotential(pot);
#endif
#ifdef  AIREBO_USING_REBO
  pot = new mdtk::REBO();
  simloop.fpot.addPotential(pot);
#endif

  pot = new mdtk::AIREBO((CREBO*)pot);
  simloop.fpot.addPotential(pot);

  pot = new mdtk::ETors();
  simloop.fpot.addPotential(pot);

  pot = new mdtk::Ackland();
  simloop.fpot.addPotential(pot);
/*
  pot = new mdtk::TightBinding();
  simloop.fpot.addPotential(pot);
*/
/*
  pot = new mdtk::Brenner(Brenner::POTENTIAL2);
  simloop.fpot.addPotential(pot);
*/
}

#endif

