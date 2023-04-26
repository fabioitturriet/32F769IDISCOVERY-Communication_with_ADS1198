#ifndef CONTAINERLOFFMATRIX_HPP
#define CONTAINERLOFFMATRIX_HPP

#include <gui_generated/containers/ContainerLOFFMatrixBase.hpp>

class ContainerLOFFMatrix : public ContainerLOFFMatrixBase
{
public:
    ContainerLOFFMatrix();
    virtual ~ContainerLOFFMatrix() {}

    virtual void initialize();

    virtual void UpdateMatrixStatusLOFF();
protected:
};

#endif // CONTAINERLOFFMATRIX_HPP
