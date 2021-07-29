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

#include "dds/dds.hpp"
#include "LedState.hpp"
#include "blinkt_interface/blinkt.hpp"
#include "blinkt_interface/color.hpp"

class Subscriber : public dds::sub::NoOpDataReaderListener<LedState::Msg>
{

private:
    blinkt_interface::Blinkt& _blinkt;

public:
    Subscriber(blinkt_interface::Blinkt& blinkt);

private:
    blinkt_interface::Pixel msg_to_pixel(const LedState::Msg& msg) const;

    void on_data_available(dds::sub::DataReader<LedState::Msg>& reader) override;

};
