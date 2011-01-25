#ifndef MDBUILDER_COMMON_HPP
#define MDBUILDER_COMMON_HPP

#include "../mdtrajview/VisBox.hpp"

#include <mdtk/tools.hpp>
#include <mdtk/SimLoop.hpp>

namespace mdbuilder
{

using namespace mdtk;

inline
mdtk::Vector3D getPosition()
{
  GLdouble m[16];
  glGetDoublev(GL_MODELVIEW_MATRIX,m);
  return Vector3D(m[3*4+0],m[3*4+1],m[3*4+2]);
}

inline
Atom*
place(ElementID id, mdtk::SimLoop& sl, Vector3D pos = getPosition())
{
  Atom* a = new Atom;
  a->ID = id;
  a->setAttributesByElementID();
  a->coords = pos;
  sl.atoms.push_back(a);
  return a;
}

}

#endif