#include <exotica/Tools/Conversions.h>
#include <algorithm>

namespace Eigen
{
    Eigen::VectorXd VectorTransform(double px, double py, double pz, double qx, double qy, double qz, double qw)
    {
        Eigen::VectorXd ret((Eigen::VectorXd(7) << px,py,pz,qx,qy,qz,qw).finished());
        return ret;
    }

    Eigen::VectorXd IdentityTransform()
    {
        return VectorTransform();
    }
}

namespace exotica
{
    KDL::Frame getFrame(Eigen::VectorXdRefConst val)
    {
        switch(val.rows())
        {
        case 7:
            return KDL::Frame(KDL::Rotation::Quaternion(val(3),val(4),val(5),val(6)),KDL::Vector(val(0),val(1),val(2)));
        case 6:
            return KDL::Frame(KDL::Rotation::RPY(val(3),val(4),val(5)),KDL::Vector(val(0),val(1),val(2)));
        case 3:
            return KDL::Frame(KDL::Vector(val(0),val(1),val(2)));
        default:
            throw_pretty("Eigen vector has incorrect length! ("+std::to_string(val.rows())+")");
        }
    }

    KDL::Frame getFrameFromMatrix(Eigen::MatrixXdRefConst val)
    {
        switch(val.cols())
        {
        case 1:
            return getFrame(val);
        case 4:
            switch(val.rows())
            {
            case 3:
            case 4:
                return KDL::Frame(KDL::Rotation(val(0,0),val(0,1),val(0,2),val(1,0),val(1,1),val(1,2),val(2,0),val(2,1),val(2,2)),KDL::Vector(val(0,3),val(1,3),val(2,3)));
            default:
                throw_pretty("Eigen matrix has incorrect number of rows! ("+std::to_string(val.rows())+")");
            }
        default:
            throw_pretty("Eigen matrix has incorrect number of columns! ("+std::to_string(val.cols())+")");
        }
    }

    Eigen::MatrixXd getFrame(const KDL::Frame& val)
    {
        Eigen::MatrixXd ret = Eigen::MatrixXd::Identity(4,4);
        ret(0,3) = val.p(0);
        ret(1,3) = val.p(1);
        ret(2,3) = val.p(2);
        ret.block(0,0,3,3) = Eigen::Map<const Eigen::Matrix3d>(val.M.data);
        return ret;
    }

    Eigen::VectorXd getFrameAsVector(const KDL::Frame& val, RotationType type)
    {
        Eigen::VectorXd ret(3+getRotationTypeLength(type));
        ret(0) = val.p[0];
        ret(1) = val.p[1];
        ret(2) = val.p[2];
        ret.segment(3,getRotationTypeLength(type)) = setRotation(val.M, type);
        return ret;
    }

    KDL::Rotation getRotation(Eigen::VectorXdRefConst data, RotationType type)
    {
        switch(type)
        {
        case RotationType::QUATERNION:
            if(data.sum()==0.0) throw_pretty("Invalid quaternion transform!");
            return KDL::Rotation::Quaternion(data(0), data(1), data(2), data(3));
        case RotationType::RPY:
            return KDL::Rotation::RPY(data(0), data(1), data(2));
        case RotationType::ZYX:
            return KDL::Rotation::EulerZYX(data(0), data(1), data(2));
        case RotationType::ZYZ:
            return KDL::Rotation::EulerZYZ(data(0), data(1), data(2));
        case RotationType::ANGLE_AXIS:
            {
                KDL::Vector axis(data(0), data(1), data(2));
                double angle = axis.Norm();
                if(fabs(angle)>1e-10)
                {
                    return KDL::Rotation::Rot(axis, angle);
                }
                else
                {
                    return KDL::Rotation();
                }
            }
        case RotationType::MATRIX:
            if(data.sum()==0.0) throw_pretty("Invalid matrix transform!");
            return KDL::Rotation(data(0), data(1), data(2),
                                 data(3), data(4), data(5),
                                 data(6), data(7), data(8));
        }
        throw_pretty("Unknown rotation represntation type!");
    }

    Eigen::VectorXd setRotation(const KDL::Rotation& data, RotationType type)
    {
        Eigen::VectorXd ret;
        switch(type)
        {
        case RotationType::QUATERNION:
            ret.resize(4);
            data.GetQuaternion(ret(0), ret(1), ret(2), ret(3));
            return ret;
        case RotationType::RPY:
            ret.resize(3);
            data.GetRPY(ret(0), ret(1), ret(2));
            return ret;
        case RotationType::ZYX:
            ret.resize(3);
            data.GetEulerZYX(ret(0), ret(1), ret(2));
            return ret;
        case RotationType::ZYZ:
            ret.resize(3);
            data.GetEulerZYZ(ret(0), ret(1), ret(2));
            return ret;
        case RotationType::ANGLE_AXIS:
            ret = Eigen::Map<const Eigen::Vector3d>(data.GetRot().data);
            return ret;
        case RotationType::MATRIX:
            ret = Eigen::Map<const Eigen::VectorXd>(data.data, 9);
            return ret;
        }
        throw_pretty("Unknown rotation represntation type!");
    }

    bool contains(std::string key, const std::vector<std::string>& vec)
    {

        if(std::find(vec.begin(),vec.end(),key)==vec.end()) return false;
        return true;
    }

    void vectorExoticaToEigen(const exotica::Vector & exotica,
        Eigen::VectorXd & eigen)
    {
      eigen.resize(exotica.data.size());
      for (int i = 0; i < exotica.data.size(); i++)
        eigen(i) = exotica.data[i];
    }

    void vectorEigenToExotica(Eigen::VectorXd eigen, exotica::Vector & exotica)
    {
      exotica.data.resize(eigen.rows());
      for (int i = 0; i < eigen.rows(); i++)
        exotica.data[i] = eigen(i);
    }

    void matrixExoticaToEigen(const exotica::Matrix & exotica,
        Eigen::MatrixXd & eigen)
    {
      if (exotica.col == 0 || exotica.row == 0 || exotica.data.size() == 0)
      {
        throw_pretty("Matrix conversion failed, no data in the matrix.");
      }
      if (exotica.col * exotica.row != exotica.data.size())
      {
        throw_pretty(
            "Matrix conversion failed, size mismatch."<<exotica.col<<" * "<<exotica.row<<" != "<<exotica.data.size());
      }
      eigen.resize(exotica.row, exotica.col);
      int cnt = 0;
      for (int r = 0; r < exotica.row; r++)
        for (int c = 0; c < exotica.col; c++)
        {
          eigen(r, c) = exotica.data[cnt];
          cnt++;
        }
    }

    void matrixEigenToExotica(const Eigen::MatrixXd & eigen,
        exotica::Matrix & exotica)
    {
      exotica.row = eigen.rows();
      exotica.col = eigen.cols();
      exotica.data.resize(exotica.col * exotica.row);
      int cnt = 0;
      for (int r = 0; r < exotica.row; r++)
        for (int c = 0; c < exotica.col; c++)
        {
          exotica.data[cnt] = eigen(r, c);
          cnt++;
        }
    }
}
