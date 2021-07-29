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
