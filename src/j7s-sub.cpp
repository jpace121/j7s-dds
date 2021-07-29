// Copyright 2021 James Pace
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "j7s-dds/j7s-sub.hpp"

int main(int argc, char *argv[])
{
    dds::domain::DomainParticipant participant(org::eclipse::cyclonedds::domain::default_id());

    dds::topic::Topic<LedState::Msg> topic(participant, "LedState");
    dds::sub::qos::SubscriberQos subQos = participant.default_subscriber_qos();
    dds::sub::Subscriber subscriber(participant, subQos);
    dds::sub::qos::DataReaderQos readerQos = subscriber.default_datareader_qos();
    dds::sub::DataReader<LedState::Msg> reader(subscriber, topic, readerQos);


    blinkt_interface::Blinkt blinkt;
    reader.listener(new Subscriber(blinkt), dds::core::status::StatusMask::data_available());

    while(true){}

    return 0;
}

Subscriber::Subscriber(blinkt_interface::Blinkt& blinkt):
    _blinkt{blinkt}
{
}

void Subscriber::on_data_available(dds::sub::DataReader<LedState::Msg>& reader)
{
    const auto samples = reader.take();
    if(samples.length() > 0)
    {
        for(auto& sample : samples)
        {
            const auto msg = sample.data();
            const auto info = sample.info();

            if(info.valid())
            {

                if(msg.index() < _blinkt.number_of_pixels())
                {
                    _blinkt.setPixel(msg.index(), msg_to_pixel(msg));
                    _blinkt.display();
                }
                else
                {
                    std::cerr << "Invalid index. What?" << std::endl;
                }
            }
        }
    }
}

blinkt_interface::Pixel Subscriber::msg_to_pixel(const LedState::Msg& msg) const
{
    switch(msg.color())
    {
        case LedState::Color::RED:
            return blinkt_interface::color::red(msg.brightness());
        case LedState::Color::LIME:
            return blinkt_interface::color::lime(msg.brightness());
        case LedState::Color::GREEN:
            return blinkt_interface::color::green(msg.brightness());
        case LedState::Color::BLUE:
            return blinkt_interface::color::blue(msg.brightness());
        case LedState::Color::WHITE:
            return blinkt_interface::color::white(msg.brightness());
        case LedState::Color::AQUA:
            return blinkt_interface::color::aqua(msg.brightness());
        default:
            return blinkt_interface::color::off(msg.brightness());
    }
}

