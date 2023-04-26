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

    void Config12Dev();
    void Config3Dev();

    void AjusteEscala();


    void M_InitESP();
    void WiFiESP_OFF();

    void SetModoNormal();
    void SetModoTeste();
    void SetModoMensRuido();

    void AtualizarListaRedes();

    void NextDerivSelect();
    void BackDerivSelect();

    void MConnectWifi();

    void TransmiteWifiECG();

    void CalcularBPM();
    void StopPanTompkins();

    void Set_LOFF_off();
    void Set_LOFF_DCResistor();
    void Set_LOFF_DCSource();
    void Set_LOFF_AC();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
