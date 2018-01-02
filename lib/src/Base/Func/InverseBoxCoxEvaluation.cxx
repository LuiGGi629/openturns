//                                               -*- C++ -*-
/**
 * @brief Class for the inverse Box cox function
 *
 * Copyright 2005-2018 Airbus-EDF-IMACS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 */

#include "openturns/InverseBoxCoxEvaluation.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseBoxCoxEvaluation)

static const Factory<InverseBoxCoxEvaluation> Factory_InverseBoxCoxEvaluation;

/* Default constructor */
InverseBoxCoxEvaluation::InverseBoxCoxEvaluation()
  : EvaluationImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
InverseBoxCoxEvaluation::InverseBoxCoxEvaluation(const Point & lambda)
  : EvaluationImplementation()
  , lambda_(lambda)
  , shift_(lambda.getDimension())
{
  setInputDescription(Description::BuildDefault(lambda_.getSize(), "x"));
  setOutputDescription(Description::BuildDefault(lambda_.getSize(), "y"));
}

InverseBoxCoxEvaluation::InverseBoxCoxEvaluation(const Point & lambda,
    const Point & shift)
  : EvaluationImplementation()
  , lambda_(lambda)
  , shift_(shift)
{
  if (lambda.getDimension() != shift.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given exponent vector has a dimension=" << lambda.getDimension() << " different from the shift dimension=" << shift.getDimension();
  setInputDescription(Description::BuildDefault(lambda_.getSize(), "x"));
  setOutputDescription(Description::BuildDefault(lambda_.getSize(), "y"));
}

/* Clone constructor */
InverseBoxCoxEvaluation * InverseBoxCoxEvaluation::clone() const
{
  return new InverseBoxCoxEvaluation(*this);
}

/* Comparison operator */
Bool InverseBoxCoxEvaluation::operator ==(const InverseBoxCoxEvaluation & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_) && (shift_ == other.shift_);
}

/* String converter */
String InverseBoxCoxEvaluation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << InverseBoxCoxEvaluation::GetClassName()
      << " name=" << getName()
      << " dimension=" << getInputDimension()
      << " lambda=" << lambda_
      << " shift=" << shift_;
  return oss;
}

/* String converter __str__ */
String InverseBoxCoxEvaluation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "InverseBoxCox(lambda=" << lambda_
      << ", shift=" << shift_
      << ")";
  return oss;
}

/* Accessor for the lambda */
Point InverseBoxCoxEvaluation::getLambda() const
{
  return lambda_;
}

/* Accessor for the shift */
Point InverseBoxCoxEvaluation::getShift() const
{
  return shift_;
}

struct InverseBoxCoxEvaluationComputeSamplePolicy
{
  const Sample & input_;
  Sample & output_;
  const InverseBoxCoxEvaluation & evaluation_;

  InverseBoxCoxEvaluationComputeSamplePolicy(const Sample & input,
      Sample & output,
      const InverseBoxCoxEvaluation & evaluation)
    : input_(input)
    , output_(output)
    , evaluation_(evaluation)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i)
    {
      for (UnsignedInteger j = 0; j < evaluation_.getInputDimension(); ++j)
      {
        const Scalar lambda_j = evaluation_.getLambda()[j];
        const Scalar x = input_[i][j] - evaluation_.getShift()[j];
        if (std::abs(lambda_j * x * x) < 1e-8) output_[i][j] = exp(x) * (1.0 - 0.5 * lambda_j * x * x);
        else output_[i][j] = pow(lambda_j * x + 1.0, 1.0 / lambda_j);
      } // j
    } // i
  } // operator ()
}; /* end struct InverseBoxCoxEvaluationComputeSamplePolicy */

/* Operator () */
Sample InverseBoxCoxEvaluation::operator() (const Sample & inS) const
{
  if (inS.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << getInputDimension() << ", got " << inS.getDimension();
  const UnsignedInteger size = inS.getSize();
  Sample result(size, getInputDimension());
  const InverseBoxCoxEvaluationComputeSamplePolicy policy( inS, result, *this );
  TBB::ParallelFor( 0, size, policy );
  callsNumber_ += size;
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inS);
    outputStrategy_.store(result);
  }
  result.setDescription(getOutputDescription());
  return result;
}


/* Operator () */
Point InverseBoxCoxEvaluation::operator() (const Point & inP) const
{
  const UnsignedInteger dimension = getInputDimension();
  if (inP.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << dimension << ", got " << inP.getDimension();
  Point result(dimension);

  // There is no check of positive variables
  // This last one must be done by user or, as the evaluation is used in a stochastic context, in the BoxCoxTransform class
  for (UnsignedInteger index = 0; index < dimension; ++index)
  {
    const Scalar x = inP[index] - shift_[index];
    const Scalar lambda_i = lambda_[index];
    if (std::abs(lambda_i * x * x) < 1e-8) result[index] = exp(x) * (1.0 - 0.5 * lambda_i * x * x);
    else
    {
      const Scalar evaluation = lambda_i * x + 1.0;
      if (evaluation <= 0) throw InvalidArgumentException(HERE) << "Can not apply the inverse Box Cox function " ;
      result[index] = pow(evaluation, 1.0 / lambda_i);
    }
  }
  ++callsNumber_;
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inP);
    outputStrategy_.store(result);
  }
  return result;
}

/* Accessor for input point dimension */
UnsignedInteger InverseBoxCoxEvaluation::getInputDimension() const
{
  return lambda_.getDimension();
}

/* Accessor for output point dimension */
UnsignedInteger InverseBoxCoxEvaluation::getOutputDimension() const
{
  return lambda_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void InverseBoxCoxEvaluation::save(Advocate & adv) const
{
  EvaluationImplementation::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
  adv.saveAttribute( "shift_", shift_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseBoxCoxEvaluation::load(Advocate & adv)
{
  EvaluationImplementation::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  adv.loadAttribute( "shift_", shift_ );
}

END_NAMESPACE_OPENTURNS
