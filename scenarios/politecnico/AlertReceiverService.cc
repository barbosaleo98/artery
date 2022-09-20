#include <boost/geometry/algorithms/distance.hpp>
#include "artery/traci/VehicleController.h"
#include "alert_msgs/SlowDownAlert_m.h"
#include "AlertReceiverService.h"
#include "artery/utility/Geometry.h"

using namespace omnetpp;
using namespace vanetza;

Define_Module(AlertReceiverService)

void AlertReceiverService::initialize()
{
    // Iniciliza serviço e obtém objeto de controle do veículo
    ItsG5BaseService::initialize();
    mVehicleController = &getFacilities().get_mutable<traci::VehicleController>();

    speedValuesVec.setName("speedValues:vector");

    safeDistance = par("safeDist");
}

void AlertReceiverService::indicate(const vanetza::btp::DataIndication& ind, omnetpp::cPacket* packet)
{
    Enter_Method("AlertReceiverService indicate");

    // Objeto referente a mensagem do tipo SlowDownAlert
    auto slowDownMessage = check_and_cast<const SlowDownAlert*>(packet);

    // Obtém identificador do veículo e API de controle dele
    const std::string id = mVehicleController->getVehicleId();
    auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

    speedValuesVec.record(vehicle_api.getSpeed(id));

    // Valor de velocidade máxima indicado na mensagem de alerta
    const double newMaxSpeed = slowDownMessage->getSpeed();

    // Posição indicado na mensagem de alerta
    const artery::Position alertPosition = slowDownMessage->getPosition();

    const artery::Position currentPosition = mVehicleController->getPosition();

    // Se o veículo tem velocidade máxima acima do indicado, deve reduzí-la
    if (vehicle_api.getMaxSpeed(id) >= newMaxSpeed && distance(currentPosition, alertPosition) < safeDistance * boost::units::si::meter) 
    {
        slowDown(newMaxSpeed);
    }

    // Destrói mensagem de alerta para recebimento de outra caso necessário
    delete slowDownMessage;
}

void AlertReceiverService::slowDown(double speedValue)
{
    // Utiliza o controlador do veículo para alterar a velocidade máxima
    // Note que é necessário também aplicar uma grandeza do pacote boost
    mVehicleController->setMaxSpeed(speedValue * units::si::meter_per_second);
}