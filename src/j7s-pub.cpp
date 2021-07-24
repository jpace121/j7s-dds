#include "dds/dds.hpp"
#include "LedState.hpp"

void loop(dds::pub::DataWriter<LedState::Msg>& writer)
{
    
}

int main(int argc, char *argv[])
{
    dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());

    dds::topic::Topic<LedState::Msg> topic(participant, "LedState");

    dds::pub::qos::PublisherQos pubQos = participant.default_publisher_qos();
    dds::pub::Publisher publisher(participant, pubQos);


    dds::pub::qos::DataWriterQos writerQos = publisher.default_datawriter_qos();
    dds::pub::DataWriter<LedState::Msg> writer(publisher, topic, writerQos);

    dds::core::cond::StatusCondition statusCondition(writer);
    statusCondition.enabled_statuses(dds::core::status::StatusMask::publication_matched());
    dds::core::cond::WaitSet waitSet;
    waitSet.attach_condition(statusCondition);
    waitSet.wait();

    return 0;
}
