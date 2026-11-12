//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "TrafficGen.h"

Define_Module(TrafficGen);

void TrafficGen::initialize(){
   // id of the destination aircraft of the generated msgs
   AircraftId_ = getIndex();

   sendingTimer_ = new cMessage("sendTimer");

   // We read parameter "T" which defines the random interval
   scheduleAt(simTime() + par("interGenerationTime"), sendingTimer_);
   p_ = par("p");

}


void TrafficGen::handleMessage(cMessage *msg)
{
    // the timer expires -> a new msg must be sent to the correct Aircraft
    if (msg->isSelfMessage()) {
        AircraftMessage* newMsg = createMsg_(AircraftId_);
        send(newMsg,"out");
        simtime_t interGenerationTime_ = par("interGenerationTime");
        EV_INFO << "TrafficGen[" << AircraftId_<< "]: InterGenerationTime: " << interGenerationTime_ << endl;
        scheduleAt(simTime() + interGenerationTime_, sendingTimer_);
    }
    else {
        EV_WARN << "Error in TrafficGen: HandleMessage received a non self message" << endl;
    }
}

AircraftMessage* TrafficGen::createMsg_(int aircraftNumber) {
    double linkRV = par("linkRV");
    int link = linkRV <= p_ ? 0 : 1;
    int size = par("size");
    AircraftMessage* newMsg = new AircraftMessage(nullptr, aircraftNumber, link, size);
    return newMsg;
}
TrafficGen::~TrafficGen(){
    cancelAndDelete(sendingTimer_);
}

