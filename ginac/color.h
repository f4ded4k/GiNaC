/** @file color.h
 *
 *  Interface to GiNaC's color (SU(3) Lie algebra) objects. */

/*
 *  GiNaC Copyright (C) 1999-2001 Johannes Gutenberg University Mainz, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __GINAC_COLOR_H__
#define __GINAC_COLOR_H__

#include "indexed.h"
#include "tensor.h"

namespace GiNaC {


/** This class holds a generator T_a or the unity element of the Lie algebra
 *  of SU(3), as used for calculations in quantum chromodynamics. A
 *  representation label (an unsigned integer) is used to distinguish
 *  elements from different Lie algebra representations (only objects with
 *  the same label "interact" with each other). These objects implement an
 *  abstract representation of the group, not a specific matrix
 *  representation. The indices used for color objects should not have a
 *  variance. */
class color : public indexed
{
	GINAC_DECLARE_REGISTERED_CLASS(color, indexed)

	// other constructors
public:
	color(const ex & b, unsigned rl = 0);
	color(const ex & b, const ex & i1, unsigned rl = 0);

	// internal constructors
	color(unsigned rl, const exvector & v, bool discardable = false);
	color(unsigned rl, exvector * vp); // vp will be deleted

	// functions overriding virtual functions from base classes
protected:
	ex simplify_ncmul(const exvector & v) const;
	ex thisexprseq(const exvector & v) const;
	ex thisexprseq(exvector * vp) const;
	unsigned return_type(void) const { return return_types::noncommutative; }
	unsigned return_type_tinfo(void) const { return TINFO_color; }

	// member variables
private:
	unsigned representation_label; /**< Representation label to distinguish independent color matrices coming from separated fermion lines */
};


/** This class represents the su(3) unity element. */
class su3one : public tensor
{
	GINAC_DECLARE_REGISTERED_CLASS(su3one, tensor)

	// functions overriding virtual functions from bases classes
public:
	void print(const print_context & c, unsigned level = 0) const;
};

/** This class represents an su(3) generator. */
class su3t : public tensor
{
	GINAC_DECLARE_REGISTERED_CLASS(su3t, tensor)

	// functions overriding virtual functions from bases classes
public:
	void print(const print_context & c, unsigned level = 0) const;
};

/** This class represents the tensor of antisymmetric su(3) structure
 *  constants. */
class su3f : public tensor
{
	GINAC_DECLARE_REGISTERED_CLASS(su3f, tensor)

	// functions overriding virtual functions from bases classes
public:
	void print(const print_context & c, unsigned level = 0) const;
	ex eval_indexed(const basic & i) const;
	bool contract_with(exvector::iterator self, exvector::iterator other, exvector & v) const;
};

/** This class represents the tensor of symmetric su(3) structure constants. */
class su3d : public tensor
{
	GINAC_DECLARE_REGISTERED_CLASS(su3d, tensor)

	// functions overriding virtual functions from bases classes
public:
	void print(const print_context & c, unsigned level = 0) const;
	ex eval_indexed(const basic & i) const;
	bool contract_with(exvector::iterator self, exvector::iterator other, exvector & v) const;
};


// global functions
inline const color &ex_to_color(const ex &e)
{
	return static_cast<const color &>(*e.bp);
}


/** Create the su(3) unity element. This is an indexed object, although it
 *  has no indices.
 *
 *  @param rl Representation label
 *  @return newly constructed unity element */
ex color_ONE(unsigned rl = 0);

/** Create an su(3) generator.
 *
 *  @param a Index
 *  @param rl Representation label
 *  @return newly constructed unity generator */
ex color_T(const ex & a, unsigned rl = 0);

/** Create an su(3) antisymmetric structure constant.
 *
 *  @param a First index
 *  @param b Second index
 *  @param c Third index
 *  @return newly constructed structure constant */
ex color_f(const ex & a, const ex & b, const ex & c);

/** Create an su(3) symmetric structure constant.
 *
 *  @param a First index
 *  @param b Second index
 *  @param c Third index
 *  @return newly constructed structure constant */
ex color_d(const ex & a, const ex & b, const ex & c);

/** This returns the linear combination d.a.b.c+I*f.a.b.c. */
ex color_h(const ex & a, const ex & b, const ex & c);


} // namespace GiNaC

#endif // ndef __GINAC_COLOR_H__
