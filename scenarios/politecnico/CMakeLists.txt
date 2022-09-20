add_artery_feature(slowDownAlert AlertSenderService.cc AlertReceiverService.cc)
generate_opp_message(SlowDownAlert.msg TARGET slowDownAlert DIRECTORY alert_msgs)
add_opp_run(politecnico NED_FOLDERS ${CMAKE_CURRENT_SOURCE_DIR})

