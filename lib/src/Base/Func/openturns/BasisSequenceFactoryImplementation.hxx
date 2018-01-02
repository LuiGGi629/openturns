//                                               -*- C++ -*-
/**
 *  @brief A factory for building BasisSequence objects
 *
 *  Copyright 2005-2018 Airbus-EDF-IMACS-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX
#define OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX

#include <iostream>
#include "openturns/PersistentObject.hxx"
#include "openturns/BasisSequence.hxx"
#include "openturns/Sample.hxx"
#include "openturns/DesignProxy.hxx"
#include "openturns/LeastSquaresMethod.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class BasisSequenceFactoryImplementation
 *
 * A factory for building BasisSequence objects
 */
class OT_API BasisSequenceFactoryImplementation
  : public PersistentObject
{
  friend class LeastSquaresMetaModelSelection;
  friend class SparseMethod;
  CLASSNAME
public:

  /** Default constructor */
  explicit BasisSequenceFactoryImplementation(const Bool verbose = false);

  /** Virtual constructor */
  virtual BasisSequenceFactoryImplementation * clone() const;

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** Method to create new BasisSequence objects */
  virtual BasisSequence build(const Sample & x,
                              const Sample & y,
                              const Basis & psi,
                              const Indices & indices);

#ifndef SWIG

  virtual BasisSequence build(const Sample & y,
                              const Indices & indices,
                              const DesignProxy & proxy);

  virtual BasisSequence build(LeastSquaresMethod & method,
                              const Sample & y);

  virtual void initialize();
  virtual void updateBasis(LeastSquaresMethod & method,
                           const Sample & y);
#endif

  /** Stopping criterion on the L1-norm of the coefficients accessor */
  void setMaximumRelativeConvergence(const Scalar coefficientsPaths);
  Scalar getMaximumRelativeConvergence() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** Verbosity flag */
  Bool verbose_;

  /** Stopping criterion on the L1-norm of the coefficients */
  Scalar maximumRelativeConvergence_;

  /** The collection of indices of the vectors in the partial basis with respect to the orthogonal basis */
  Indices currentIndices_;

  /** The ranks of the added vectors in the current partial basis */
  Indices addedPsi_k_ranks_;

  /** The ranks of the vectors common between the previous partial basis and the current partial basis, as found in the previous partial basis */
  Indices conservedPsi_k_ranks_;

  /** The ranks of the removed vectors in the previous partial basis */
  Indices removedPsi_k_ranks_;
}; /* class BasisSequenceFactoryImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX */
