/** @file expairseq.h
 *
 *  Interface to sequences of expression pairs. */

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

#ifndef __GINAC_EXPAIRSEQ_H__
#define __GINAC_EXPAIRSEQ_H__

#include <vector>
#include <list>
// CINT needs <algorithm> to work properly with <vector> and <list>
#include <algorithm>

#include "expair.h"

namespace GiNaC {

/** Using hash tables can potentially enhance the asymptotic behaviour of
 *  combining n terms into one large sum (or n terms into one large product)
 *  from O(n*log(n)) to about O(n).  There are, however, several drawbacks.
 *  The constant in front of O(n) is quite large, when copying such an object
 *  one also has to copy the has table, comparison is quite expensive because
 *  there is no ordering any more, it doesn't help at all when combining two
 *  expairseqs because due to the presorted nature the behaviour would be
 *  O(n) anyways, the code is quite messy, etc, etc.  The code is here as
 *  an example for following generations to tinker with. */
#define EXPAIRSEQ_USE_HASHTAB 0

typedef std::vector<expair> epvector;       ///< expair-vector
typedef epvector::iterator epp;             ///< expair-vector pointer
typedef std::list<epp> epplist;             ///< list of expair-vector pointers
typedef std::vector<epplist> epplistvector; ///< vector of epplist

/** A sequence of class expair.
 *  This is used for time-critical classes like sums and products of terms
 *  since handling a list of coeff and rest is much faster than handling a
 *  list of products or powers, respectively. (Not incidentally, Maple does it
 *  the same way, maybe others too.)  The semantics is (at least) twofold:
 *  one for addition and one for multiplication and several methods have to
 *  be overridden by derived classes to reflect the change in semantics.
 *  However, most functionality turns out to be shared between addition and
 *  multiplication, which is the reason why there is this base class. */
class expairseq : public basic
{
	GINAC_DECLARE_REGISTERED_CLASS_NO_CTORS(expairseq, basic)

// member functions

	// default ctor, dtor, copy ctor assignment operator and helpers
public:
	expairseq() : basic(TINFO_expairseq)
#if EXPAIRSEQ_USE_HASHTAB
	                                    , hashtabsize(0)
#endif // EXPAIRSEQ_USE_HASHTAB
	{ }
	~expairseq() { destroy(false); }
	expairseq(const expairseq & other);
	const expairseq & operator=(const expairseq & other);
protected:
	void copy(const expairseq & other);
	void destroy(bool call_parent);
	// other ctors
public:
	expairseq(const ex & lh, const ex & rh);
	expairseq(const exvector & v);
	expairseq(const epvector & v, const ex & oc);
	expairseq(epvector * vp, const ex & oc); // vp will be deleted
	
	// functions overriding virtual functions from base classes
public:
	basic * duplicate() const;
	void print(const print_context & c, unsigned level = 0) const;
	unsigned precedence(void) const {return 10;}
	bool info(unsigned inf) const;
	unsigned nops() const;
	ex op(int i) const;
	ex & let_op(int i);
	ex map(map_function & f) const;
	ex eval(int level=0) const;
	ex to_rational(lst &repl_lst) const;
	bool match(const ex & pattern, lst & repl_lst) const;
	ex subs(const lst & ls, const lst & lr, bool no_pattern = false) const;
protected:
	int compare_same_type(const basic & other) const;
	bool is_equal_same_type(const basic & other) const;
	unsigned return_type(void) const;
	unsigned calchash(void) const;
	ex expand(unsigned options=0) const;
	
	// new virtual functions which can be overridden by derived classes
protected:
	virtual ex thisexpairseq(const epvector & v, const ex & oc) const;
	virtual ex thisexpairseq(epvector * vp, const ex & oc) const;
	virtual void printseq(const print_context & c, char delim,
	                      unsigned this_precedence,
	                      unsigned upper_precedence) const;
	virtual void printpair(const print_context & c, const expair & p,
	                       unsigned upper_precedence) const;
	virtual expair split_ex_to_pair(const ex & e) const;
	virtual expair combine_ex_with_coeff_to_pair(const ex & e,
												 const ex & c) const;
	virtual expair combine_pair_with_coeff_to_pair(const expair & p,
												   const ex & c) const;
	virtual ex recombine_pair_to_ex(const expair & p) const;
	virtual bool expair_needs_further_processing(epp it);
	virtual ex default_overall_coeff(void) const;
	virtual void combine_overall_coeff(const ex & c);
	virtual void combine_overall_coeff(const ex & c1, const ex & c2);
	virtual bool can_make_flat(const expair & p) const;
	
	// non-virtual functions in this class
protected:
	void construct_from_2_ex_via_exvector(const ex & lh, const ex & rh);
	void construct_from_2_ex(const ex & lh, const ex & rh);
	void construct_from_2_expairseq(const expairseq & s1,
	                                const expairseq & s2);
	void construct_from_expairseq_ex(const expairseq & s,
	                                 const ex & e);
	void construct_from_exvector(const exvector & v);
	void construct_from_epvector(const epvector & v);
	void make_flat(const exvector & v);
	void make_flat(const epvector & v);
	void canonicalize(void);
	void combine_same_terms_sorted_seq(void);
#if EXPAIRSEQ_USE_HASHTAB
	void combine_same_terms(void);
	unsigned calc_hashtabsize(unsigned sz) const;
	unsigned calc_hashindex(const ex & e) const;
	void shrink_hashtab(void);
	void remove_hashtab_entry(epvector::const_iterator element);
	void move_hashtab_entry(epvector::const_iterator oldpos,
	                        epvector::iterator newpos);
	void sorted_insert(epplist & eppl, epvector::const_iterator elem);
	void build_hashtab_and_combine(epvector::iterator & first_numeric,
	                               epvector::iterator & last_non_zero,
	                               vector<bool> & touched,
	                               unsigned & number_of_zeroes);
	void drop_coeff_0_terms(epvector::iterator & first_numeric,
	                        epvector::iterator & last_non_zero,
	                        vector<bool> & touched,
	                        unsigned & number_of_zeroes);
	bool has_coeff_0(void) const;
	void add_numerics_to_hashtab(epvector::iterator first_numeric,
	                             epvector::const_iterator last_non_zero);
#endif // EXPAIRSEQ_USE_HASHTAB
	bool is_canonical() const;
	epvector * expandchildren(unsigned options) const;
	epvector * evalchildren(int level) const;
	epvector * subschildren(const lst & ls, const lst & lr, bool no_pattern = false) const;
	
// member variables
	
protected:
	epvector seq;
	ex overall_coeff;
#if EXPAIRSEQ_USE_HASHTAB
	epplistvector hashtab;
	unsigned hashtabsize;
	unsigned hashmask;
	static unsigned maxhashtabsize;
	static unsigned minhashtabsize;
	static unsigned hashtabfactor;
#endif // EXPAIRSEQ_USE_HASHTAB
};

// utility functions

/** Return the expairseq object handled by an ex.  Deprecated: use ex_to<expairseq>().
 *  This is unsafe: you need to check the type first. */
inline const expairseq &ex_to_expairseq(const ex &e)
{
	return static_cast<const expairseq &>(*e.bp);
}

/** Specialization of is_exactly_a<expairseq>(obj) for expairseq objects. */
template<> inline bool is_exactly_a<expairseq>(const basic & obj)
{
	return obj.tinfo()==TINFO_expairseq;
}

} // namespace GiNaC

#endif // ndef __GINAC_EXPAIRSEQ_H__
