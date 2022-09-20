#pragma once

#include "artery/application/ItsG5Service.h"

using namespace omnetpp;

// forward declaration
namespace traci { class VehicleController; }

// Classe que extende o serviço ITGG5 base para RECEBIMENTO de alertas
// de redução de velocidade através de um sinal do storyboard
class AlertReceiverService : public artery::ItsG5Service
{
    protected:
        // Sobrepõe o método indicate do serviço ITSG5 base
        void indicate(const vanetza::btp::DataIndication&, omnetpp::cPacket*) override;

        // Sobrepõe o método initialize do serviço ITSG5 base
        void initialize() override;

        cOutVector speedValuesVec;

        double safeDistance;

    private:
        // Reduz a velocidade de um veículo para um dado valor
        void slowDown(double speedValue);

        // Obtém um ponteiro para um controlador do veículo
        traci::VehicleController* mVehicleController = nullptr;
};

