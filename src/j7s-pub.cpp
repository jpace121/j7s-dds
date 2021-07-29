#include "j7s-dds/j7s-pub.hpp"
#include "dds/dds.hpp"
#include <argparse/argparse.hpp>
#include <chrono>
#include <cmath>
#include <thread>
#include <iostream>
#include "j7s-dds/utils.hpp"

int main(int argc, char *argv[])
{
    // Process args.
    argparse::ArgumentParser program("j7s-pub");
    // clang-format off
    program.add_argument("-c", "--color")
        .required()
        .help("Color to set controlled LED.");
    program.add_argument("-i", "--index")
        .required()
        .help("LED index this publisher controls.")
        .action([](const std::string & value) { return std::stoi(value); });
    program.add_argument("-f", "--frequency")
        .default_value(1.0)
        .help("Frequency for sin wave.")
        .action([](const std::string & value) { return std::stod(value); });
    program.add_argument("-d", "--display-frequency")
        .default_value(10.0)
        .help("Frequency to publish a new decision.")
        .action([](const std::string & value) { return std::stod(value); });
    // clang-format on
    try
    {
        program.parse_args(argc, argv);

        validate_color(program.get<std::string>("--color"));
    }
    catch (const std::runtime_error & err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

    // Set up all the dds ojects.
    dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());
    dds::topic::Topic<LedState::Msg> topic(participant, "LedState");
    dds::pub::qos::PublisherQos pubQos = participant.default_publisher_qos();
    dds::pub::Publisher publisher(participant, pubQos);
    dds::pub::qos::DataWriterQos writerQos = publisher.default_datawriter_qos();
    dds::pub::DataWriter<LedState::Msg> writer(publisher, topic, writerQos);
    // Wait until we have a subscriber.
    dds::core::cond::StatusCondition statusCondition(writer);
    statusCondition.enabled_statuses(dds::core::status::StatusMask::publication_matched());
    dds::core::cond::WaitSet waitSet;
    waitSet.attach_condition(statusCondition);
    std::cout << "Waiting for subscriber." << std::endl;
    waitSet.wait();

    LedState::Msg message;
    message.index(program.get<int>("--index"));
    message.color(string_to_color(program.get<std::string>("--color")));

    double time = 0.0;
    const auto sleep_time = 1.0 / program.get<double>("--display-frequency");
    const auto sleep_duration = std::chrono::milliseconds(static_cast<int>(sleep_time * 1e3));
    const auto freq = program.get<double>("--frequency");
    while (true)
    {
        message.brightness(sin(2 * M_PI * time * freq));
        std::cout << "Publish brightness: " << message.brightness() << std::endl;
        writer.write(message);

        std::this_thread::sleep_for(sleep_duration);
        time += sleep_time;
    }

    return 0;
}

LedState::Color string_to_color(const std::string& color)
{
    if(color == "red")
    {
        return LedState::Color::RED;
    }
    else if(color == "lime")
    {
        return LedState::Color::LIME;
    }
    else if(color == "green")
    {
        return LedState::Color::GREEN;
    }
    else if(color == "blue")
    {
        return LedState::Color::BLUE;
    }
    else if(color == "white")
    {
        return LedState::Color::WHITE;
    }
    else if(color == "aqua")
    {
        return LedState::Color::AQUA;
    }
    else if(color == "off")
    {
        return LedState::Color::OFF;
    }
    else
    {
        throw std::logic_error("Invalid color provided.");
    }
}

