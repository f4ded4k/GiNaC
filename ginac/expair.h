/** @file expair.h
 *
 *  Definition of expression pairs (building blocks of expairseq). */

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

#ifndef __GINAC_EXPAIR_H__
#define __GINAC_EXPAIR_H__

#include "ex.h"
#include "numeric.h"
#include "print.h"

namespace GiNaC {

/** A pair of expressions.
 *  This is similar to STL's pair<>.  It is slightly extended since we need to
 *  account for methods like .compare().  Also, since this is meant for use by
 *  class expairseq it must satisfy the invariance that the member coeff must
 *  be of type numeric. */
class expair
{
public:
	expair() : rest(0), coeff(1) { }
	~expair() { }
	expair(const expair & other) : rest(other.rest), coeff(other.coeff)
	{
		GINAC_ASSERT(is_exactly_a<numeric>(coeff));
	}
	const expair & operator=(const expair & other)
	{
		if (this != &other) {
			rest = other.rest;
			coeff = other.coeff;
		}
		return *this;
	}
	
	/** Construct an expair from two ex. */
	expair(const ex & r, const ex & c) : rest(r), coeff(c)
	{
		GINAC_ASSERT(is_exactly_a<numeric>(coeff));
	}
	
	/** Member-wise check for canonical ordering equality. */
	bool is_equal(const expair & other) const
	{
		return (rest.is_equal(other.rest) && coeff.is_equal(other.coeff));
	}
	
	/** Member-wise check for canonical ordering lessness. */
	bool is_less(const expair & other) const 
	{
		int restcmp = rest.compare(other.rest);
		return ((restcmp<0) ||
		        (!(restcmp>0) && (coeff.compare(other.coeff)<0)));
	}
	
	/** Member-wise check for canonical ordering. */
	int compare(const expair & other) const
	{
		int restcmp = rest.compare(other.rest);
		if (restcmp!=0)
			return restcmp;
		else
			return coeff.compare(other.coeff);
	}
	
	void print(std::ostream & os) const
	{
		os << "expair:";
		print_tree c(os);
		rest.print(c, c.delta_indent);
		coeff.print(c, c.delta_indent);
	}
	
	/** True if this is of the form (numeric,ex(1)). */
	bool is_canonical_numeric(void) const
	{
		GINAC_ASSERT(is_exactly_a<numeric>(coeff));
		return (is_ex_exactly_of_type(rest,numeric) &&
		        (coeff.is_equal(1)));
	}

	/** Swap contents with other expair. */
	void swap(expair & other)
	{
		rest.swap(other.rest);
		coeff.swap(other.coeff);
	}
	
	ex rest;    ///< first member of pair, an arbitrary expression
	ex coeff;   ///< second member of pair, must be numeric
};

/** Function objects for insertion into third argument of STL's sort() etc. */
struct expair_is_less : public std::binary_function<expair, expair, bool> {
	bool operator()(const expair &lh, const expair &rh) const { return lh.is_less(rh); }
};

struct expair_swap : public std::binary_function<expair, expair, void> {
	void operator()(expair &lh, expair &rh) const { lh.swap(rh); }
};

} // namespace GiNaC

#endif // ndef __GINAC_EXPAIR_H__
