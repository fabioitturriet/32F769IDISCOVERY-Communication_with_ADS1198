#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <touchgfx/hal/Types.hpp>
#include "main.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    virtual void SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala){}

    virtual void UpdateScrollList(){}

    virtual void ToCadastro(){}

    virtual void AtualizarBPM(){}

    virtual void AtualizarIconHeart(){}

    virtual void GeraPopup(){}

    virtual void PreStopLoadingAnimation(){}

    virtual void DefNumRedes(uint8_t NumeroRedes){}

    virtual void UpdateBatChargeLevel(){}

    virtual void UpdateMatrixLOFF(){}

    void bind(Model* m)
    {
        model = m;
    }
    virtual void UpdateGraph(float value){}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
