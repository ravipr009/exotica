/*
 *  Created on: 19 Apr 2014
 *      Author: Vladimir Ivan
 * 
 * Copyright (c) 2016, University Of Edinburgh 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of  nor the names of its contributors may be used to 
 *    endorse or promote products derived from this software without specific 
 *    prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE. 
 *
 */

/** \file UnconstrainedTimeIndexedProblem.h
 \brief Approximate Inference Control Problem specification */

#ifndef UNCONSTRAINETIMEINDEXEDPROBLEM_H_
#define UNCONSTRAINETIMEINDEXEDPROBLEM_H_

#include <exotica/PlanningProblem.h>
#include <exotica/UnconstrainedTimeIndexedProblemInitializer.h>

namespace exotica
{

  /**
   * \brief Unconstrained time-indexed problem.
   */
  class UnconstrainedTimeIndexedProblem: public PlanningProblem, public Instantiable<UnconstrainedTimeIndexedProblemInitializer>
  {
    public:
      UnconstrainedTimeIndexedProblem();
      virtual ~UnconstrainedTimeIndexedProblem();
      virtual void Instantiate(UnconstrainedTimeIndexedProblemInitializer& init);
      double getDuration();
      void Update(Eigen::VectorXdRefConst x, int t);
      void setGoal(const std::string & task_name, Eigen::VectorXdRefConst goal, int t = 0);
      void setRho(const std::string & task_name, const double rho, int t = 0);
      Eigen::VectorXd getGoal(const std::string & task_name, int t = 0);
      double getRho(const std::string & task_name, int t = 0);


      int T; //!< Number of time steps
      double tau; //!< Time step duration
      double Q_rate; //!< System transition error covariance multipler (per unit time) (constant throughout the trajectory)
      double H_rate; //!< Control error covariance multipler (per unit time) (constant throughout the trajectory)
      double W_rate; //!< Kinematic system transition error covariance multiplier (constant throughout the trajectory)
      Eigen::MatrixXd W;
      Eigen::MatrixXd H;
      Eigen::MatrixXd Q;

      std::vector<Eigen::VectorXd> Rho;
      std::vector<TaskSpaceVector> y;
      std::vector<TaskSpaceVector> Phi;
      std::vector<Eigen::VectorXd> ydiff;
      std::vector<Eigen::MatrixXd> J;

      int PhiN;
      int JN;
      int N;
      int NumTasks;

  };

  typedef std::shared_ptr<exotica::UnconstrainedTimeIndexedProblem> UnconstrainedTimeIndexedProblem_ptr;
}

#endif
