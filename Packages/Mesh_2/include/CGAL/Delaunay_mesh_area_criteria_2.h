// Copyright (c) 2003-2004  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Laurent RINEAU

#ifndef CGAL_DELAUNAY_MESH_AREA_CRITERIA_2_H
#define CGAL_DELAUNAY_MESH_AREA_CRITERIA_2_H

#include <CGAL/Delaunay_mesh_size_criteria_2.h>

namespace CGAL {

template <class Tr>
class Delaunay_mesh_area_criteria_2 
  : public virtual Delaunay_mesh_criteria_2<Tr>,
    private Delaunay_mesh_size_criteria_2<Tr>
/* This class "is a" Delaunay_mesh_criteria_2<Tr> and is implemented by
   Delaunay_mesh_size_criteria_2<Tr>. Delaunay_mesh_criteria_2<Tr> is a
   virtual base class of Delaunay_mesh_size_criteria_2<Tr>. */
{
public:
  typedef Delaunay_mesh_criteria_2<Tr> Base;
  typedef Delaunay_mesh_size_criteria_2<Tr> Private_base;

  typedef typename Delaunay_mesh_size_criteria_2<Tr>::Quality Quality;

  Delaunay_mesh_area_criteria_2(const double aspect_bound = 0.125, 
			      const double area_bound = 0)
    : Private_base(aspect_bound, area_bound) {};

  inline
  double area_bound() const { return this->sizebound; };

  inline
  void set_area_bound(const double ab) { this->sizebound = ab; };

  class Is_bad: public Private_base::Is_bad
  {
  public:
    typedef typename Private_base::Is_bad Is_bad_base;

    typedef typename Tr::Point Point_2;
    typedef typename Tr::Triangle Triangle_2;
    typedef typename Tr::Face_handle Face_handle;

    Is_bad(const double aspect_bound,
	   const double area_bound)
      : Is_bad_base(aspect_bound, area_bound) {};

    bool operator()(const Face_handle& fh,
		    Quality& q) const
    {
      typedef typename Tr::Geom_traits Geom_traits;

      typedef typename Geom_traits::Point_2 Point_2;
      typedef typename Geom_traits::Triangle_2 Triangle_2;
      typedef typename Geom_traits::Compute_area_2 Compute_area_2;
      typedef typename Geom_traits::Compute_squared_distance_2
	Compute_squared_distance_2;
      typedef typename Geom_traits::FT FT;


      Geom_traits geom_traits;

      Compute_area_2 area_2 = geom_traits.compute_area_2_object();
      Compute_squared_distance_2 squared_distance = 
	geom_traits.compute_squared_distance_2_object();

      const Point_2& pa = fh->vertex(0)->point();
      const Point_2& pb = fh->vertex(1)->point();
      const Point_2& pc = fh->vertex(2)->point();

      Triangle_2 t = geom_traits.construct_triangle_2_object()(pa,pb,pc);
      double area = 2*CGAL::to_double(area_2(t));
      area=area*area; // squared area

      double
	a = CGAL::to_double(squared_distance(pb, pc)),
	b = CGAL::to_double(squared_distance(pc, pa)),
	c = CGAL::to_double(squared_distance(pa, pb));

      double min_sine; // squared minimum sine

      if(a<b)
	if(a<c)
	  min_sine = area/(b*c);
	else
	  min_sine = area/(a*b);
      else
	if(b<c)
	  min_sine = area/(a*c);
	else
	  min_sine = area/(a*b);
      
      q.first = min_sine;
      q.second = area;

      if( min_sine < this->B ) return true;
      if( this->SB == 0 ) return false;
      return ( area > this->SB );
    };
  };

  Is_bad is_bad_object() const
  { return Is_bad(this->bound(), area_bound()); }
};

} //end namespace

#endif
