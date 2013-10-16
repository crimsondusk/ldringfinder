/*
 *  Copyright (C) 2013 Santeri Piippo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RINGFINDER_ALGORITHM_H
#define RINGFINDER_ALGORITHM_H

#include <QVector>

// =============================================================================
// RingFinder
//
// Provides an algorithm for finding a solution of rings between radii r0 and r1.
// =============================================================================
class RingFinder
{	public:
		struct Component
		{	int num;
			double scale;
		};

		class Solution
		{	public:
				// Components of this solution
				inline const QVector<Component>& components() const
				{	return m_components;
				}

				// Add a component to this solution
				void addComponent (const Component& a)
				{	m_components.push_back (a);
				}

				// Compare solutions
				bool operator> (const Solution& other) const;

			private:
				QVector<Component> m_components;
		};

		RingFinder() {}
		bool findRings (double r0, double r1);

		inline const Solution* bestSolution()
		{	return m_bestSolution;
		}

		inline const QVector<Solution>& allSolutions() const
		{	return m_solutions;
		}

		inline bool operator() (double r0, double r1)
		{	return findRings (r0, r1);
		}

	private:
		QVector<Solution> m_solutions;
		const Solution*   m_bestSolution;
		int               m_stack;

		bool findRingsRecursor (double r0, double r1, Solution& currentSolution);
};

// Templated absolute value
template<class T> static inline T abs (T a)
{	return (a >= 0) ? a : -a;
}

template<class T> inline bool isZero (T a)
{	return abs<T> (a) < 0.0001;
}

template<class T> inline bool isInteger (T a)
{	return isZero (a - (int) a);
}

extern RingFinder g_RingFinder;

#endif // RINGFINDER_ALGORITHM_H
