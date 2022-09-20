#pragma once

#include "artery/application/ItsG5Service.h"

// forward declaration
namespace traci { class VehicleController; }

// Classe que extende o serviço ITGG5 base para ENVIO de alertas
// de redução de velocidade através de um sinal do storyboard
class AlertSenderService : public artery::ItsG5Service
{
    public:
        // Sobrepõe o método initialize do serviço ITSG5 base
        void initialize() override;

        // Sobrepõe o método trigger do serviço ITSG5 base
        void trigger() override;

        // Sobrepõe o método receiveSignal do serviço ITSG5 base
        void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, omnetpp::cObject*, omnetpp::cObject*) override;

    protected:
        // Obtém um ponteiro para um controlador do veículo
        const traci::VehicleController* mVehicleController = nullptr;

        // Booleana que indica se o veículo deve emitir o alerta
        bool activatedAlert = false;
};