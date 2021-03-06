//                                               -*- C++ -*-
/**
 *  @brief The MarshallOlkinCopula distribution
 *
 *  Copyright 2005-2020 Airbus-EDF-IMACS-ONERA-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OPENTURNS_MARSHALLOLKINCOPULA_HXX
#define OPENTURNS_MARSHALLOLKINCOPULA_HXX

#include "openturns/CopulaImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class MarshallOlkinCopula
 *
 * The MarshallOlkinCopula distribution.
 */
class OT_API MarshallOlkinCopula
  : public CopulaImplementation
{
  CLASSNAME
public:

  /** Default constructor */
  MarshallOlkinCopula();

  /** Parameters constructor */
  MarshallOlkinCopula(const Scalar alpha,
                      const Scalar beta);

  /** Comparison operator */
  Bool operator ==(const MarshallOlkinCopula & other) const;
protected:
  Bool equals(const DistributionImplementation & other) const;
public:

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */
  /** Virtual constructor */
  virtual MarshallOlkinCopula * clone() const;

  /** Get the CDF of the distribution */
  using CopulaImplementation::computeCDF;
  Scalar computeCDF(const Point & point) const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Parameters value accessors */
  void setParameter(const Point & parameter);
  Point getParameter() const;

  /* Interface specific to MarshallOlkinCopula */

  /** Alpha accessor */
  void setAlpha(const Scalar alpha);
  Scalar getAlpha() const;

  /** Beta accessor */
  void setBeta(const Scalar beta);
  Scalar getBeta() const;

  /** Get the Kendall concordance of the distribution */
  CorrelationMatrix getKendallTau() const;

  /** Get the Spearman correlation of the distribution */
  CorrelationMatrix getSpearmanCorrelation() const;

  /** Get one realization of the distribution */
  Point getRealization() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

  /** The first parameter of the MarshallOlkinCopula distribution */
  Scalar alpha_;

  /** The second parameter of the MarshallOlkinCopula distribution */
  Scalar beta_;

}; /* class MarshallOlkinCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MARSHALLOLKINCOPULA_HXX */
