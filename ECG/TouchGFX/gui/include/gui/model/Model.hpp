#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void M_1kSPS_selected();
    void M_500SPS_selected();
    void M_250SPS_selected();
    void M_125SPS_selected();

    void M_Gain12_selected();
    void M_Gain8_selected();
    void M_Gain6_selected();
    void M_Gain4_selected();
    void M_Gain3_selected();
    void M_Gain2_selected();
    void M_Gain1_selected();

    void StartRDataC();
    void StopRDataC();

    void AjusteEscala();

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
