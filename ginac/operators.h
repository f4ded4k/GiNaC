/** @file operators.h
 *
 *  Interface to GiNaC's overloaded operators. */

/*
 *  GiNaC Copyright (C) 1999-2000 Johannes Gutenberg University Mainz, Germany
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

#ifndef __GINAC_OPERATORS_H__
#define __GINAC_OPERATORS_H__

#include <iostream>

#ifndef NO_NAMESPACE_GINAC
namespace GiNaC {
#endif // ndef NO_NAMESPACE_GINAC

class ex;
class numeric;
class relational;

// binary arithmetic operators ex with ex
ex operator+(const ex & lh, const ex & rh);
ex operator-(const ex & lh, const ex & rh);
ex operator*(const ex & lh, const ex & rh);
ex operator/(const ex & lh, const ex & rh);
ex operator%(const ex & lh, const ex & rh); // non-commutative multiplication

// binary arithmetic operators numeric with numeric
numeric operator+(const numeric & lh, const numeric & rh);
numeric operator-(const numeric & lh, const numeric & rh);
numeric operator*(const numeric & lh, const numeric & rh);
numeric operator/(const numeric & lh, const numeric & rh);

// binary arithmetic assignment operators with ex
const ex & operator+=(ex & lh, const ex & rh);
const ex & operator-=(ex & lh, const ex & rh);
const ex & operator*=(ex & lh, const ex & rh);
const ex & operator/=(ex & lh, const ex & rh);
const ex & operator%=(ex & lh, const ex & rh); // non-commutative multiplication

// binary arithmetic assignment operators with numeric
const numeric & operator+=(numeric & lh, const numeric & rh);
const numeric & operator-=(numeric & lh, const numeric & rh);
const numeric & operator*=(numeric & lh, const numeric & rh);
const numeric & operator/=(numeric & lh, const numeric & rh);

// unary operators
ex operator+(const ex & lh);
ex operator-(const ex & lh);

numeric operator+(const numeric & lh);
numeric operator-(const numeric & lh);
numeric& operator++(numeric & rh);
numeric& operator--(numeric & rh);
numeric operator++(numeric & lh, int);
numeric operator--(numeric & lh, int);

// binary relational operators ex with ex
relational operator==(const ex & lh, const ex & rh);
relational operator!=(const ex & lh, const ex & rh);
relational operator<(const ex & lh, const ex & rh);
relational operator<=(const ex & lh, const ex & rh);
relational operator>(const ex & lh, const ex & rh);
relational operator>=(const ex & lh, const ex & rh);

// input/output stream operators
ostream & operator<<(ostream & os, const ex & e);
istream & operator>>(istream & is, ex & e);

#ifndef NO_NAMESPACE_GINAC
} // namespace GiNaC
#endif // ndef NO_NAMESPACE_GINAC

#endif // ndef __GINAC_OPERATORS_H__
