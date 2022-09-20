#include "AlertSenderService.h"
#include "artery/traci/VehicleController.h"
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>
#include "alert_msgs/SlowDownAlert_m.h"
#include <artery/application/StoryboardSignal.h>

using namespace omnetpp;
using namespace vanetza;

// Objeto que representa o sinal do storyboard
static const simsignal_t storyboardSignal = cComponent::registerSignal("StoryboardSignal");

Define_Module(AlertSenderService)

void AlertSenderService::initialize()
{
    // Iniciliza serviço e obtém objeto de controle do veículo
    ItsG5BaseService::initialize();
    mVehicleController = &getFacilities().get_const<traci::VehicleController>();
    subscribe(storyboardSignal);
    activatedAlert = false;
}

void AlertSenderService::trigger()
{
    Enter_Method("AlertSenderService trigger");
    if (activatedAlert) 
    {
        // Objeto de mensagem BTP
        btp::DataRequestB btpRequest;

        // Indica porta do serviço, tipo de transporte Single Hop Broadcast
        // Descentralized Congestion Control com prioridade DP1 e perfil de comunicação ITS-G5
        btpRequest.destination_port = host_cast<AlertSenderService::port_type>(getPortNumber());
        btpRequest.gn.transport_type = geonet::TransportType::SHB;
        btpRequest.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
        btpRequest.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

        // Obtém identificador do veículo e instância da API TraCI dele
        const std::string id = mVehicleController->getVehicleId();
        auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

        // Gera um pacote de 40 B com alerta de redução de velocidade 
        auto packet = new SlowDownAlert();
        packet->setSpeed(vehicle_api.getMaxSpeed(id));
        packet->setPosition(mVehicleController->getPosition());
        packet->setByteLength(40);
        request(btpRequest, packet);
    }
}

// É acionado sempre que uma mensagem é recebida
void AlertSenderService::receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t sig, omnetpp::cObject* sigobj, omnetpp::cObject*)
{
    // Verifica se a mensagem recebida é do storyboard. Se sim, o alerta é acionado
    if (sig == storyboardSignal) 
    {
        auto storysig = dynamic_cast<artery::StoryboardSignal*>(sigobj);
        if (storysig && storysig->getCause() == "Alert on") 
        {
            activatedAlert = true;
        }
    }
}